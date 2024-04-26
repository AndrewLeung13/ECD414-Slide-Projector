#include "spi1.h"
#include "usbcdc.h"
#include "uclib.h"
#include <stdbool.h>
#include <stdint.h>
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include <rp2040/spi.h>
#include <rp2040/clocks.h>

#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define GPIO(X) CONCAT2(gpio,X)
#define GPIO_CTRL(X) CONCAT3(gpio,X,_ctrl)

#define DATA_BITS 8
#define MOTOROLA_CPHA 0
#define MOTOROLA_CPOL 0
#define SPI1_SCLK_LOC 10
#define SPI1_TXD_LOC 11
#define SPI1_RXD_LOC 12
#define SPI1_SSn_LOC 13

#define SPI1_RESETS ( RESETS_RESET_SPI1_MASK \
		| RESETS_RESET_IO_BANK0_MASK \
		| RESETS_RESET_PADS_BANK0_MASK )

void configure_spi1()
{
	clocks -> clk_peri_ctrl = 1|CLOCKS_CLK_PERI_CTRL_ENABLE_MASK;

	resets -> clr_reset = SPI1_RESETS;

	while( (resets->reset_done & SPI1_RESETS) != SPI1_RESETS);

	const uint32_t pads_bank0_gpio_out =
	PADS_BANK0_GPIO25_OD(0)
	| PADS_BANK0_GPIO25_IE(0)
	| PADS_BANK0_GPIO25_DRIVE(0)
	| PADS_BANK0_GPIO25_PUE(0)
	| PADS_BANK0_GPIO25_PDE(0)
	| PADS_BANK0_GPIO25_SCHMITT(0)
	| PADS_BANK0_GPIO25_SLEWFAST(0);

	const uint32_t pads_bank0_gpio_in =
	PADS_BANK0_GPIO25_OD(0)
	| PADS_BANK0_GPIO25_IE(1)
	| PADS_BANK0_GPIO25_DRIVE(0)
	| PADS_BANK0_GPIO25_PUE(0)
	| PADS_BANK0_GPIO25_PDE(0)
	| PADS_BANK0_GPIO25_SCHMITT(0)
	| PADS_BANK0_GPIO25_SLEWFAST(0);

	const uint32_t io_bank0_gpio_ctrl=
	IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
	IO_BANK0_GPIO25_CTRL_INOVER(0)  |
	IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
	IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
	IO_BANK0_GPIO25_CTRL_FUNCSEL(1);

	io_bank0 -> GPIO_CTRL( SPI1_SCLK_LOC ) = io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL( SPI1_TXD_LOC  ) = io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL( SPI1_RXD_LOC ) = io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL( SPI1_SSn_LOC  ) =
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(5);
	pads_bank0 -> GPIO( SPI1_SCLK_LOC ) = pads_bank0_gpio_out;
	pads_bank0 -> GPIO( SPI1_TXD_LOC  ) = pads_bank0_gpio_out;
	pads_bank0 -> GPIO( SPI1_SSn_LOC  ) = pads_bank0_gpio_out;
	pads_bank0 -> GPIO( SPI1_RXD_LOC  ) = pads_bank0_gpio_in;

	sio->gpio_oe_set = (
		(1<<SPI1_SCLK_LOC)
		|(1<<SPI1_TXD_LOC )
		|(1<<SPI1_SSn_LOC )
	);

	spi1 -> sspcr0 =
		 SPI1_SSPCR0_SCR(0)
		|SPI1_SSPCR0_SPH(MOTOROLA_CPHA)
		|SPI1_SSPCR0_SPO(MOTOROLA_CPOL)
		|SPI1_SSPCR0_FRF(0)
		|SPI1_SSPCR0_DSS(DATA_BITS-1);

	spi1 -> sspcpsr=100;

	spi1 -> sspcr1 =
		 SPI1_SSPCR1_SOD(0)
		|SPI1_SSPCR1_MS(0)
		|SPI1_SSPCR1_SSE(0)
		|SPI1_SSPCR1_LBM(0);

	spi1 -> set_sspcr1 = SPI1_SSPCR1_SSE(1);
	spi1_deselect();

	spi1_select();
	spi1->sspdr=0x40;
	spi1->sspdr=0x00;
	spi1->sspdr=0xc0;
	spi1->sspdr=0x00;
	spi1_deselect();

	spi1_select();
	spi1->sspdr=0x40;
	spi1->sspdr=0x0C;
	spi1->sspdr=0xC0;
	spi1_deselect();

}

void spi1_select(){
  sio -> gpio_out_clr = (1<<SPI1_SSn_LOC);
}

void spi1_deselect(){
	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}
  sio -> gpio_out_set = (1<<SPI1_SSn_LOC);
}

void s1_proj_high(uint8_t projector){
	spi1_select();
	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}
	spi1->sspdr=0x40;
	spi1->sspdr=0x12;
	if(projector <= 7){ //Projector is on channel B (0-7)
		spi1->sspdr=0x00;
		spi1->sspdr= (1<<projector);
	}
	else{ //Projector is on channel A (8-11)
		spi1->sspdr= (1<< (projector - 8) );
		spi1->sspdr=0x00;
	}
	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}
	spi1_deselect();
	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}
}

void all_s1_proj_low(){

	spi1_select();

	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

	spi1->sspdr=0x40;
	spi1->sspdr=0x12;
	spi1->sspdr=0x00;  // A off
	spi1->sspdr=0x00;  // B off

	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

	spi1_deselect();

	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

}

uint8_t read_btn_7_8(){

	spi1_select();
	for(uint8_t i = 0; i<=8; i++){spi1->sspdr;}
	spi1->sspdr=0x41;
	spi1->sspdr=0x12;
	spi1->sspdr=0x00;
	spi1_deselect();
	spi1->sspdr;
	spi1->sspdr;
	uint8_t read = spi1->sspdr;

	if((read & 0x40) == 0){
		return (uint8_t) 8;
	} if((read & 0x80) == 0){
		return (uint8_t) 7;
	} else{
		return (uint8_t) 99;
	}
}
