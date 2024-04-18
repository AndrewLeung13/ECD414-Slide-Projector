#include "usbcdc.h"
#include "uclib.h"
#include "spi1.h"
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

int main()
{
	char c;
	configure_spi1();
	configure_usbcdc();
	while(1)
	{
		if( usbcdc_getchar(&c))
		{
			switch(c){
			case 'w':  // configure outputs
				usbcdc_putchar(':');
				spi1->sspdr=0x40;
				spi1->sspdr=0x00; // channel A, 10 for channel B
				spi1->sspdr=0x00; // write to channel B, write 12
				spi1->sspdr=0x00;
				break;
			case 't':
				spi1->sspdr=0x40;
				spi1->sspdr=0x12;
				spi1->sspdr=0xFF;
				spi1->sspdr=0xFF;
				break;
			break;
			case 'd':
				spi1_deselect();
				break;
			case 's':
				spi1_select();
				break;
			case '1': // Turn on all channel A
				spi1->sspdr=0x40;
				spi1->sspdr=0x12;
				spi1->sspdr=0xFF;
				break;
			case '2': // Turn off all channel A
				spi1->sspdr=0x40;
				spi1->sspdr=0x12;
				spi1->sspdr=0x00;
				break;
			case '3': // Turn on all channel B
				spi1->sspdr=0x40;
				spi1->sspdr=0x13;
				spi1->sspdr=0xFF;
				break;
			case '4': // Turn off all channel B
				spi1->sspdr=0x40;
				spi1->sspdr=0x13;
				spi1->sspdr=0x00;
				break;
			case '5': // Turn on all on
				s0_proj_high(7);
			break;
			case '6': // Turn on all off
				all_s0_proj_low();
			break;
			case '7': // self initialize

				spi1_select();
				while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

				spi1->sspdr=0x40;
				spi1->sspdr=0x00; // channel A, 10 for channel B
				spi1->sspdr=0x00; // write to channel B, write 12
				spi1->sspdr=0x00;

				spi1_deselect();
				while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

			break;
			default:
				break;
			}
		}
	}
	return 0;
}


void s0_proj_high(uint8_t projector){

	spi1_select();

	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

	spi1->sspdr=0x40;
	spi1->sspdr=0x12;

	if(projector >= 7){ //Projector is on channel A (0-7)
		spi1->sspdr= (1<<projector) ;
		spi1->sspdr=0x00;
	}
	else{
		spi1->sspdr=0x00; //Projector is on channel B (7-0)
		spi1->sspdr= (1<< (projector - 8) );
	}

	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

	spi1_deselect();

	while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

}

void all_s0_proj_low(){

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
