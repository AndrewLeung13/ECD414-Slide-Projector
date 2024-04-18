#ifndef SPI1_H
#define SPI1_H
#include <stdint.h>
void configure_spi1();
void spi1_select();
void spi1_deselect();
void s1_proj_high(uint8_t projector);
void all_s1_proj_low();
uint8_t read_btn_7_8();
#endif
