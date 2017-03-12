// Implementiert von Christian Korte, Hassen Zakraoui und Simon Janzon, 09.01.2017

// Aufgabe 1, Teilaufgabe B

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include "rsbs_xplained.h"

// Anzahl der maximalen Iterationen (die mit 8 Bits dargestellt werden koennen)
int maxIterations = 255;

// Zaehler
int i = 0;

// Integer im Terminal ausgeben
void sendNumberToTerminal(int i) {
	char str[15];
	sprintf(str, "%d", i);
	fprintf(COMM_PC, str);
}

// Zeit nach Programmstart im Terminal ausgeben
void sendTimeToTerminal(long time) {
	char str[15];
	sprintf(str, "%ld", time);
	fprintf(COMM_PC, "(Zeit seit Programmstart: ");
	fprintf(COMM_PC, str);
	fprintf(COMM_PC, "ms) ");
}

int main(void) {
	xplained_init();

	// Varaiable zum Zaehlen der Zeit seit Programmstart
	long time = 0;

	while (1) {
		while (i <= maxIterations) {
			// Zaehler auf LEDs ausgeben
			PORTE.OUT = ~(i);
			sendNumberToTerminal(i);
			sendTimeToTerminal(time);
			i++;
			time = time + 1000;
			_delay_ms(1000);
		}
		// Wieder bei 0 anfangen, wenn 255 erreicht
		if (i > maxIterations) {
			i = 0;
			time = time + 1000;
		}
	}
	return 0;
}
