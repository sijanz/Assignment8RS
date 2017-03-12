// Implementiert von Christian Korte, Hassen Zakraoui und Simon Janzon, 09.01.2017

// Aufgabe 2, Teilaufgabe A

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include "rsbs_xplained.h"

// X-Koordinate auf dem LCD-Bildschirm
volatile int x = 0;

// Y-Koordinate auf dem LCD-Bildschirm
volatile int y = 0;

int main(void) {
	xplained_init();
	// Das Symbol "¥" auf dem LCD-Bildschirm ausgeben
	while (1) {
		st7036_goto(x, y);
		st7036_putc(230);
		_delay_ms(100);
	}
	return 0;
}

// Code fuer PORT-D Interrupt 1
ISR (PORTD_INT1_vect) {
	// sw2
	if ((PORTD.IN & 0x04) == 0) {
		if (y == 0) {
			// Rand erreicht
		} else {
			y--;
		}
		st7036_clear();

		// sw3
	} else if ((PORTD.IN & 0x08) == 0) {
		if (x == 2) {
			// Rand erreicht
		} else {
			x++;
		}
		st7036_clear();

		// sw4
	} else if ((PORTD.IN & 0x10) == 0) {
		if (x == 0) {
			// Rand erreicht
		} else {
			x--;
		}
		st7036_clear();

		// sw5
	} else if ((PORTD.IN & 0x20) == 0) {
		if (y == 15) {
			// Rand erreicht
		} else {
			y++;
		}
		st7036_clear();
	}
}

// Code fuer PORT-C Timer/Counter 0 Overflow
ISR (TCC0_OVF_vect) {

}

// Code fuer PORT-C Timer/Counter 1 Overflow
ISR (TCC1_OVF_vect) {

}
