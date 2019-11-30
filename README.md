# Mini RTOS

Mini RTOS is simple real time operating system with few basic functions.

**Task control functions:**

```c
void task_start(func fn, uint pr);
void task_sleep(uint ticks);
void task_exit();
```

**Task synchronization functions:**

```c
void sem_init(uint id, uint v);
void sem_take(uint id);
void sem_give(uint id);
uint sem_try(uint id);
```
