/*
 * xplained_init.c
 *
 *  Created on: 20.12.2011
 *      Author: Administrator
 */

#include "rsbs_xplained.h"

void xplained_init(void){
	board_init();
	mcu_wait_us(10);
	mcu_io_set(PIN_EXTERNAL, 1);
	mcu_enable_interrupt();
	mcu_wait_ms(700);	// Wenn man nicht lange genug wartet startet das Display nicht richtig

	st7036_init(ST7036_DISPLAY_3x16, PF_4, PF_3, MCU_SPI_NUM_F, SPI5Mbit);


	/* Der nachfolgende Code aktiviert die Interrupts von PORT-D (sw2-sw7) sowie der
	 * Opverflow-Signale von Timer/Counter 0 und 1 von Port-C.
	 */
	PORTD.INT1MASK |=
					0xFC;

	PORTD.PIN2CTRL =
			1<<7|//Enable slew rate control//
			0<<6|
			0x03<<3|//Pull up//
			0x0<<0;
	PORTD.PIN3CTRL =
				1<<7|//Enable slew rate control//
				0<<6|
				0x03<<3|//Pull up//
				0x0<<0;
	PORTD.PIN4CTRL =
				1<<7|//Enable slew rate control//
				0<<6|
				0x03<<3|//Pull up//
				0x0<<0;
	PORTD.PIN5CTRL =
				1<<7|//Enable slew rate control//
				0<<6|
				0x03<<3|//Pull up//
				0x0<<0;
	PORTD.PIN6CTRL =
				1<<7|//Enable slew rate control//
				0<<6|
				0x03<<3|//Pull up//
				0x0<<0;
	PORTD.PIN7CTRL =
				1<<7|//Enable slew rate control//
				0<<6|
				0x03<<3|//Pull up//
				0x0<<0;
	//TimerInterrupt 0
	TCC0.PER = 10000;
	TCC0.CTRLA = 0x07;
	TCC0.INTCTRLA = ( TCC0.INTCTRLA & ~TC0_OVFINTLVL_gm ) |
		              TC_OVFINTLVL_MED_gc;
	//TimerInterrupt 1
	TCC1.PER = 10000;
	TCC1.CTRLA = 0x07;
	TCC1.INTCTRLA = ( TCC0.INTCTRLA & ~TC0_OVFINTLVL_gm ) |
		              TC_OVFINTLVL_MED_gc;
}
