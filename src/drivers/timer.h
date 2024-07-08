#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIMER_FREQUENCY 1193180

typedef void (*timer_callback_t)(void);

void timer_init(uint32_t frequency);
void timer_set_frequency(uint32_t frequency);
uint64_t timer_ticks(void);
uint64_t timer_seconds(void);
void timer_sleep(uint32_t milliseconds);
void timer_set_callback(timer_callback_t callback);
void timer_enable_interrupts(void);
void timer_disable_interrupts(void);

#endif
