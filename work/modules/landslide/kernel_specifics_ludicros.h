/**
 * @file kernel_specifics_ludicros.h
 * @brief #defines for the ludicros guest kernel (automatically generated)
 * @author Ben Blum <bblum@andrew.cmu.edu>
 */

#ifndef __LS_KERNEL_SPECIFICS_LUDICROS_H
#define __LS_KERNEL_SPECIFICS_LUDICROS_H

#include "x86.h"

#define GUEST_ESP0_ADDR (0x00100930 + 4)
#define GUEST_ESP0(cpu) READ_MEMORY(cpu, GUEST_ESP0_ADDR)
#define GUEST_TCB_STATE_FLAG_OFFSET 20

#define GUEST_TIMER_WRAP_ENTER     0x00101298
#define GUEST_TIMER_WRAP_EXIT      0x101c21
#define GUEST_CONTEXT_SWITCH_ENTER 0x00104917
#define GUEST_CONTEXT_SWITCH_EXIT  0x1049a4

#define GUEST_READLINE_WINDOW_ENTER 0x00105020
#define GUEST_READLINE_WINDOW_EXIT 0x1050cb

#define GUEST_LMM_ALLOC_ENTER      0x0010a0c4
#define GUEST_LMM_ALLOC_EXIT       0x10a2f3
#define GUEST_LMM_ALLOC_SIZE_ARGNUM 2
#define GUEST_LMM_ALLOC_GEN_ENTER  0x001069f4
#define GUEST_LMM_ALLOC_GEN_EXIT   0x106d40
#define GUEST_LMM_ALLOC_GEN_SIZE_ARGNUM 2
#define GUEST_LMM_FREE_ENTER       0x00106690
#define GUEST_LMM_FREE_EXIT        0x1068ce
#define GUEST_LMM_FREE_BASE_ARGNUM 2
#define GUEST_LMM_FREE_SIZE_ARGNUM 3
#define GUEST_LMM_REMOVE_FREE_ENTER 0x001068e0
#define GUEST_LMM_REMOVE_FREE_EXIT 0x1069f1

#define GUEST_IMG_END 0x00256a8c
#define GUEST_DATA_START 0x0024e000
#define GUEST_DATA_END 0x00256014
#define GUEST_BSS_START 0x00256014
#define GUEST_BSS_END GUEST_IMG_END
#define GUEST_PANIC 0x001083f8
#define GUEST_KERNEL_MAIN 0x00105630

#define TELL_LANDSLIDE_THREAD_SWITCH 0x00105c48
#define TELL_LANDSLIDE_SCHED_INIT_DONE 0x00105c4d
#define TELL_LANDSLIDE_FORKING 0x00105c52
#define TELL_LANDSLIDE_VANISHING 0x00105c57
#define TELL_LANDSLIDE_SLEEPING 0x00105c5c
#define TELL_LANDSLIDE_THREAD_RUNNABLE 0x00105c61
#define TELL_LANDSLIDE_THREAD_DESCHEDULING 0x00105c66
#define TELL_LANDSLIDE_MUTEX_LOCKING 0x00105c6b
#define TELL_LANDSLIDE_MUTEX_BLOCKING 0x00105c70
#define TELL_LANDSLIDE_MUTEX_LOCKING_DONE 0x00105c75
#define TELL_LANDSLIDE_MUTEX_UNLOCKING 0x00105c7a
#define TELL_LANDSLIDE_MUTEX_UNLOCKING_DONE 0x00105c7f

#define GUEST_SCHEDULER_FUNCTIONS { \
	{ 0x0010439a, 0x1043f4 }, \
	{ 0x00104326, 0x104399 }, \
	{ 0x001042d7, 0x104325 }, \
	{ 0x0010428e, 0x1042d6 }, \
	{ 0x0010485f, 0x1048da }, \
	{ 0x00104917, 0x1049a4 }, \
	{ 0x001043f5, 0x10444a }, \
	{ 0x00104a54, 0x104a6c }, \
	{ 0x00104a6d, 0x104a70 }, \
	{ 0x0010472c, 0x10485e }, \
	{ 0x001049a5, 0x104a06 }, \
	}
#define GUEST_SCHEDULER_GLOBALS { \
	{ 0x0025604c, 4 }, \
	{ 0x00256948, 4 }, \
	{ 0x0025694c, 4 }, \
	{ 0x00256a74, 8 }, \
	{ 0x00256a84, 8 }, \
	{ 0x00256a6c, 8 }, \
	{ 0x00256a64, 8 }, \
	{ 0x00256a7c, 8 }, \
	{ 0x00256a5c, 8 }, \
	}

#define GUEST_MUTEX_LOCK 0x00102bbb
#define GUEST_VANISH 0x0010260c
#define GUEST_VANISH_END 0x1028ac
#define GUEST_MUTEX_LOCK_MUTEX_ARGNUM 1
#define GUEST_MUTEX_IGNORES { \
	{ 0x00256960, 8 }, \
	{ 0x002569d4, 8 }, \
	{ 0x002569dc, 8 }, \
	{ 0x00256920, 40 }, \
	{ 0x00256480, 40 }, \
	{ 0x002564c0, 40 }, \
	}

#endif
