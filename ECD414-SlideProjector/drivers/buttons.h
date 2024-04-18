#ifndef PB0_H
#define PB0_H

#include<stdint.h>

void configure_buttons();
uint8_t read_buttons();
uint8_t debounce_buttons();
_Bool read_b12();

#endif
