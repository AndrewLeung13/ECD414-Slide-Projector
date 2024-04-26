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
			case '0':  // configure outputs and buttons
				s1_proj_high(1);
			break;
			case '1':  // configure outputs and buttons
				all_s1_proj_low();
			break;
			case '2':  // configure outputs and buttons
				spi1_select();

				while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

				spi1->sspdr=0x40;
				spi1->sspdr=0x12;
					spi1->sspdr= (0xFF);
					spi1->sspdr=0x00;


				while(spi1->sspsr & SPI1_SSPSR_BSY_MASK){}

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
