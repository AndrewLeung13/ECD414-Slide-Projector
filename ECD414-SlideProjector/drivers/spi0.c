#include "spi0.h"
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

#define DATA_BITS 8 //must be 4-16
#define MOTOROLA_CPHA 0
#define MOTOROLA_CPOL 0
#define SPI0_SCLK_LOC 18 //blue 2
#define SPI0_TXD_LOC 19 //red 3
#define SPI0_RXD_LOC 16 //yellow 0
#define SPI0_SSn_LOC 17 //grey 1

#define SPI0_RESETS ( RESETS_RESET_SPI0_MASK \
		| RESETS_RESET_IO_BANK0_MASK \
		| RESETS_RESET_PADS_BANK0_MASK )

void configure_spi0()
{
	clocks -> clk_peri_ctrl = 1|CLOCKS_CLK_PERI_CTRL_ENABLE_MASK;

	resets -> clr_reset = SPI0_RESETS;

	while( (resets->reset_done & SPI0_RESETS) != SPI0_RESETS);

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

	io_bank0 -> GPIO_CTRL( SPI0_SCLK_LOC ) = io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL( SPI0_TXD_LOC  ) = io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL( SPI0_RXD_LOC ) = io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL( SPI0_SSn_LOC  ) =
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(5);
	pads_bank0 -> GPIO( SPI0_SCLK_LOC ) = pads_bank0_gpio_out;
	pads_bank0 -> GPIO( SPI0_TXD_LOC  ) = pads_bank0_gpio_out;
	pads_bank0 -> GPIO( SPI0_SSn_LOC  ) = pads_bank0_gpio_out;
	pads_bank0 -> GPIO( SPI0_RXD_LOC  ) = pads_bank0_gpio_in;

	sio->gpio_oe_set = (
		(1<<SPI0_SCLK_LOC)
		|(1<<SPI0_TXD_LOC )
		|(1<<SPI0_SSn_LOC )
	);

	spi0 -> sspcr0 =
		 SPI0_SSPCR0_SCR(0)
		|SPI0_SSPCR0_SPH(MOTOROLA_CPHA)
		|SPI0_SSPCR0_SPO(MOTOROLA_CPOL)
		|SPI0_SSPCR0_FRF(0)
		|SPI0_SSPCR0_DSS(DATA_BITS-1);

	spi0 -> sspcpsr=100;

	spi0 -> sspcr1 =
		 SPI0_SSPCR1_SOD(0)
		|SPI0_SSPCR1_MS(0)
		|SPI0_SSPCR1_SSE(0)
		|SPI0_SSPCR1_LBM(0);

	spi0 -> set_sspcr1 = SPI0_SSPCR1_SSE(1);
	spi0_deselect();

	spi0_select();
	spi0->sspdr=0x40;
	spi0->sspdr=0x00; // channel A, 10 for channel B
	spi0->sspdr=0x00; // write to channel B, write 12
	spi0->sspdr=0x00;
	spi0_deselect();

}

void spi0_select()
{
    sio -> gpio_out_clr = (1<<SPI0_SSn_LOC);
}
void spi0_deselect()
{
		while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}
    sio -> gpio_out_set = (1<<SPI0_SSn_LOC);
}

void s0_proj_high(uint8_t projector){

	spi0_select();

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

	spi0->sspdr=0x40;
	spi0->sspdr=0x12;

	if(projector <= 7){ //Projector is on channel B (0-7)
		spi0->sspdr=0x00;
		spi0->sspdr= (1<<projector);
	}
	else{ //Projector is on channel A (8-11)
		spi0->sspdr= (1<< (projector - 8) );
		spi0->sspdr=0x00; 
	}

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

	spi0_deselect();

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

}

void all_s0_proj_low(){

	spi0_select();

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

	spi0->sspdr=0x40;
	spi0->sspdr=0x12;
	spi0->sspdr=0x00;  // A off
	spi0->sspdr=0x00;  // B off

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

	spi0_deselect();

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

}
