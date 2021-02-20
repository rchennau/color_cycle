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
	display_list();
	// _graphics(0);
	color = POKE(710,0);
	current_color_reg = 0;
	gotoxy(0,0);
	cprintf("%s", text);

/**************************************************
*      Draw arrows in the middle of the screen    *
*/
	draw_screen(20,11,0x1C);
	draw_screen(20,13,0x1D);
	draw_screen(19,12,0x1E);
	draw_screen(21,12,0x1F);

	while (1) {
		*keypress = cgetc();
		switch (*keypress) {
			case 0x41:  // print display list
				draw_screen(20,12,0x41);
				display_list();
				wait(10);
				draw_screen(20,12,0x12);
				break;
			case 0x1C:  // left arrow
				color=color+2;	
				POKE(710,color);
				draw_screen(20,11,0x9C);
				wait(10);
				draw_screen(20,11,0x1C);
				break;
			case 0x1D:  // right arrow
				color=color-2;
				POKE(710,color);
				draw_screen(20,13,0x9D);
				wait(10);
				draw_screen(20,13,0x1D);
				break;
			case 0x1E:  // up arrow 
				if (current_color_reg != 14) 
					current_color_reg = current_color_reg+1;
		
				if (current_color_reg >= 0 && current_color_reg <= 14) {
					POKE(710,COLOR_REG[current_color_reg]);
				// 	color=PEEK(710);
				draw_screen(19,12,0x9E);
				wait(10);
				draw_screen(19,12,0x1E);
				break;
			
			case 0x1F: // down arrow
				if (current_color_reg != 0) 
					current_color_reg = current_color_reg-1;
			
				if (current_color_reg >= 0 && current_color_reg <= 14) {
					POKE(710,COLOR_REG[current_color_reg]);
					color=PEEK(710);
				}	
				draw_screen(21,12,0x9F);
				wait(10);
				draw_screen(21,12,0x1F);
				break;
			case 0x58: // Exit
				clrscr();	
				return 0;

			default:
				draw_screen(20,12,*keypress);
				wait(10);
				draw_screen(20,12,0x20);	
			}
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
 * display_list returns the current instructions for display list. **
 * display list is the set of instructions that tell the Atari how  **
 * and what to draw to the screen .                                 **
 */
unsigned char * display_list() {
	unsigned char *dlist = PEEK(560)+PEEK(561)*256;  // memory address of DLIST
	BYTE display_list[36] = { 
    112, 112, 112, 66, 64, 156, 2, 2, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 65, 32,
    156
}; 
	// unsigned char dlist2[31];  // The array of numbers for display list size is 31 with each member of the array containing one byte instruction 
	unsigned char x = 0;
	// gotoxy(0,18);
	for (x = 0; x <=36; x++) {
		// cprintf("%i,", PEEK(dlist+x));
		POKE(dlist,display_list[x]);	
		wait(10);
	}

	return dlist;
}