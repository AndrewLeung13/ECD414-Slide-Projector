/******************************************************
Andrew Leung
EECE 558 - Embedded Systems - Professor Summerville

File name: Systick.h
Description: Header file for watchdog.c
******************************************************/
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdint.h>

void configure_watchdog(uint32_t reload);
void feed_the_watchdog();

#endif
