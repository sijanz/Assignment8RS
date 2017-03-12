// Implementiert von Christian Korte, Hassen Zakraoui und Simon Janzon, 09.01.2017

// Aufgabe 1, Teilaufgabe C

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include "rsbs_xplained.h"

// Anzahl der maximalen Iterationen (die mit 8 Bits dargestellt werden koennen)
int maxIterations = 255;

// Zaehler
volatile int i = 0;

// Integer im Terminal ausgeben
void sendNumberToTerminal(int i) {
	char str[15];
	sprintf(str, "%d", i);
	fprintf(COMM_PC, str);
}

int main(void) {
	xplained_init();
	while (1) {
		while (i <= maxIterations) {
			// Zaehler auf LEDs ausgeben
			PORTE.OUT = ~(i);
		}
		// Wieder bei 0 anfangen, wenn 255 erreicht
		if (i > maxIterations) {
			i = 0;
		}
	}
	return 0;
}

// Code fuer PORT-D Interrupt 1
ISR (PORTD_INT1_vect) {
	// Dekrementieren mit sw2
	if ((PORTD.IN & 0x4) == 0) {
		if (i == 0) {
			i = 255;
		} else {
			i--;
		}
		// Den derzeitigen Zaehler im Terminal ausgeben
		sendNumberToTerminal(i);
		fprintf(COMM_PC, " ");
	}
	// Inkrementieren mit sw3
	if ((PORTD.IN & 0x8) == 0) {
		if (i == 255) {
			i = 0;
		} else {
			i++;
		}
		// Den derzeitigen Zaehler im Terminal ausgeben
		sendNumberToTerminal(i);
		fprintf(COMM_PC, " ");
	}
}

// Code fuer PORT-C Timer/Counter 0 Overflow
ISR (TCC0_OVF_vect) {

}

// Code fuer PORT-C Timer/Counter 1 Overflow
ISR (TCC1_OVF_vect) {

}
