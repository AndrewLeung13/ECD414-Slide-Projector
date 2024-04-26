/******************************************************
Andrew Leung
EECE 558 - Embedded Systems - Professor Summerville

File name: uclib.c
Description: Function definitions for assignment 2
print_string - prints string
print_hex - input value and prints hex
hex_dump - outputs hex dump for an inputted address for
  n amount of bits, calculated the start address for the
  line, end address and prints the hex dump
******************************************************/
#include <stdint.h>
#include "uclib.h"
#include "usbcdc.h"

const uint32_t ONE_TENTH = 0x199a;

static uint32_t random_number_state;
uint16_t rand() // LCG based on ANSI C
{
    random_number_state = (1103515245 * random_number_state + 12345)&0x7FFFFFFF;
    return random_number_state>>16;
}

void srand(uint32_t x){ //Set random number generator seed
    random_number_state = x;
}

void print_string(char const *str){
    while (*str && usbcdc_putchar(*str++))
        continue;
}

void print_hex(uint8_t num_digits, uint32_t value){
    int8_t i;

    if (num_digits > 8)
        num_digits = 8;

    for ( i = num_digits-1; i>=0; i--)
    {
        uint8_t hex_digit = (value >> 4*i)&0xF;
        usbcdc_putchar(hex_digit + ((hex_digit<10)?'0':'A'-10));
    }
}

void hex_dump( uint32_t start, uint8_t bytes ){
    uint32_t value;
    uint32_t start_Byte = start &(~(uint32_t)7);
    uint32_t end_Byte = start+bytes-1;

    while(start_Byte < end_Byte){
        print_string("\n\r0x");
        print_hex(8, start_Byte);
        print_string(" ");

        for (uint8_t half_Line = 0; half_Line<2; half_Line++){
            value = (*((uint32_t*) start_Byte));

            for (uint8_t digit_Iteration = 0; digit_Iteration<4; digit_Iteration++){
                print_string(" ");
                print_hex(2, value);
                value = value >> 8;
            }

            start_Byte += 4;
        }
    }

    if (bytes>0)
        print_string("\n\r");
}

uint16_t div10( uint16_t x){
    return ((x * ONE_TENTH)>>16);
}

uint16_t mod10( uint16_t x){
    return (x - (div10(x) * 10));
}
