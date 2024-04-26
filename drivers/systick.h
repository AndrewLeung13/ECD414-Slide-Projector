/******************************************************
Andrew Leung
EECE 558 - Embedded Systems - Professor Summerville

File name: systick.h
Description: Header file for systick.c
******************************************************/
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdbool.h>

void configure_systick();
_Bool systick_has_fired();
_Bool systick_register_callback(void(*p_fn)());

#endif
