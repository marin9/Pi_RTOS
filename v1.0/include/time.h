#pragma once

typedef struct time_t{
  unsigned int sec;
  unsigned int usec;
} time_t;


void time_init();
void time_set(time_t *newtime);
void time_get(time_t *current);

void time_add(time_t *ta, time_t *tb, time_t *res);
void time_sub(time_t *ta, time_t *tb, time_t *res);
int time_cmp(time_t *ta, time_t *tb);