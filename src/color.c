#include <conio.h>
#include <peekpoke.h>
#include <atari.h>
#include <_atarios.h>

extern const char text[]; // Defined in text.s file 
typedef unsigned char BYTE; // convenience alias 

#pragma static-locals (1)  // Declare local variables as static and add them to the zero-page register of the start system

#define SAVMSC *(unsigned int *) 88 // memory screen address dec 88 or hex 0x58 for the Atari 

unsigned char * display_list();
void custom_screen(char *);

void draw_screen(BYTE *, BYTE * , BYTE );
void wait(BYTE);
int main (void) {
	BYTE *dlist;
	BYTE *color;
	BYTE *keypress;
	BYTE COLOR_REG[12] = { COLOR_BLACK, COLOR_WHITE, COLOR_RED, COLOR_CYAN, COLOR_VIOLET, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_ORANGE, COLOR_BROWN, COLOR_LIGHTRED, COLOR_LIGHTRED };
	BYTE current_color_reg;
	// BYTE character_code[4] = { 0x1C, 0x1D, 0x1E, 0x1F };
	extern const char text[80]; // Defined In text.s file.  
	// dlist = display_list();
	_graphics(0);
	// clrscr();
	color = POKE(710,0);
	current_color_reg = 0;
	cprintf("%s", text);



/**************************************************
* Place the arrows in the middle of the screen    *
*/
	draw_screen(20,11,0x1C);
	draw_screen(20,13,0x1D);
	draw_screen(19,12,0x1E);
	draw_screen(21,12,0x1F);

	while (1) {
		*keypress = cgetc();
		if ((*keypress) == 0x41) {
			gotoxy(20,12);
			cprintf("%c", 0x41);
			display_list();	
			OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}
			gotoxy(20,12);
			cputs(" ");	
		}
		if ((*keypress) == 28) {
			color=color+2;	
			POKE(710,color);
			draw_screen(20,11,0x9C);
			/*
			gotoxy(20,11);
			cprintf("%c",0x9C);
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}	
			*/
			wait(12);
			draw_screen(20,11,0x1C);
			// gotoxy(20,11);
			// cprintf("%c",0x1C);
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
			if (current_color_reg != 14) 
				current_color_reg = current_color_reg+1;
		
			if (current_color_reg >= 0 && current_color_reg <= 14) {
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
			
			if (current_color_reg >= 0 && current_color_reg <= 14) {
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
		else {
			gotoxy(20,12);
			cprintf((keypress));
			OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
			while (OS.rtclok[2]<10); { 
				// twiddle thumbs 
			}	
			gotoxy(20,12);
			cprintf("%c",(keypress)+100);

		} 
	}
	return 0;
}
/*********************************************************************
 * wait a specified amount of time in msseconds                     **
 */

void wait(BYTE t) {
	OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
	while (OS.rtclok[2]<t); { 
		// twiddle thumbs 
	}	
}
/*********************************************************************
 * build_screen and position text on the screen                     ** 
 * 
 */
void draw_screen(BYTE *x, BYTE *y, BYTE character_code) {
	gotoxy(x,y);
	cprintf("%c", character_code);
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
	// gotoxy(0,0);
	// cprintf("Hello World\n");
	display_list();		// call display list function
}

/*********************************************************************
 * display_list returns the current instructions for display list. **
 * display list is the set of instructions that tell the Atari how  **
 * and what to draw to the screen .                                 **
 */
unsigned char * display_list() {
	unsigned char *dlist = PEEK(560)+PEEK(561)*256;  // memory address of DLIST
	
	unsigned char dlist2[31];  // The array of numbers for display list size is 31 with each member of the array containing one byte instruction 
	unsigned char x = 0;
	gotoxy(0,18);
	for (x = 0; x <=31; x++) {
		cprintf("%i,", PEEK(dlist+x));
		OS.rtclok[0]=OS.rtclok[1]=OS.rtclok[2]=0; 
		while (OS.rtclok[2]<10); { 
			// twiddle thumbs 
		}	
	}

	return dlist;
}