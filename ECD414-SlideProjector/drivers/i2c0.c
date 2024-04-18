#include "i2c0.h"
#include "uclib.h"
#include <stdbool.h>
#include <stdint.h>
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include <rp2040/i2c.h>
#include <rp2040/clocks.h>

#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define GPIO(X) CONCAT2(gpio,X)
#define GPIO_CTRL(X) CONCAT3(gpio,X,_ctrl)

const uint8_t sseg_values[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6f};
uint16_t trans[16];

#define I2C0_SDA_LOC 4
#define I2C0_SCL_LOC 5

#define I2C0_RESETS ( RESETS_RESET_I2C0_MASK \
		| RESETS_RESET_IO_BANK0_MASK \
		| RESETS_RESET_PADS_BANK0_MASK )

void configure_i2c0()
{
	resets -> clr_reset = I2C0_RESETS;

	while( (resets->reset_done & I2C0_RESETS) != I2C0_RESETS);

	pads_bank0 -> GPIO( I2C0_SCL_LOC ) =
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(1)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(1)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	pads_bank0 -> GPIO( I2C0_SDA_LOC  ) =
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(1)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(1)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	io_bank0 -> GPIO_CTRL( I2C0_SCL_LOC ) =
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(3);
	io_bank0 -> GPIO_CTRL( I2C0_SDA_LOC ) =
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(3);

	i2c0 -> ic_enable = 0;
	i2c0 -> ic_con =
		I2C0_IC_CON_IC_SLAVE_DISABLE(1) |
		I2C0_IC_CON_IC_RESTART_EN(1) |
		I2C0_IC_CON_IC_10BITADDR_MASTER(0) |
		I2C0_IC_CON_SPEED(2) |
		I2C0_IC_CON_MASTER_MODE(1) ;
	i2c0 -> ic_fs_scl_hcnt = 113;
	i2c0 -> ic_fs_scl_lcnt = 200;
	i2c0 -> ic_rx_tl = 15;
	i2c0 -> ic_tx_tl = 15;

	// Initialization
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

	for(volatile uint32_t i = 0; i<200000; i++);

	trans[0]=0x21|I2C_STOP;
	trans[1]=0xef|I2C_STOP;
	trans[2]=0x80|I2C_STOP;
	trans[3]=0x00|I2C_STOP;
	trans[4]=0x55|I2C_STOP;
	trans[5]=0x81|I2C_STOP;
	i2c0_do_transaction(0x72,6,trans);

	for(volatile uint32_t i = 0; i<200000; i++);

	trans[0]=0x21|I2C_STOP;
	trans[1]=0xef|I2C_STOP;
	trans[2]=0x80|I2C_STOP;
	trans[3]=0x00|I2C_STOP;
	trans[4]=0x55|I2C_STOP;
	trans[5]=0x81|I2C_STOP;
	i2c0_do_transaction(0x73,6,trans);

	for(volatile uint32_t i = 0; i<200000; i++);

	trans[0]=0x21|I2C_STOP;
	trans[1]=0xef|I2C_STOP;
	trans[2]=0x80|I2C_STOP;
	trans[3]=0x00|I2C_STOP;
	trans[4]=0x55|I2C_STOP;
	trans[5]=0x81|I2C_STOP;
	i2c0_do_transaction(0x74,6,trans);

	for(volatile uint32_t i = 0; i<200000; i++);

	trans[0]=0x21|I2C_STOP;
	trans[1]=0xef|I2C_STOP;
	trans[2]=0x80|I2C_STOP;
	trans[3]=0x00|I2C_STOP;
	trans[4]=0x55|I2C_STOP;
	trans[5]=0x81|I2C_STOP;
	i2c0_do_transaction(0x75,6,trans);

	for(volatile uint32_t i = 0; i<200000; i++);

	trans[0]=0x21|I2C_STOP;
	trans[1]=0xef|I2C_STOP;
	trans[2]=0x80|I2C_STOP;
	trans[3]=0x00|I2C_STOP;
	trans[4]=0x55|I2C_STOP;
	trans[5]=0x81|I2C_STOP;
	i2c0_do_transaction(0x76,6,trans);

	for(volatile uint32_t i = 0; i<200000; i++);

	printProjector0(0, 0);
	printProjector0(1, 0);
	printProjector0(2, 0);
	printProjector0(3, 0);
	printProjector0(4, 0);
	printProjector0(5, 0);
	printProjector0(6, 0);

}
_Bool i2c0_receive_fifo_not_empty()
{
	return i2c0 -> ic_status & I2C0_IC_STATUS_RFNE_MASK;
}
_Bool i2c0_transmit_fifo_not_full()
{
	return i2c0 -> ic_status & I2C0_IC_STATUS_TFNF_MASK;
}
_Bool i2c0_is_busy()
{
	return !(i2c0 -> ic_status & I2C0_IC_STATUS_TFE_MASK);
}
_Bool i2c0_do_transaction( uint8_t addr, uint8_t len, uint16_t *data_cmd )
{
	i2c0 -> ic_enable = 0;
	i2c0 -> ic_tar = addr;
	i2c0 -> ic_enable = 1;
	for( uint8_t i=0; i<len && i < 16; i++ )
		i2c0 -> ic_data_cmd = data_cmd[i];
	return true;
}
_Bool i2c0_get_data(uint16_t *data)
{
	if( i2c0_receive_fifo_not_empty() )
	{
		*data=i2c0->ic_data_cmd;
		return true;
	}
	return false;
}

void printProjector0(uint16_t projector, uint16_t slide){

	while(i2c0_is_busy()) {}

	uint16_t temp = slide;
	uint16_t fourth_num = mod10(temp);
	temp = div10(temp);

	uint16_t third_num = mod10(temp);
	temp = div10(temp);

	uint16_t second_num = mod10(temp);
	temp = div10(temp);

	uint16_t first_num = mod10(temp);

	trans[0]=0;  // location 0
	trans[1]=sseg_values[first_num]; // value 0
	trans[2]=0;
	trans[3]=sseg_values[second_num];
	trans[4]=0;
	trans[5]=0;
	trans[6]=0;
	trans[7]=sseg_values[third_num];
	trans[8]=0;
	trans[9]=sseg_values[fourth_num]|I2C_STOP;

	i2c0_do_transaction(0x70 + projector,10,trans);
}
