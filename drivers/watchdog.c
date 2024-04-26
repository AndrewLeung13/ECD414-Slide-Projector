/******************************************************
Andrew Leung
EECE 558 - Embedded Systems - Professor Summerville

File name: Watchdog.c
Description: Driver for the watchdog timer
configure_watchdog - Configures the watchdog timer
feed_the_watchdog - Resets the watchdog timer
******************************************************/
#include "watchdog.h"
#include <rp2040/watchdog.h>
#include <rp2040/psm.h>
#include <stdint.h>

static uint32_t reload_value;

void configure_watchdog(uint32_t reload){
  watchdog->ctrl &= ~(1<<30);

  psm->wdsel = 0x0000FFFC;

  watchdog->tick |= ((1<<9) | (0xC));

  reload_value = reload*2;
  feed_the_watchdog();

  watchdog->ctrl |= (1<<30);
}

void feed_the_watchdog(){
  watchdog->load = reload_value;
}
