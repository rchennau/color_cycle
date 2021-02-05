#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <peekpoke.h>
#include <atari.h>
#include <string.h>
#include <tgi.h>

extern const char text[]; // Defined in text.s file 

#pragma static-locals (1)  // Declare local variables as static and add them to the zero-page register of the start system

#define SAVMSC *(unsigned int *) 88 // default screen address for the Atari

void __fastcall__ str_to_internal(char *s);  // convert string in memory to internal character format Atasci

void graphics2();

unsigned char * display_list();
void custom_screen(char *);

int main (void) {
	unsigned int y;	
	// static int *x;
	unsigned char *dlist;
	unsigned char *color;
	unsigned char *hue;
	unsigned char *luminance;
	unsigned char *keypress;
	unsigned char *keycode;
	// x = display_list();
	dlist = display_list();
	// POKE(dlist+3,71);
	// POKE(dlist+6,7);
	// POKE(710,207);
	// POKE(712,207);
	// POKE(711,207);
	_graphics(0);
	gotoxy(0,0);
	color = PEEK(710);
	printf("Press up/down to change Hue\nPress left/right to change luminance\n");
	printf("The current color: %i, %i, %i\n", PEEK(710), PEEK(711), PEEK(712));

	/*******************************************************************
	 * Test for up, down, left and right and set the color accordingly *
	 * 
	 */
	
	while (1) {
		*keypress = cgetc();
		gotoxy(0,14);
		if ((*keypress) == 28) {
			printf("Key pressed up : %i\t", *keypress);
			// color = *hue*16+1+*luminance;
			hue = *hue*16+1;
			printf("Peek = %i",PEEK(710));
		} 
		if ((*keypress) == 29) {
			printf("Key pressed down : %i\t", *keypress);
			// color = color - *hue*16-1+*luminance;
			hue = *hue*16-1;
			printf("Peek = %i",PEEK(710));
		} 
		if ((*keypress) == 30) {
			printf("Key pressed left : %i\t", *keypress);
			// color = color + *hue*16+*luminance+1;
			luminance++;
			printf("Peek = %i",PEEK(710));
		} 
		if ((*keypress) == 31) {
			printf("Key pressed right : %i\t", *keypress);
			luminance--;
			printf("Peek = %i",PEEK(710));
		} 
		color = *hue*16+*luminance;
		POKE(710,color);
		printf("Peek = %i",PEEK(710));
	}
	keypress = cgetc();
	printf("Keypressed : %i\n", *keycode);
	keycode = (*keypress/64)-(*keycode*64);
	printf("Keypressed : %i\n", *keycode);
	// if (keypress = )	
	printf("Set Hue:");
	cscanf("%i",hue);
	gotoxy(0,20);
	printf("\tHue set: %i", *hue);
	printf("\nSet Luminance:");
	cscanf("%i",luminance);
	printf("\tLuminance set:%i\n", *luminance);
	color = *hue*16+luminance;
	POKE(710,color);
	// POKE(711,hue);
	// POKE(712,luminance);
	
	printf("What is the current color: %i, %i, %i\n", PEEK(710), PEEK(711), PEEK(712));
	sleep(4); 
	/*
	for (y = 0; y <= 31; y++) {
	 	printf("Display List %i\t", dlist[y]);	
	}
	*/
	while(1);
	return EXIT_SUCCESS;
}
/*********************************************************************
 * custom_screen builds a new display list based on params			**
 * 
 */
void custom_screen(char *x) {

	unsigned char local = x;	
	unsigned int dlist = PEEK(560)+PEEK(561)*256;  // Assign display list instructions 
	// POKE(dlist, 66);
	// POKE(dlist+1, 64);
	// POKE(dlist+2, 156);
	POKE(dlist+3,71);   // set the graphics mode to 2 LMS
	POKE(dlist+6,7);	// set the graphics mode to 2 HMS
	// POKE(dlist+5,2);
	POKE(720,192);		// set color
	gotoxy(0,0);
	printf("Hello World\n");
	display_list();		// call display list function
}
/*********************************************************************
 * display_list returns the current instructions for display list. **
 * display list is the set of instructions that tell the Atari how  **
 * and what to draw to the screen .                                 **
 */
unsigned char * display_list() {
	unsigned char *dlist = PEEK(560)+PEEK(561)*256;
	unsigned char dlist2[31];  // The array of numbers for display list size is 31 with each member of the array containing one byte instruction 
	unsigned char x = 0;
	for (x = 0; x <=31; x++) {
		dlist2[x] = PEEK(dlist+x);
		sleep(.75);
	}

	return dlist;
}
void graphics2() {
	unsigned int i = 0;	
	char *screen;			// initalize a point to a memory address labled screen
	char txt[15];			// create a var char labled txt and hold 16 bytes

	_graphics(2);   		// change to gr.2
	screen = (char *) SAVMSC;	// asign the screen address to a var	
	
	for (i = 0; i <=255; i++) {

		memcpy(txt, text, strlen(text)+1);		// Confusing but to convert to internal screen format we need a non-constant copy of our 'constant' string	
		str_to_internal(txt);		// convert content of text.s to internal screen format (Atasci)	
		memcpy(&screen[20], txt, strlen(text));	// Copy the contexnt of text to the pointer to the memory address labled screen (copy into RAM). 

		printf("%s\n", text);		// Print the same line to the second print area of the screen 
		printf("%s and length is:%i\n", *screen, strlen(text));		// Print the same line to the second print area of the screen 

		POKEW(710,i);
		sleep(2);
	}
	return;
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
