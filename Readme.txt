Real time operating system for ARM
API:

stdio:
	Gets
	Printf

memory:
	Malloc
	Free

time:
	TimerSet
	TimerGet

task:
	TaskCreate
	TaskExit
	TaskSelf
	TaskCount
	TaskSleep
	TaskWakeup
	TaskStatus

sem:
	SemCreate
	SemDelete
	SemWait
	SemTryWait
	SemPost

lib:
	memcpy
	memset
	strlen
	atoi
	itoa

list.h
	list_init
	list_count
	list_addFirst
	list_addLast
	list_addSort
	list_removeFirst
	list_remove
	list_getFirst
	list_getById
