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
			case 'w':  // configure outputs and buttons
				usbcdc_putchar(':');
				spi1->sspdr=0x40;
				spi1->sspdr=0x00; // channel A, 10 for channel B
				spi1->sspdr=0xc0; // c is data direction
				spi1->sspdr=0x00;
			break;
			case 'W':  // configure expander pullups on A7 and A6
				spi1->sspdr=0x40;
				spi1->sspdr=0x0C;
				spi1->sspdr=0xC0;
			break;

			case 'b':  // read channel A 6 and 7
				spi1->sspdr=0x41;
				spi1->sspdr=0x12;
				spi1->sspdr=0x00;
			break;

			case 'B':  //  output button reads
				print_hex(3,spi1->sspdr);
				print_string("\n\r");
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
			break;
			case '7': // self initialize

				spi1_select();
				spi1->sspdr=0x40;
				spi1->sspdr=0x00; // channel A, 10 for channel B
				spi1->sspdr=0xc0; // write to channel B, write 12
				spi1->sspdr=0x00;
				spi1_deselect();

				spi1_select();
				spi1->sspdr=0x40;
				spi1->sspdr=0x12;
				spi1->sspdr=0xFF;
				spi1->sspdr=0xFF;
				spi1_deselect();

			break;
			default:
				break;
			}


		}

	}
	return 0;
}
