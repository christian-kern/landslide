/**
 * @file schedule.h
 * @brief Thread scheduling logic for landslide
 * @author Ben Blum
 */

#ifndef __LS_SCHEDULE_H
#define __LS_SCHEDULE_H

#include <simics/api.h>

#include "common.h"
#include "kernel_specifics.h"
#include "lockset.h"
#include "memory.h"
#include "stack.h"
#include "user_sync.h"
#include "variable_queue.h"
#include "vector_clock.h"

struct ls_state;

/* The agent represents a single thread, or active schedulable node on the
 * runqueue. */
struct agent {
	unsigned int tid;
	/* Link in our runqueue */
	Q_NEW_LINK(struct agent) nobe;
	/* state tracking for what the corresponding kthread is up to */
	struct {
		/* is there a timer handler frame on this thread's stack?
		 * FIXME: similar for keyboard will be needed */
		bool handling_timer;
		/* are they in the context switcher? (note: this and
		 * handling_$INTERRUPT are not necessarily linked!) */
		bool context_switch;
		/* Threads get a "free pass" out of the context switch exiting
		 * assertion the first time, since some kernels have newly
		 * forked threads not exit through the context switcher, so we
		 * start all threads with the above flag turned off.
		 * This makes the assertion weaker than if we got to use
		 * kern_fork_returns_to_cs, but we can't use it, so I think this
		 * is the strongest we can do besides. */
		bool cs_free_pass;
		/* are they about to create a new thread? */
		bool forking;
		/* about to take a spin on the sleep queue? */
		bool sleeping;
		bool spleeping; // pintos
		/* do they have properties of red wizard? */
		bool vanishing;
		/* are they reading lines */
		bool readlining;
		/* have they not even had a chance to run yet? */
		bool just_forked;
		/* special action flag for ignoring heap accesses */
		bool lmm_init;
		/* special action flag for being in vm_do_user_copy in pathos */
		bool vm_user_copy;
		/* special action flag for blocking on disk I/O in pintos */
		bool disk_io;
		/* are they taking or releasing a mutex? */
		bool kern_mutex_locking;
		bool kern_mutex_unlocking;
		bool kern_mutex_trylocking;
		/* what about in userspace? */
		bool user_mutex_initing;
		bool user_mutex_locking;
		bool user_mutex_unlocking;
		bool user_mutex_yielding; /* if true, locking addr shall also be set */
		bool user_mutex_destroying;
		bool user_cond_waiting;
		bool user_cond_signalling;
		bool user_cond_broadcasting;
		bool user_sem_proberen;
		bool user_sem_verhogen;
		bool user_rwlock_locking;
		bool user_rwlock_unlocking;
		bool user_locked_mallocing;
		bool user_locked_callocing;
		bool user_locked_reallocing;
		bool user_locked_freeing;
		/* are we trying to schedule this agent? */
		bool schedule_target;
	} action;
#ifdef ALLOW_REENTRANT_MALLOC_FREE
	/* If the dynamic allocator is allowed to reenter itself, such as in
	 * Pintos, then the action flags for tracking the malloc/free boundaries
	 * must be stored per-thread. If it's forbidden, you can find these
	 * flags in the global mem_state instead. */
	struct malloc_actions kern_malloc_flags;
	struct malloc_actions user_malloc_flags;
#endif
	/* For noob deadlock detection. The pointer might not be set; if it is
	 * NULL but the tid field is not -1, it should be computed. */
	struct agent *kern_blocked_on;
	unsigned int kern_blocked_on_tid;
	/* action.locking implies addr is valid; also kern_blocked_on set implies
	 * locking, which implies addr is valid. -1 if nothing. */
	unsigned int kern_blocked_on_addr;
	unsigned int kern_mutex_unlocking_addr;
	/* similar for userspace */
	bool user_mutex_recently_unblocked; /* tricky_disco.ogg */
	unsigned int user_blocked_on_addr;
	unsigned int user_mutex_initing_addr;
	unsigned int user_mutex_locking_addr;
	unsigned int user_mutex_unlocking_addr;
	unsigned int user_rwlock_locking_addr;
	// int user_rwlock_unlocking_addr; // not needed
	/* for helpful debug info on user page faults */
	unsigned int last_pf_eip;
	unsigned int last_pf_cr2; /* valid iff eip above != -1 */
	/* Whether we just inserted a dummy instruction before a suspected data
	 * race instruction (to delay its access until after the save point). */
	bool just_delayed_for_data_race;
	unsigned int delayed_data_race_eip; /* ...and if so, where was it */
#ifdef PREEMPT_EVERYWHERE
	bool preempt_for_shm_here;
#endif
	/* Same as above but used when exiting a VR yield to a new thread. */
	bool just_delayed_for_vr_exit;
	unsigned int delayed_vr_exit_eip; /* ...and if so, where was it */
	/* used to narrow down data race candidates based on eip */
	unsigned int most_recent_syscall;
	unsigned int last_call; /* like a mini (much faster) stack trace */
	/* locks held for data race detection */
	struct lockset kern_locks_held;
	struct lockset user_locks_held;
#ifdef PURE_HAPPENS_BEFORE
	struct vector_clock clock;
#endif
	/* State for tracking userspace synchronization actions */
	struct user_yield_state user_yield;
	/* Possible stack trace saved from before sim_unreg_process. */
	struct stack_trace *pre_vanish_trace;
	/* Used by partial order reduction, only in "oldsched"s in the tree. */
	bool do_explore;
};

Q_NEW_HEAD(struct agent_q, struct agent);

#define BLOCKED(a) \
	((a)->kern_blocked_on_tid != -1 || (a)->user_blocked_on_addr != -1 || \
	 agent_is_user_yield_blocked(&(a)->user_yield))

#define INITING_SOMETHING(a) ((a)->action.user_mutex_initing)
#define DESTROYING_SOMETHING(a) ((a)->action.user_mutex_destroying)

#define IN_USER_MALLOC_WRAPPERS(a)		\
	((a)->action.user_locked_mallocing ||	\
	 (a)->action.user_locked_callocing ||	\
	 (a)->action.user_locked_reallocing ||	\
	 (a)->action.user_locked_freeing)

#define IN_USER_SYNC_PRIMITIVES(a)		\
	((a)->action.user_mutex_locking ||	\
	 (a)->action.user_mutex_unlocking ||	\
	 (a)->action.user_cond_waiting ||	\
	 (a)->action.user_cond_signalling ||	\
	 (a)->action.user_cond_broadcasting ||	\
	 (a)->action.user_sem_proberen ||	\
	 (a)->action.user_sem_verhogen ||	\
	 (a)->action.user_rwlock_locking ||	\
	 (a)->action.user_rwlock_unlocking)

#define USER_SYNC_ACTION_STR(a)						\
	((a)->action.user_mutex_locking     ? "mutex_lock" :		\
	 (a)->action.user_mutex_unlocking   ? "mutex_unlock" :		\
	 (a)->action.user_cond_waiting      ? "cond_wait" :		\
	 (a)->action.user_cond_signalling   ? "cond_signal" :		\
	 (a)->action.user_cond_broadcasting ? "cond_broadcast" :	\
	 (a)->action.user_sem_proberen      ? "sem_wait" :		\
	 (a)->action.user_sem_verhogen      ? "sem_signal" :		\
	 (a)->action.user_rwlock_locking    ? "rwlock_lock" :		\
	 (a)->action.user_rwlock_unlocking  ? "rwlock_unlock" :		\
	 "<unknown>")

/* Internal state for the scheduler.
 * If you change this, make sure to update save.c! */
struct sched_state {
	/* Reflection of the currently runnable threads in the guest kernel */
	struct agent_q rq;
	/* Reflection of threads which exist but are not runnable */
	struct agent_q dq;
	/* Reflection of threads which will become runnable on their own time */
	struct agent_q sq;
	/* Currently active thread */
	struct agent *cur_agent;
	struct agent *last_agent;
	/* Denotes whether the current thread is not on the runqueue and yet is
	 * runnable anyway. (True only in some kernels.) */
	bool current_extra_runnable;
	/* Counters - useful for telling when tests begin and end */
	unsigned int num_agents;
	unsigned int most_agents_ever;
	/* See agent_vanish for justification */
	struct agent *last_vanished_agent;
	/* List of known semaphores that were initialized with values other than
	 * 1 (i.e., ones that don't behave like mutexes for sake of locksets). */
	struct lockset known_semaphores;
#ifdef PURE_HAPPENS_BEFORE
	/* vector clocks that track time from the perspective of each lock;
	 * iow, the "L" map from the fasttrack paper. tracks only kernel or
	 * only user locks depending on which space we're testing, not both. */
	struct lock_clocks lock_clocks;
	/* special case lock clock entry for cli/sti (or scheduler locking) */
	struct vector_clock scheduler_lock_clock;
	/* not necessarily matching interrupts state, since we ignore cli/sti
	 * during timer or context switch. but needed for tracking "handoff". */
	bool scheduler_lock_held;
#endif
	/* Avoid false positive deadlocks caused by ad-hoc yield-blocking. */
	unsigned int deadlock_fp_avoidance_count;
	/* ICB */
	unsigned int icb_preemption_count;
	/* Did the guest finish initialising its own state */
	bool guest_init_done;
	/* It does take many instructions for us to switch, after all. This is
	 * NULL if we're not trying to schedule anybody. */
	struct agent *schedule_in_flight;
	unsigned int inflight_tick_count;
	bool delayed_in_flight;
	bool just_finished_reschedule;
	/* Whether we think we ought to be entering the timer handler or not. */
	bool entering_timer;
	/* The stack trace for the most recent voluntary reschedule. Used in
	 * save.c, when voluntary resched decision points are too late to get
	 * a useful stack trace. Set at context switch entry. */
	unsigned int voluntary_resched_tid;
	struct stack_trace *voluntary_resched_stack;
	/* TODO: have a scheduler-global schedule_landing to assert against the
	 * per-agent flag (only violated by interrupts we don't control) */
};

#define EVAPORATE_FLOW_CONTROL(code) \
	do {								\
	bool __nested_loop_guard = true;				\
	do {								\
		assert(__nested_loop_guard &&				\
		       "Illegal 'continue' in nested loop macro");	\
		__nested_loop_guard = false;				\
		code;							\
		__nested_loop_guard = true;				\
	} while (!__nested_loop_guard);					\
	assert(__nested_loop_guard &&					\
	       "Illegal 'break' in nested loop macro");			\
	} while (0)

/* Can't be used with 'break' or 'continue', though 'return' is fine. */
#define FOR_EACH_RUNNABLE_AGENT(a, s, code) do {	\
	bool __idle_is_runnable = true;			\
	if ((s)->current_extra_runnable &&		\
	    /* skip cur_agent if it's idle (see #74) */	\
	    !TID_IS_IDLE((s)->cur_agent->tid)) {	\
		a = (s)->cur_agent;			\
		__idle_is_runnable = false;		\
		EVAPORATE_FLOW_CONTROL(code);		\
	}						\
	Q_FOREACH(a, &(s)->rq, nobe) {			\
		if (TID_IS_IDLE(a->tid))		\
			continue;			\
		__idle_is_runnable = false;		\
		EVAPORATE_FLOW_CONTROL(code);		\
	}						\
	Q_FOREACH(a, &(s)->sq, nobe) {			\
		if (TID_IS_IDLE(a->tid))		\
			continue; /* why it sleep?? */	\
		__idle_is_runnable = false;		\
		EVAPORATE_FLOW_CONTROL(code);		\
	}						\
	if (__idle_is_runnable && kern_has_idle()) {	\
		a = agent_by_tid_or_null(&(s)->dq, kern_get_idle_tid()); \
		if (a == NULL)				\
			a = agent_by_tid_or_null(&(s)->rq, kern_get_idle_tid()); \
		assert(a != NULL && "couldn't find idle in FOR_EACH");	\
		EVAPORATE_FLOW_CONTROL(code);		\
	}						\
	} while (0)

/* For purposes of ICB, would switching to the given thread constitute a
 * preemption? Note that in a voluntary resched senario, both cur and last
 * agence are allowed (i.e., we could choose them even when the ICB bound
 * is exceeded), as we may need to run one or the other for correctness. */
#define NO_PREEMPTION_REQUIRED(s, voluntary, a) ({			\
	struct agent *__a = (a);					\
	struct sched_state *____s = (s); /* wtf gcc wrt shadowing  */	\
	(__a == ____s->cur_agent ||					\
	 ((voluntary) && __a == ____s->last_agent)); })

#ifdef ICB
#define ICB_BLOCKED(s, bound, voluntary, a) ({			\
	struct sched_state *__s = (s);				\
	(__s->icb_preemption_count >= (bound) &&		\
	 !NO_PREEMPTION_REQUIRED(__s, voluntary, a)); })
#else
#define ICB_BLOCKED(ls, bound, voluntary, a) false
#endif

struct agent *agent_by_tid_or_null(struct agent_q *, unsigned int tid);

void sched_init(struct sched_state *);

void print_agent(verbosity v, const struct agent *);
void print_qs(verbosity v, const struct sched_state *);
void print_scheduler_state(verbosity v, const struct sched_state *);
struct agent *find_agent(struct sched_state *s, unsigned int tid);
struct agent *find_runnable_agent(struct sched_state *s, unsigned int tid);

/* called at every "interesting" point ... */
void sched_update(struct ls_state *);
/* called after time-travel */
void sched_recover(struct ls_state *);

#endif
