#
# Default makefile for Atari
#

# Run 'make SYS=<target>'; or, set a SYS env.
# var. to build another target system
SYS ?= atari

# Standard define for single space character
SPACE :=
SPACE +=

CC65=/usr/local/share/cc65
CC65_BIN=$(CC65)../../bin
CC65_INC=$(CC65)/include
CC65_ASMINC=$(CC65)/asminc
CC65_LIB=$(CC65)/lib
BUILDDIR=$(MAKEDIR)/$(BUILD)
ODIR=$(MAKEDIR)/obj
BUILD=$(MAKEDIR)../bin
.SUFFIXES : .c .s .o .asm .bmp .pal .spr
.SOURCE :
 
# Compiling for Atari 8-Bit system
SYS=atari
 
# Names of tools
CO=co65
CC=cc65
AS=ca65
AR=ar65
CL=cl65
SPRPCK=sprpck
CP=cp
RM=rm
ECHO=echo
TOUCH=touch
 
CODE_SEGMENT=CODE
DATA_SEGMENT=DATA
RODATA_SEGMENT=RODATA
BSS_SEGMENT=BSS
 
SEGMENTS=--code-name $(CODE_SEGMENT) \
	--rodata-name $(RODATA_SEGMENT) \
	--bss-name $(BSS_SEGMENT) \
	--data-name $(DATA_SEGMENT)
 
# Flag for assembler
AFLAGS=
 
# Flags for C-code compiler
CFLAGS=-I . -t $(SYS) --add-source -O -Or -Cl -Os -Oi -v -W const-comparison,no-effect
 
# Rule for making a *.o file out of a *.s file
.s.o:
	$(AS) -t $(SYS) -I $(CC65_ASMINC) -o $@ $(AFLAGS) $<
 
# Rule for making a *.o file out of a *.c file
.c.o:
	$(CC) -t $(SYS) $(SEGMENTS) $(CFLAGS) $<
	$(AS) -t $(SYS) -o $@ $(AFLAGS) $(*).s
