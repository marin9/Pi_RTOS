#pragma once

#define SEM_BIN		0
#define SEM_COUNT	1


void sem_init();
int	sem_create(int val, int type);
int	sem_delete(int sid);
int	sem_wait(int sid);
int	sem_trywait(int sid);
int	sem_post(int sid);