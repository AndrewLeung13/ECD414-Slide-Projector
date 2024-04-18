/******************************************************
Andrew Leung
EECE 558 - Embedded Systems - Professor Summerville

File name: uclib.h
Description: Header file for uclib.c
******************************************************/

#ifndef UCLIB_H
#define UCLIB_H

#include <stdint.h>

uint16_t rand();
void srand(uint32_t x);
void print_string(char const *str);
void print_hex(uint8_t num_digits, uint32_t value);
void hex_dump( uint32_t start, uint8_t bytes );

uint16_t div10( uint16_t x);
uint16_t mod10( uint16_t x);

#endif
