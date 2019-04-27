#pragma once

void timer_init();
void timer_set(unsigned int usec, void (*action)());
void timer_get(unsigned int *elsaped);
