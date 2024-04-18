#include "usbcdc.h"
#include "uclib.h"
#include "i2c0.h"
#include <stdbool.h>
#include <rp2040/i2c.h>

// a is seven segment val
// i is index of the seven segment displays

// seperate connection of each transaction

// s, setup 6 presses
// a, a aaddress
// +, increment a, address
// r, set i to 1
// <, left shift i
// space, unsure
// 0, 1, 2, 3
// p, prints ?

int main()
{
	char c;
	uint8_t sseg_values[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6f};

	configure_i2c0();
	configure_usbcdc();
	while(1)
	{
		if( usbcdc_getchar(&c))
		{
			uint16_t trans[16];
			switch(c)
			{

			case 'A':
					print_string("A\n\r");
					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c0_do_transaction(0x70,6,trans);
			break;

			case 'B':
				print_string("B\n\r");
					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c0_do_transaction(0x71,6,trans);
			break;

			case 'C':
					print_string("C\n\r");
					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c0_do_transaction(0x70,6,trans);

					for(volatile uint32_t i = 0; i<200000; i++);

					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c0_do_transaction(0x71,6,trans);

			break;

			case '0':
				for(uint8_t i = 0; i<2; i++){
					printProjector0(i,0000);
				}
			break;
			case '1':
			print_string("1\n\r");
				printProjector0(6,1111);
				printProjector0(1,1111);
			break;
			case '2':
			print_string("2\n\r");
				printProjector0(6,2222);
				printProjector0(1,2222);
			break;
			case '3':
				printProjector0(6,3333);
				printProjector0(1,3333);
			break;
			case '4':
				printProjector0(6,4444);
				printProjector0(1,4444);
			break;
			case '5':
				printProjector0(6,5555);
				printProjector0(1,5555);
			break;
			case '6':
				printProjector0(6,6666);
				printProjector0(1,6666);
			break;
			case '7':
				printProjector0(6,7777);
				printProjector0(1,7777);
			break;
			case '8':
				printProjector0(6,8888);
				printProjector0(1,8888);
			break;
			case '9':
				printProjector0(6,9999);
				printProjector0(1,9999);
			break;
			}

		}
	}
	return 0;
}
