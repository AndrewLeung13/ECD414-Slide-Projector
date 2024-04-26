#ifndef BUTTONS_H
#define BUTTONS_H
#include <stdint.h>

void configure_buttons();
uint8_t read_buttons();
uint8_t debounce_buttons();
_Bool read_b12();

#endif
