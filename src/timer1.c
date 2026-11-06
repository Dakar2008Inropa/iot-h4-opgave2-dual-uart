#include "../include/ProjectDefines.h"
#include "../include/timer1.h"

static void (*on_tick)(void);

void timer1_start_ms(uint16_t ms, void (*callback)(void))
{
    uint32_t ticks;

    on_tick = callback;

    TCCR1A = 0;
    TCNT1 = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12);

    ticks = ((F_CPU / 256UL) * ms) / 1000UL;
    if (ticks > 0) {
        ticks--;
    }
    if (ticks > 65535UL) {
        ticks = 65535UL;
    }

    OCR1A = (uint16_t)ticks;
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    if (on_tick) {
        on_tick();
    }
}
