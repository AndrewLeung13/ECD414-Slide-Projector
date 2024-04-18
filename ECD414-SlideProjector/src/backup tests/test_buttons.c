#include "buttons.h"
#include "watchdog.h"
#include "systick.h"
#include "usbcdc.h"
#include "uclib.h"
#include "interrupt.h"
#include <stdint.h>
#include <stdbool.h>


void main_fsm();
int32_t counter = 0;
uint8_t btn = 0;

int main(){
  disable_irq();

  systick_register_callback(main_fsm);
  configure_systick();
  configure_watchdog(1500);
  configure_buttons();
  configure_usbcdc();

  enable_irq();

  while(1){
    asm("WFI");
    if (!systick_has_fired())
        continue;

    feed_the_watchdog();
  }
  return 0;
}

void main_fsm(){

  if(debounce_buttons() != 99){
    print_string("btn");
  }

}
