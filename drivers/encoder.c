#include "encoder.h"
#include "usbcdc.h"
#include "uclib.h"
#include "systick.h"
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include <stdbool.h>
#include <stdint.h>

static void callback();
static void debounce_callback();
static _Bool Chan_A_Last;
static _Bool Chan_A_Current;
static _Bool Chan_B_Last;
static _Bool Chan_B_Current;
static _Bool Button_Last;
static _Bool Button_Current;

#define CHANNEL_A 1<<22
#define CHANNEL_B 1<<26
#define BUTTON 1<<27

static uint32_t debounce_counter;

void configure_encoder(){
    resets -> clr_reset =
	    RESETS_RESET_IO_BANK0_MASK |
	    RESETS_RESET_PADS_BANK0_MASK;

    while(!(resets -> reset_done & RESETS_RESET_IO_BANK0_MASK))
    	continue;

    while(!(resets -> reset_done & RESETS_RESET_PADS_BANK0_MASK))
    	continue;

    pads_bank0 -> gpio22 =
	   PADS_BANK0_GPIO22_OD(0) |
	    PADS_BANK0_GPIO22_IE(1) |
	     PADS_BANK0_GPIO22_DRIVE(1) |
	      PADS_BANK0_GPIO22_PUE(1) |
	       PADS_BANK0_GPIO22_PDE(0) |
	        PADS_BANK0_GPIO22_SCHMITT(0) |
	         PADS_BANK0_GPIO22_SLEWFAST(0);

    io_bank0 -> gpio22_ctrl =
	   IO_BANK0_GPIO22_CTRL_IRQOVER(0) |
	    IO_BANK0_GPIO22_CTRL_INOVER(0) |
	     IO_BANK0_GPIO22_CTRL_OEOVER(0) |
	      IO_BANK0_GPIO22_CTRL_OUTOVER(0) |
	       IO_BANK0_GPIO22_CTRL_FUNCSEL(5);

     pads_bank0 -> gpio26 =
   	   PADS_BANK0_GPIO26_OD(0) |
   	    PADS_BANK0_GPIO26_IE(1) |
   	     PADS_BANK0_GPIO26_DRIVE(1) |
   	      PADS_BANK0_GPIO26_PUE(1) |
   	       PADS_BANK0_GPIO26_PDE(0) |
   	        PADS_BANK0_GPIO26_SCHMITT(0) |
   	         PADS_BANK0_GPIO26_SLEWFAST(0);

     io_bank0 -> gpio26_ctrl =
   	   IO_BANK0_GPIO26_CTRL_IRQOVER(0) |
   	    IO_BANK0_GPIO26_CTRL_INOVER(0) |
   	     IO_BANK0_GPIO26_CTRL_OEOVER(0) |
   	      IO_BANK0_GPIO26_CTRL_OUTOVER(0) |
   	       IO_BANK0_GPIO26_CTRL_FUNCSEL(5);

     pads_bank0 -> gpio27 =
       PADS_BANK0_GPIO27_OD(0) |
        PADS_BANK0_GPIO27_IE(1) |
         PADS_BANK0_GPIO27_DRIVE(1) |
          PADS_BANK0_GPIO27_PUE(1) |
           PADS_BANK0_GPIO27_PDE(0) |
            PADS_BANK0_GPIO27_SCHMITT(0) |
             PADS_BANK0_GPIO27_SLEWFAST(0);

     io_bank0 -> gpio27_ctrl =
       IO_BANK0_GPIO27_CTRL_IRQOVER(0) |
        IO_BANK0_GPIO27_CTRL_INOVER(0) |
         IO_BANK0_GPIO27_CTRL_OEOVER(0) |
          IO_BANK0_GPIO27_CTRL_OUTOVER(0) |
           IO_BANK0_GPIO27_CTRL_FUNCSEL(5);

    systick_register_callback(callback);
    systick_register_callback(debounce_callback);

    Chan_A_Current = sio->gpio_in & (CHANNEL_A);
    Chan_A_Last = Chan_A_Current;

    Chan_B_Current = sio->gpio_in & (CHANNEL_B);
    Chan_B_Last = Chan_B_Current;

    Button_Current = sio->gpio_in & (BUTTON);
    Button_Last = Button_Current;
}

static void callback(){
  Chan_A_Current = sio->gpio_in & (CHANNEL_A);
  Chan_B_Current = sio->gpio_in & (CHANNEL_B);
  Button_Current = sio->gpio_in & (BUTTON);

  Chan_A_Last = Chan_A_Current;
  Chan_B_Last = Chan_B_Current;
  Button_Last = Button_Current;
}

static void debounce_callback(){
  debounce_counter++;
}

_Bool read_pb(){
  static uint32_t last_counter;
  if (debounce_counter - last_counter >= 175){
    last_counter = debounce_counter;
    return (Button_Last == Button_Current && Button_Current == 0);
  }
  return false;
}

uint8_t read_encoder(){
  if (((Chan_A_Last || Chan_A_Current) != 0) && ((Chan_B_Last || Chan_B_Current) != 0))
    return 0; // untouched
  else if (((Chan_A_Last || Chan_A_Current) == 0) && ((Chan_B_Last || Chan_B_Current) != 0))
    return 1; // clockwise
  else if (((Chan_A_Last || Chan_A_Current) != 0) && ((Chan_B_Last || Chan_B_Current) == 0))
    return 2; // counter clockwise
  else if (((Chan_A_Last || Chan_A_Current) == 0) && ((Chan_B_Last || Chan_B_Current) == 0))
    return 3; // mid-turn
  else return 0xF;
}
