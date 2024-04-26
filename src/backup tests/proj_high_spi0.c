#include "usbcdc.h"
#include "uclib.h"
#include "spi0.h"
#include "rp2040/spi.h"

// MCP23017

// in minicom, input
// SRDPPP
// SWDPPP
// SRDPPP

// after this, there should be values printed from the registers

// ddr to outputs and make it data registers
// spi write to the data register
// two bytes to track which side

void s0_proj_high(uint8_t projector);
void all_s0_proj_low();

int main(){
	char c;
	configure_spi0();
	configure_usbcdc();
	while(1){

		spi0_select();
		while(spi0->sspsr & SPI1_SSPSR_BSY_MASK){}
		spi0->sspdr=0x40;
		spi0->sspdr=0x12;
		spi0->sspdr=0xFF;
		spi0->sspdr=0xFF;

		spi0_deselect();
		while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

	}
}


void s0_proj_high(uint8_t projector){

	spi0_select();

	while(spi0->sspsr & SPI0_SSPSR_BSY_MASK){}

	spi0->sspdr=0x40;
	spi0->sspdr=0x12;

	if(projector >= 7){ //Projector is on channel A (0-7)
		spi0->sspdr= (1<<projector) ;
		spi0->sspdr=0x00;
	}
	else{
		spi0->sspdr=0x00; //Projector is on channel B (7-0)
		spi0->sspdr= (1<< (projector - 8) );
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
