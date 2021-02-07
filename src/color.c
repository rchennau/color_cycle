#include <stdlib.h>
// #include <unistd.h>
#include <conio.h>
#include <peekpoke.h>
#include <atari.h>
#include <string.h>

extern const char text[]; // Defined in text.s file 
typedef unsigned char BYTE; // convience alias 

#pragma static-locals (1)  // Declare local variables as static and add them to the zero-page register of the start system

#define SAVMSC *(unsigned int *) 88 // default screen address for the Atari

void __fastcall__ str_to_internal(char *s);  // convert string in memory to internal character format Atasci

void graphics2();

unsigned char * display_list();
void custom_screen(char *);

int main (void) {
	BYTE y;	
	BYTE x;	
	BYTE *dlist;
	BYTE *color;
	BYTE *hue;
	BYTE *luminance;
	BYTE *keypress;
	BYTE *keycode;
	BYTE COLOR_REG[12] = { COLOR_BLACK, COLOR_WHITE, COLOR_RED, COLOR_CYAN, COLOR_VIOLET, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_ORANGE, COLOR_BROWN, COLOR_LIGHTRED, COLOR_LIGHTRED };
	BYTE current_color_reg;

	// x = display_list();
	dlist = display_list();
	// POKE(dlist+3,71);
	// POKE(dlist+6,7);
	// POKE(710,207);
	// POKE(712,207);
	// POKE(711,207);
	_graphics(0);
	clrscr();
	color = POKE(710,0);
	current_color_reg = 0;
	// printf("Press up/down to change Hue\nPress left/right to change luminance\n");
	cputs("      Press up/down to change Hue       ");
	cputs("  Press left/right to change luminance  ");
	// cprintf("The current color: %i, %i, %i\n", PEEK(710), PEEK(711), PEEK(712));
	/*******************************************************************
	 * Test for up, down, left and right and set the color accordingly *
	 * 
	 */
	/*
	while (1) {
		cprintf("%c",cgetc());
	}
*/
	OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0;
	gotoxy(20,11);
	cprintf("%c",0x1C);
	gotoxy(20,13);
	cprintf("%c",0x1D);	
	gotoxy(19,12);
	cprintf("%c",0x1E);	
	gotoxy(21,12);
	cprintf("%c",0x1F);

	while (1) {
		*keypress = cgetc();
		gotoxy(0,14);
		if ((*keypress) == 28) {
			color=color+2;	
			POKE(710,color);
			OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
			gotoxy(20,11);
			cprintf("%c",0x9C);
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}	
			gotoxy(20,11);
			cprintf("%c",0x1C);
		} 
		if ((*keypress) == 29) {
			color=color-2;
			POKE(710,color);
			gotoxy(20,13);
			cprintf("%c",0x9D);	
			OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}	
			gotoxy(20,13);
			cprintf("%c",0x1D);

		}
		if ((*keypress) == 30) {   // change color hue one register up
			if (current_color_reg != 12) 
				current_color_reg = current_color_reg+1;
		
			if (current_color_reg >= 0 && current_color_reg <= 12) {
				POKE(710,COLOR_REG[current_color_reg]);
				color=PEEK(710);
			}
			gotoxy(19,12);
			cprintf("%c",0x9E);
			OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}	
			gotoxy(19,12);
			cprintf("%c",0x1E);
		} 
		if ((*keypress) == 31) {
			if (current_color_reg != 0) 
				current_color_reg = current_color_reg-1;
			
			if (current_color_reg >= 0 && current_color_reg <= 12) {
				POKE(710,COLOR_REG[current_color_reg]);
				color=PEEK(710);
			}
			gotoxy(21,12);
			cprintf("%c",0x9F);
			OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}	
			gotoxy(21,12);
			cprintf("%c",0x1F);
		} 
	}
	/*
	for (y = 0; y <= 31; y++) {
	 	printf("Display List %i\t", dlist[y]);	
	}
	*/
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
	cprintf("Hello World\n");
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
		// sleep(.75);
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

		cprintf("%s\n", text);		// Print the same line to the second print area of the screen 
		cprintf("%s and length is:%i\n", *screen, strlen(text));		// Print the same line to the second print area of the screen 

		POKEW(710,i);
		// sleep(2);
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
