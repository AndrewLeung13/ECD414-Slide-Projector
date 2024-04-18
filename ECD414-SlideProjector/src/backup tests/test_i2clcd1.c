#include "usbcdc.h"
#include "uclib.h"
#include "i2c1.h"
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

	configure_i2c1();
	configure_usbcdc();
	while(1)
	{
		if( usbcdc_getchar(&c))
		{
			uint16_t trans[16];
			switch(c)
			{

			case 'A':
					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c1_do_transaction(0x70,6,trans);
			break;

			case 'B':
					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c1_do_transaction(0x71,6,trans);
			break;

			case 'C':
					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c1_do_transaction(0x70,6,trans);

					for(volatile uint32_t i = 0; i<200000; i++);

					trans[0]=0x21|I2C_STOP;
					trans[1]=0xef|I2C_STOP;
					trans[2]=0x80|I2C_STOP;
					trans[3]=0x00|I2C_STOP;
					trans[4]=0x55|I2C_STOP;
					trans[5]=0x81|I2C_STOP;
					i2c1_do_transaction(0x71,6,trans);

			break;

			case '0':
				for(uint8_t i = 0; i<2; i++){
					printProjector1(i,0000);
				}
			break;
			case '1':
				printProjector1(0,1111);
				printProjector1(1,1111);
			break;
			case '2':
				printProjector1(0,2222);
				printProjector1(1,2222);
			break;
			case '3':
				printProjector1(0,3333);
				printProjector1(1,3333);
			break;
			case '4':
				printProjector1(0,4444);
				printProjector1(1,4444);
			break;
			case '5':
				printProjector1(0,5555);
				printProjector1(1,5555);
			break;
			case '6':
				printProjector1(0,6666);
				printProjector1(1,6666);
			break;
			case '7':
				printProjector1(0,7777);
				printProjector1(1,7777);
			break;
			case '8':
				printProjector1(0,8888);
				printProjector1(1,8888);
			break;
			case '9':
				printProjector1(0,9999);
				printProjector1(1,9999);
			break;
			}

		}
	}
	return 0;
}
