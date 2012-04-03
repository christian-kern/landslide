/**
 * @file kernel_specifics_pobbles_race.h
 * @brief #defines for the pobbles_race guest kernel (automatically generated)
 * @author Ben Blum <bblum@andrew.cmu.edu>
 */

#ifndef __LS_KERNEL_SPECIFICS_POBBLES_RACE_H
#define __LS_KERNEL_SPECIFICS_POBBLES_RACE_H

#include "x86.h"

#define GUEST_ESP0_ADDR (0x00100930 + 4)
#define GET_ESP0(cpu) READ_MEMORY(cpu, GUEST_ESP0_ADDR)

#define GUEST_TIMER_WRAP_ENTER     0x001035d0
#define GUEST_TIMER_WRAP_EXIT      0x1035d7
#define GUEST_CONTEXT_SWITCH_ENTER 0x00105a18
#define GUEST_CONTEXT_SWITCH_EXIT  0x105adc

#define GUEST_READLINE_WINDOW_ENTER 0x00106f92
#define GUEST_READLINE_WINDOW_EXIT 0x1071db

#define GUEST_LMM_ALLOC_ENTER      0x0010d208
#define GUEST_LMM_ALLOC_EXIT       0x10d4c3
#define GUEST_LMM_ALLOC_SIZE_ARGNUM 2
#define GUEST_LMM_ALLOC_GEN_ENTER  0x001099ec
#define GUEST_LMM_ALLOC_GEN_EXIT   0x109df3
#define GUEST_LMM_ALLOC_GEN_SIZE_ARGNUM 2
#define GUEST_LMM_FREE_ENTER       0x0010959c
#define GUEST_LMM_FREE_EXIT        0x109892
#define GUEST_LMM_FREE_BASE_ARGNUM 2
#define GUEST_LMM_FREE_SIZE_ARGNUM 3
#define GUEST_LMM_REMOVE_FREE_ENTER 0x001098a4
#define GUEST_LMM_REMOVE_FREE_EXIT 0x1099e9

#define GUEST_IMG_END 0x0015757c
#define GUEST_DATA_START 0x00134140
#define GUEST_DATA_END 0x00135200
#define GUEST_BSS_START 0x00135200
#define GUEST_BSS_END GUEST_IMG_END
#define GUEST_PANIC 0x0010b438
#define GUEST_KERNEL_MAIN 0x00101158

#define TELL_LANDSLIDE_DECIDE 0x00108598
#define TELL_LANDSLIDE_THREAD_SWITCH 0x0010859d
#define TELL_LANDSLIDE_SCHED_INIT_DONE 0x001085a2
#define TELL_LANDSLIDE_FORKING 0x001085a7
#define TELL_LANDSLIDE_VANISHING 0x001085ac
#define TELL_LANDSLIDE_SLEEPING 0x001085b1
#define TELL_LANDSLIDE_THREAD_RUNNABLE 0x001085b6
#define TELL_LANDSLIDE_THREAD_DESCHEDULING 0x001085bb
#define TELL_LANDSLIDE_MUTEX_LOCKING 0x001085c0
#define TELL_LANDSLIDE_MUTEX_BLOCKING 0x001085c5
#define TELL_LANDSLIDE_MUTEX_LOCKING_DONE 0x001085ca
#define TELL_LANDSLIDE_MUTEX_UNLOCKING 0x001085cf
#define TELL_LANDSLIDE_MUTEX_UNLOCKING_DONE 0x001085d4

#define GUEST_SCHEDULER_FUNCTIONS { \
	{ 0x00105655, 0x1056a7 },\
	{ 0x001056a8, 0x105736 },\
	{ 0x00105737, 0x10591c },\
	{ 0x0010591d, 0x10594a },\
	{ 0x0010594b, 0x105965 },\
	{ 0x00105966, 0x105982 },\
	{ 0x00105983, 0x1059a7 },\
	{ 0x001059a8, 0x105a17 },\
	{ 0x00105a18, 0x105adc },\
	{ 0x00106564, 0x106582 },\
	{ 0x00103522, 0x1035cc }, }
#define GUEST_SCHEDULER_GLOBALS { \
	{ 0x00137508, 4 }, \
	{ 0x00157548, 4 }, \
	{ 0x00135244, 4 }, \
	{ 0x00136434, 16 }, \
	{ 0x00137520, 16 }, \
	{ 0x0015754c, 16 }, \
	{ 0x0015755c, 8 }, \
	{ 0x00136428, 8 }, \
	{ 0x00157540, 8 },  }

#define GUEST_INIT_TID 1
#define GUEST_SHELL_TID 2
#define GUEST_FIRST_TID 1
#ifdef GUEST_IDLE_TID
#undef GUEST_IDLE_TID
#endif

#define GUEST_SCHEDULER_LOCK 0x00137508
#define GUEST_WITHIN_FUNCTIONS { \
	{ 0x00104203, 0x104583 }, }
#define BUG_ON_THREADS_WEDGED 0
#define EXPLORE_BACKWARDS 1

#endif
