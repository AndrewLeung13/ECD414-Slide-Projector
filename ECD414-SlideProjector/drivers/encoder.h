#ifndef ENCODER_H
#define ENCODER_H

#include <stdbool.h>
#include <stdint.h>

void configure_encoder();
_Bool read_pb();
_Bool read_channel_a();
_Bool read_channel_b();
uint8_t read_encoder();
void print_button(int32_t counter);

#endif
