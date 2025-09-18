#ifndef __LED_H__

#define __LED_H__

#include <stdbool.h>

void led_init();

char *led_current_state();

int led_turn_on();

int led_turn_off();

bool led_is_on();

#endif