#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <peekpoke.h>
#include <atari.h>
#include <string.h>

extern const char text[]; // Defined in text.s file 

#pragma static-locals (1)  // Declare local variables as static and add them to the zero-page register of the start system

#define SAVMSC *(unsigned int *) 88 // default screen address for the Atari

void __fastcall__ str_to_internal(char *s);  // convert string in memory to internal character format Atasci

int main (void) {
	unsigned int i = 0; // initialize counter var for incrimenting through the colors

	char *screen;			// initalize a point to a memory address labled screen
	char txt[15];			// create a var char labled txt and hold 16 bytes

	_graphics(2);   		// change to gr.2
	screen = (char *) SAVMSC;	// asign the screen address to a var
	
	bgcolor(COLOR_BLACK);		// set the background color to black
	POKEW(710,192);			// set the color to green for the font
	clrscr();			// clear the screen


	// strcpy(screen, text);		// copy the contents of text.s into memory address screen
	memcpy(txt, text, strlen(text)+1);		// Confusing but to convert to internal screen format we need a non-constant copy of our 'constant' string	
	str_to_internal(txt);		// convert content of text.s to internal screen format (Atasci)	
	memcpy(&screen[20], txt, strlen(text));	// Copy the contexnt of text to the pointer to the memory address labled screen (copy into RAM). 

	printf("%s\n", text);		// Print the same line to the second print area of the screen 
	// printf("%s and length is:%i\n", txt, strlen(txt));		// Print the same line to the second print area of the screen 
	printf("%s and length is:%i\n", *screen, strlen(text));		// Print the same line to the second print area of the screen 

	// Setup color section
//		for (i = 0; i <= 256; i++) {
//			POKEW(710,i);
//		}
	// sleep(30);
	while(1);
	return EXIT_SUCCESS;
}

/***************************************
*                    
* Converts a string from atascii code to internal character set code.
* Usually we want to do this before writing it to a bitmap 
*/
void __fastcall__ str_to_internal(char *s) {
	unsigned int i;
	unsigned int len;

	len = strlen(s);
	for (i = 0; i < len; i++) {
		if (s[i] < 32)
			s[i] += 64;
		else if (s[i] < 96)
			s[i] -= 32;
	}
	return;
}
