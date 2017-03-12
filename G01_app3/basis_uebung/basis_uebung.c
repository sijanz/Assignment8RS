// Implementiert von Christian Korte, Hassen Zakraoui und Simon Janzon, 09.01.2017

// Aufgabe 3

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>
#include "rsbs_xplained.h"

// Zeit in Sekunden
int timeInSeconds = 0;

// Anzahl Stunden
int hours = 0;

// Anzahl Minuten
int minutes = 0;

// Anzahl Sekunden
int seconds = 0;

// Gibt an, in welchem Modus sich das Programm befindet
volatile int modus = 1;

// Zaehlt die jeweiligen Stunden, Minuten und Sekunden hoch oder runter
volatile int counter = 0;

// Eine Nummer auf dem LCD-Bildschirm ausgeben
void printNumber(int x, int y, int number) {
	if (number < 10) {
		st7036_goto(x, y);
		st7036_putc('0');
		st7036_goto(x, y + 1);
		st7036_putc(number + '0');
	} else {
		st7036_goto(x, y);
		st7036_putc((number / 10) + '0');
		st7036_goto(x, y + 1);
		st7036_putc((number % 10) + '0');
	}
}

// Zeit auf dem LCD-Bildschirm ausgeben (Format: hh:mm:ss)
void printTime(int hours, int minutes, int seconds) {
	printNumber(0, 0, hours);
	st7036_goto(0, 2);
	st7036_putc(':');
	printNumber(0, 3, minutes);
	st7036_goto(0, 5);
	st7036_putc(':');
	printNumber(0, 6, seconds);
}

// Alarm ausloesen
void alarm(void) {
	printTime(0, 0, 0);
	for (int i = 1; i <= 25; i++) {
		PORTE.OUT = 0xff;
		_delay_ms(200);
		PORTE.OUT = 0x00;
		_delay_ms(200);
	}
	// Zum Neustart alle Zeiten auf 0 setzen
	hours = 0;
	minutes = 0;
	seconds = 0;
	printTime(hours, minutes, seconds);
}

// Den Timer starten
void startTimer(int timeInSeconds) {
	while (timeInSeconds > 0) {
		int hours = timeInSeconds / 3600;
		int minutes = (timeInSeconds - hours * 3600) / 60;
		int seconds = timeInSeconds - hours * 3600 - minutes * 60;
		printTime(hours, minutes, seconds);
		timeInSeconds--;
		_delay_ms(1000);
	}
	if (timeInSeconds == 0) {
		// Alarm ausloesen
		alarm();
		// Modus wieder auf "Stunden einstellen" setzen
		modus = 1;
	}
}

// Zeit in Sekunden umrechnen
int toTimeInSeconds(int hours, int minutes, int seconds) {
	int timeInSeconds = 3600 * hours + 60 * minutes + seconds;
	return timeInSeconds;
}

int main(void) {
	xplained_init();
	while (1) {
		int i = 0;
		int j = 0;
		printTime(hours, minutes, seconds);

		// Modus 1 (Stunden einstellen)
		while (modus == 1) {
			// Blinken
			while (j < 3) {
				j++;
				st7036_goto(0, 0);
				st7036_putc((i == 0 ? '0' : ' '));
				st7036_goto(0, 1);
				st7036_putc((i == 0 ? (hours + '0') : ' '));
				i = (i + 1) % 2;
				_delay_ms(300);
			}
			j = 0;

			if (counter > 0) {
				if (hours == 3) {
					hours = 0;
					counter = 0;
				} else {
					hours++;
					counter = 0;
				}
			} else if (counter < 0) {
				if (hours == 0) {
					hours = 3;
					counter = 0;
				} else {
					hours--;
					counter = 0;
				}
			}
			printTime(hours, minutes, seconds);
		}

		// Modus 2 (Minuten einstellen)
		while (modus == 2) {
			// Blinken
			while (j < 3) {
				j++;
				st7036_goto(0, 3);
				st7036_putc((i == 0 ? ((minutes / 10) + '0') : ' '));
				st7036_goto(0, 4);
				st7036_putc((i == 0 ? ((minutes % 10) + '0') : ' '));
				i = (i + 1) % 2;
				_delay_ms(300);
			}
			j = 0;

			if (counter > 0) {
				if (minutes == 59) {
					minutes = 0;
					counter = 0;
				} else {
					minutes++;
					counter = 0;
				}
			} else if (counter < 0) {
				if (minutes == 0) {
					minutes = 59;
					counter = 0;
				} else {
					minutes--;
					counter = 0;
				}
			}
			printTime(hours, minutes, seconds);
		}

		// Modus 3 (Sekunden einstellen)
		while (modus == 3) {
			// Blinken
			while (j < 3) {
				j++;
				st7036_goto(0, 6);
				st7036_putc((i == 0 ? ((seconds / 10) + '0') : ' '));
				st7036_goto(0, 7);
				st7036_putc((i == 0 ? ((seconds % 10) + '0') : ' '));
				i = (i + 1) % 2;
				_delay_ms(300);
			}
			j = 0;

			if (counter > 0) {
				if (seconds == 59) {
					seconds = 0;
					counter = 0;
				} else {
					seconds++;
					counter = 0;
				}
			} else if (counter < 0) {
				if (seconds == 0) {
					seconds = 59;
					counter = 0;
				} else {
					seconds--;
					counter = 0;
				}
			}
			printTime(hours, minutes, seconds);
		}

		// Modus 0 (Timer starten)
		while (modus == 0) {
			startTimer(toTimeInSeconds(hours, minutes, seconds));
		}
	}
	return 0;
}

// Code fuer PORT-D Interrupt 1
ISR(PORTD_INT1_vect) {
	// sw2
	if ((PORTD.IN & 0x04) == 0) {
		// Inkrementiert den jeweiligen Wert fuer Stunden, Minuten und Sekunden
		counter++;

		// sw3
	} else if ((PORTD.IN & 0x08) == 0) {
		// Dekrementiert den jeweiligen Wert fuer Stunden, Minuten und Sekunden
		counter--;

		// sw4
	} else if ((PORTD.IN & 0x10) == 0) {
		// Schaltet die Modi durch
		if (modus == 3) {
			modus = 1;
		} else {
			modus++;
		}

		// sw5
	} else if ((PORTD.IN & 0x20) == 0) {
		// Schaltet in Modus 0 (Timer starten)
		modus = 0;
	}
}

// Code fuer PORT-C Timer/Counter 0 Overflow
ISR(TCC0_OVF_vect) {

}

// Code fuer PORT-C Timer/Counter 1 Overflow
ISR(TCC1_OVF_vect) {

}
