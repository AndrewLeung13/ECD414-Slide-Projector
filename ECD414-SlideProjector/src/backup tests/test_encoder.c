#include "encoder.h"
#include "watchdog.h"
#include "systick.h"
#include "usbcdc.h"
#include "uclib.h"
#include "interrupt.h"
#include <stdint.h>
#include <stdbool.h>

// LIBS="encoder.o watchdog.o sytick.o usbcdc.o uclib.o" make clean test_encoder.uf2
// make program
// gnome-terminal
// minicom -D /dev/ttyACM0

// Counter starts at 0
// turning the knob should increment/decrement the Counter
// pressing the encoder button should print the value of the counter

void main_fsm();
int32_t counter = 0;

int main(){
  disable_irq();

  systick_register_callback(main_fsm);
  configure_systick();
  configure_watchdog(1500);
  configure_encoder();
  configure_usbcdc();

  enable_irq();

  while(1){
    asm("WFI");
    if (!systick_has_fired())
        continue;

    if(read_pb()){
      print_button(counter);
    }

    feed_the_watchdog();
  }
  return 0;
}

void main_fsm(){
  static enum STATES {WAIT, B_HIGH, BOTH_HIGH, A_HIGH} state = WAIT;
  //Clockwise, increment, read encoder --> 01320
  //Counter-clockwise, decrement, read encoder --> 02310

  switch (state){
    case WAIT:
      if(read_encoder() == 1) // Clockwise
        state = B_HIGH;
      if(read_encoder() == 2) // Counter clockwise
        state = A_HIGH;

    break;

    case B_HIGH:
      if(read_encoder() == 3)
        state = BOTH_HIGH;
      if(read_encoder() == 0){
        state = WAIT;
        counter--;
      }

    break;

    case BOTH_HIGH:
      if(read_encoder() == 2)
        state = A_HIGH;
      if(read_encoder() == 1)
        state = B_HIGH;

    break;

    case A_HIGH:
      if(read_encoder() == 3)
        state = BOTH_HIGH;
      if(read_encoder() == 0){
        state = WAIT;
        counter++;
      }
    break;

  }
}
