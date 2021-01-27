#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <peekpoke.h>

extern const char text[]; // Defined in text.s file 

#pragma static-locals (1)  // Declare local variables as static and add them to the zero-page register of the start system

int i = 0; // initialize counter var for incrimenting through the colors

int main (void) {
// setup graphics

	bgcolor(COLOR_BLACK);
	POKEW(710,192);
	clrscr();

	for (i = 0; i <= 512; i++) {
		POKEW(710,i);
		printf("%s %d", text, i);
		sleep(1);
	}
	return EXIT_SUCCESS;
}
