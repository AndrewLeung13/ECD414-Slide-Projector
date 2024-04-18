#ifndef SPI0_H
#define SPI0_H
#include <stdint.h>
void configure_spi0();
void spi0_select();
void spi0_deselect();
void s0_proj_high(uint8_t projector);
void all_s0_proj_low();
#endif
