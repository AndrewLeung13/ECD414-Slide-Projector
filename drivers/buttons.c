#include "buttons.h"
#include "spi1.h"
#include <stdint.h>
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>

void configure_buttons(){
    resets -> clr_reset =
	    RESETS_RESET_IO_BANK0_MASK |
	    RESETS_RESET_PADS_BANK0_MASK;

    while(!(resets -> reset_done & RESETS_RESET_IO_BANK0_MASK))
    	continue;

    while(!(resets -> reset_done & RESETS_RESET_PADS_BANK0_MASK))
    	continue;

    uint32_t const in_pads_bank0_gpio =
	   PADS_BANK0_GPIO25_OD(0) |
      PADS_BANK0_GPIO25_IE(1) |
	     PADS_BANK0_GPIO25_DRIVE(0) |
	      PADS_BANK0_GPIO25_PUE(1) |
	       PADS_BANK0_GPIO25_PDE(0) |
	        PADS_BANK0_GPIO25_SCHMITT(0) |
	         PADS_BANK0_GPIO25_SLEWFAST(0);

    uint32_t const in_io_bank0_gpio_ctrl =
	   IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
	    IO_BANK0_GPIO25_CTRL_INOVER(0) |
	     IO_BANK0_GPIO25_CTRL_OEOVER(0) |
	      IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
	       IO_BANK0_GPIO25_CTRL_FUNCSEL(5);

    pads_bank0 -> gpio0 = in_pads_bank0_gpio;
    pads_bank0 -> gpio1 = in_pads_bank0_gpio;
    pads_bank0 -> gpio2 = in_pads_bank0_gpio;
    pads_bank0 -> gpio6 = in_pads_bank0_gpio;
    pads_bank0 -> gpio7 = in_pads_bank0_gpio;
    pads_bank0 -> gpio8 = in_pads_bank0_gpio;
    pads_bank0 -> gpio9 = in_pads_bank0_gpio;
    pads_bank0 -> gpio13 = in_pads_bank0_gpio;
    pads_bank0 -> gpio17 = in_pads_bank0_gpio;
    pads_bank0 -> gpio20 = in_pads_bank0_gpio;
    pads_bank0 -> gpio21 = in_pads_bank0_gpio;
    pads_bank0 -> gpio27 = in_pads_bank0_gpio;
    pads_bank0 -> gpio28 = in_pads_bank0_gpio;
    io_bank0 -> gpio0_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio1_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio2_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio6_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio7_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio8_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio9_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio13_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio17_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio20_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio21_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio27_ctrl = in_io_bank0_gpio_ctrl;
    io_bank0 -> gpio28_ctrl = in_io_bank0_gpio_ctrl;
}

uint8_t read_buttons(){

  if(!(sio -> gpio_in & (1<<0))){
    return (uint8_t) 0;
  }
  else if(!(sio -> gpio_in & (1<<1))){
    return (uint8_t) 1;
  }
  else if(!(sio -> gpio_in & (1<<2))){
    return (uint8_t) 2;
  }
  else if(!(sio -> gpio_in & (1<<6))){
    return (uint8_t) 3;
  }
  else if(!(sio -> gpio_in & (1<<7))){
    return (uint8_t) 4;
  }

  else if(!(sio -> gpio_in & (1<<8))){
    return (uint8_t) 5;
  }

  else if(!(sio -> gpio_in & (1<<9))){
    return (uint8_t) 6;
  }

  else if(read_btn_7_8() == 7){
    return (uint8_t) 7;
  }

  else if(read_btn_7_8() == 8){
    return (uint8_t) 8;
  }

  else if(!(sio -> gpio_in & (1<<20))){
    return (uint8_t) 9;
  }
  else if(!(sio -> gpio_in & (1<<21))){
    return (uint8_t) 10;
  }
  else if(!(sio -> gpio_in & (1<<28))){
    return (uint8_t) 11;
  }
  else
    return (uint8_t) 99;
}

_Bool read_b12(){
  if(!(sio -> gpio_in & (1<<27))){
    return 1;
  }
  else
    return 0;
}

uint8_t debounce_buttons(){
  static enum {WAIT, DEBOUNCE, RELEASE} state = WAIT;
  static uint16_t db_count = 0;
  const uint8_t DB_TIME = 150;

  switch(state){
    case WAIT:
      if(read_buttons() != 99){
        db_count = 0;
        state = DEBOUNCE;
      }
      return 99;
    break;

    case DEBOUNCE:
      if(read_buttons() == 99){
        state = WAIT;
        return 99;
      }
      else if(++db_count >= DB_TIME){
        state = RELEASE;
        return read_buttons();
      }
      else{
        return 99;
      }
    break;

    case RELEASE:
      if(read_buttons() == 99){
        state = WAIT;
      }
      return 99;

    break;
  }
  return 99;
}
