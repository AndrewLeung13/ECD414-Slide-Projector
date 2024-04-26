#include "watchdog.h"
#include "systick.h"
#include "usbcdc.h"
#include "uclib.h"

#include "encoder.h"  // Frequency, button for fb toggle
#include "buttons.h"
#include "spi0.h"  // Backward, right
#include "spi1.h"  // Forward, 2 buttons, left
#include "i2c0.h"  // Displays 0-6
#include "i2c1.h"  // Displays 7-12

#include "interrupt.h"
#include <stdint.h>
#include <stdbool.h>
#include <rp2040/sio.h>

void poll_encoder();
_Bool poll_b12();
uint8_t poll_buttons();

void main_fsm();

uint8_t p0_slide = 0;
uint8_t p1_slide = 0;
uint8_t p2_slide = 0;
uint8_t p3_slide = 0;
uint8_t p4_slide = 0;
uint8_t p5_slide = 0;
uint8_t p6_slide = 0;
uint8_t p7_slide = 0;
uint8_t p8_slide = 0;
uint8_t p9_slide = 0;
uint8_t p10_slide = 0;
uint8_t p11_slide = 0;

int32_t encoder_count = 50;  // Default value of the encoder, + 50 ms
uint32_t clock_count = 0;  // Global clock

_Bool direction = 0; // 0 is forward

uint8_t button = 0;
uint8_t currentProjector = 0;

uint32_t loop_time = 1000/12;
uint16_t trans[16];  // I2c communication array

_Bool btn_tf = 0;  // Button pressed flag
_Bool btn_pressed_direction = 0;  // 0 is forward
uint8_t btn_pressed_num = 0;

int main(){
  configure_i2c1();  // Displays for Projectors 0-6
  configure_i2c0();  // Displays for Projectors 7-12

  disable_irq();

  systick_register_callback(main_fsm);
  configure_systick();  // Systick timer configuration
  configure_encoder();  // Encoder input setup
  configure_buttons();  // Button input setup
  configure_spi0();  // Communication to backward switching circuits
  configure_spi1();  // Communication to forward switching circuits

  configure_usbcdc();
  configure_watchdog(1500);  // Watchdog timer

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
  poll_encoder();
  poll_b12();
  poll_buttons();

  clock_count++;

  if(clock_count >= (loop_time + encoder_count) ){
    clock_count = 0;
    currentProjector++;

    if(currentProjector > 12){
      direction = !direction;
      currentProjector = 0;
      printProjector1(5, encoder_count);
    }

    if(direction == 0){ // forward
      if(btn_tf && btn_pressed_direction == 0 && btn_pressed_num == currentProjector){
        btn_tf = 0;
        all_s0_proj_low();
        all_s1_proj_low();
      }
      else{
        s1_proj_high(currentProjector);
        all_s0_proj_low();
      }
    }

    else{ // backward
      if(btn_tf && (btn_pressed_direction == 1 && (btn_pressed_num == currentProjector))){
        btn_tf = 0;
        all_s0_proj_low();
        all_s1_proj_low();
      }
      else{
        s0_proj_high(currentProjector);
        all_s1_proj_low();
      }
    }
  }
}

void poll_encoder(){
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
        if(encoder_count > 0)
          encoder_count--;
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
        if(encoder_count < 9999)
          encoder_count++; // increase display
      }
    break;

  }
}

_Bool poll_b12(){
  if(read_b12() == 1){
    return 1;
  }
  else{
    return 0;
  }
}

uint8_t poll_buttons(){
  // Returns projector pressed
  // Returns 99 for no press, 98 for fb toggle
  // Increases/decreases count
  uint8_t btn_pressed = debounce_buttons();

  if(btn_pressed == 99){
    return 99;
  }

  if(btn_pressed == 0 && btn_tf == 0){
    btn_tf = 1;
    btn_pressed_num = 0;

    if(poll_b12() == 0){
      btn_pressed_direction = 1;
      p0_slide++;
      printProjector0(0, p0_slide);
    } else if(poll_b12() == 1 && p0_slide > 0){
      btn_pressed_direction = 0;
      p0_slide--;
      printProjector0(0, p0_slide);
    }
    return 0;
  }

  if(btn_pressed == 1 && btn_tf == 0){
    btn_tf = 1;
    btn_pressed_num = 1;

    if(poll_b12() == 0){
      btn_pressed_direction = 1;
      p1_slide++;
      printProjector0(1, p1_slide);
    } else if(poll_b12() == 1 && p1_slide > 0){
      btn_pressed_direction = 0;
      p1_slide--;
      printProjector0(1, p1_slide);
    }
    return 1;
  }

  if(btn_pressed == 2 && btn_tf == 0){
    btn_tf = 1;
    btn_pressed_num = 2;

    if(poll_b12() == 0){
      btn_pressed_direction = 1;
      p2_slide++;
      printProjector0(2, p2_slide);
    } else if(poll_b12() == 1 && p2_slide > 0){
      btn_pressed_direction = 0;
      p2_slide--;
      printProjector0(2, p2_slide);
    }
    return 2;
  }

  if (btn_pressed == 3 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 3;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p3_slide++;
      printProjector0(3, p3_slide);
    } else if (poll_b12() == 1 && p3_slide > 0) {
      btn_pressed_direction = 0;
      p3_slide--;
      printProjector0(3, p3_slide);
    }
    return 3;
  }

  if (btn_pressed == 4 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 4;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p4_slide++;
      printProjector0(4, p4_slide);
    } else if (poll_b12() == 1 && p4_slide > 0) {
      btn_pressed_direction = 0;
      p4_slide--;
      printProjector0(4, p4_slide);
    }
    return 4;
  }

  if (btn_pressed == 5 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 5;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p5_slide++;
      printProjector0(5, p5_slide);
    } else if (poll_b12() == 1 && p5_slide > 0) {
      btn_pressed_direction = 0;
      p5_slide--;
      printProjector0(5, p5_slide);
    }
    return 5;
  }

  if (btn_pressed == 6 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 6;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p6_slide++;
      printProjector0(6, p6_slide);
    } else if (poll_b12() == 1 && p6_slide > 0) {
      btn_pressed_direction = 0;
      p6_slide--;
      printProjector0(6, p6_slide);
    }
    return 6;
  }

  if (btn_pressed == 7 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 7;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p7_slide++;
      printProjector1(0, p7_slide);
    } else if (poll_b12() == 1 && p7_slide > 0) {
      btn_pressed_direction = 0;
      p7_slide--;
      printProjector1(0, p7_slide);
    }
    return 7;
  }

  if (btn_pressed == 8 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 8;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p8_slide++;
      printProjector1(1, p8_slide);
    } else if (poll_b12() == 1 && p8_slide > 0) {
      btn_pressed_direction = 0;
      p8_slide--;
      printProjector1(1, p8_slide);
    }
    return 8;
  }

  if (btn_pressed == 9 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 9;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p9_slide++;
      printProjector1(2, p9_slide);
    } else if (poll_b12() == 1 && p9_slide > 0) {
      btn_pressed_direction = 0;
      p9_slide--;
      printProjector1(2, p9_slide);
    }
    return 9;
  }

  if (btn_pressed == 10 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 10;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p10_slide++;
      printProjector1(3, p10_slide);
    } else if (poll_b12() == 1 && p10_slide > 0) {
      btn_pressed_direction = 0;
      p10_slide--;
      printProjector1(3, p10_slide);
    }
    return 10;
  }

  if (btn_pressed == 11 && btn_tf == 0) {
    btn_tf = 1;
    btn_pressed_num = 11;

    if (poll_b12() == 0) {
      btn_pressed_direction = 1;
      p11_slide++;
      printProjector1(4, p11_slide);
    } else if (poll_b12() == 1 && p11_slide > 0) {
      btn_pressed_direction = 0;
      p11_slide--;
      printProjector1(4, p11_slide);
    }
    return 11;
  }

  else
    return (uint8_t) 99;
}
