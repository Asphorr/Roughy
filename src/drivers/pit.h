#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#define PIT_CHANNEL0_DATA 0x40
#define PIT_CHANNEL1_DATA 0x41
#define PIT_CHANNEL2_DATA 0x42
#define PIT_COMMAND 0x43

void pit_init(uint32_t frequency);
void pit_handler(void);
uint32_t pit_get_tick_count(void);

#endif // PIT_H
