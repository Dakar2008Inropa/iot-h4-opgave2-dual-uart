#ifndef TIMER1_H
#define TIMER1_H

#include <stdint.h>

void timer1_start_ms(uint16_t ms, void (*on_tick)(void));

#endif
