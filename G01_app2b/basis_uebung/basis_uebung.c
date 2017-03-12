// Implementiert von Christian Korte, Hassen Zakraoui und Simon Janzon, 10.01.2017

// Aufgabe 2, Teilaufgabe B

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include "rsbs_xplained.h"

// X-Koordinate auf dem LCD-Bildschirm
int x = 2;

// Y-Koordinate auf dem LCD-Bildschirm
int y = 0;

// Hilfsvariable, in dem ein Wert aus einem Array gespeichert wird
int number = 0;

// Bewegt den Cursor nach rechts (mit sw2)
volatile int counter1 = 0;

// Beweget den Cursor nach oben/ unten (mit sw3)
volatile int counter2 = 0;

// Aendert den jeweiligen Wert (mit sw4)
volatile int counter3 = 0;

// Array 1 auf dem LCD-Bildschirm ausgeben
void printArray1(int array1[]) {
	for (int i = 0; i < 8; i++) {
		st7036_goto(0, 2 + i);
		st7036_putc(array1[i] + '0');
	}
}

// Array 2 auf dem LCD-Bildschirm ausgeben
void printArray2(int array2[]) {
	for (int i = 0; i < 8; i++) {
		st7036_goto(1, 2 + i);
		st7036_putc(array2[i] + '0');
	}
}

// Ergebnis-Array auf dem LCD-Bildschirm ausgeben
void printErgebnisArray(int tmp[]) {
	for (int i = 0; i < 9; i++) {
		st7036_goto(2, 1 + i);
		st7036_putc(tmp[i] + '0');
	}
}

// Das Ergebnis-Array berechnen
void calculate(int array1[], int array2[], int tmp[]) {
	int ergebnis[9];
	int uebertrag = 0;
	int i = 8;
	while (i > 0) {
		ergebnis[i] = array1[i - 1] + array2[i - 1] + uebertrag;
		uebertrag = 0;
		if (ergebnis[i] >= 2) {
			ergebnis[i] -= 2;
			uebertrag = 1;
		}
		i--;
	}
	ergebnis[0] = uebertrag;
	for (int i = 0; i < 9; i++) {
		tmp[i] = ergebnis[i];
	}
}

// Die Zeichen "+" und "=" an die jeweilige Stelle auf dem LCD-Bildschirm ausgeben
void initialize(void) {
	st7036_goto(1, 1);
	st7036_putc('+');

	st7036_goto(2, 0);
	st7036_putc('=');
}

// Das Ergebnis-Array berechnen und alle Arrays auf dem LCD-Bildschirm ausgeben
void update(int array1[], int array2[], int tmp[]) {
	calculate(array1, array2, tmp);
	printArray1(array1);
	printArray2(array2);
	printErgebnisArray(tmp);
}

int main(void) {
	xplained_init();

	// Deklaration der Arrays
	int array1[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int array2[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int tmp[9];

	// Hilfsvaraiablen fuer das Blinken
	int i = 0;
	int j = 0;

	initialize();
	update(array1, array2, tmp);

	while (1) {
		// Blinken
		while (j < 3) {
			j++;
			st7036_goto(y, x);
			st7036_putc((i == 0 ? (number + '0') : ' '));
			i = (i + 1) % 2;
			_delay_ms(300);
		}
		j = 0;

		update(array1, array2, tmp);

		//Nummer berechnen
		if (y == 0) {
			number = array1[x - 2];
		} else {
			number = array2[x - 2];
		}

		// Cursor nach rechts bewegen
		if (counter1 != 0) {
			if (x < 9) {
				x++;
			} else {
				x = 2;
			}
			counter1 = 0;
		}

		// Cursor nach oben/ unten bewegen
		if (counter2 != 0) {
			if (y == 0) {
				y = 1;
			} else {
				y = 0;
			}
			counter2 = 0;
		}

		// Wert aendern
		if (y == 0) {
			number = array1[x - 2];
		} else {
			number = array2[x - 2];
		}
		if (counter3 != 0) {
			if (number == 1) {
				number = 0;
			} else {
				number = 1;
			}
			if (y == 0) {
				array1[x - 2] = number;
			} else {
				array2[x - 2] = number;
			}
			update(array1, array2, tmp);
			counter3 = 0;
		}
	}
	return 0;
}

// Code fuer PORT-D Interrupt 1
ISR(PORTD_INT1_vect) {
	// sw2
	if ((PORTD.IN & 0x04) == 0) {
		counter1++;

		// sw3
	} else if ((PORTD.IN & 0x08) == 0) {
		counter2++;

		// sw4
	} else if ((PORTD.IN & 0x10) == 0) {
		counter3++;
	}
}

// Code fuer PORT-C Timer/Counter 0 Overflow
ISR(TCC0_OVF_vect) {

}

// Code fuer PORT-C Timer/Counter 1 Overflow
ISR(TCC1_OVF_vect) {

}
