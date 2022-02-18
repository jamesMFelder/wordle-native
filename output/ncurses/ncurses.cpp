// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// Our function definitions
#include <output.h>
// Functions for controlling the screen
#include <ncurses.h>

// Setup the screen
int setup(){
	// Initialize ncurses
	initscr();
	// Basic sanity check for a big enough screen
	if(LINES<13 || COLS<7){
		//Too small for anything fancy
		endwin();
		fputs("Error: too small screen.\n", stderr);
		fputs("Needs to be at least 7x13 characters.\n", stderr);
		return 1;
	}
	// Character at a time input
	cbreak();
	// Don't automatically echo the characters
	noecho();
	// Don't automatically output every character to the screen on ^C
	intrflush(stdscr, FALSE);
	// Allow use of the keypad
	keypad(stdscr, TRUE);

	// Draw the board
	// Inner lines
	mvhline(LINES/2-4, COLS/2-3, 0, 7);
	mvhline(LINES/2-2, COLS/2-3, 0, 7);
	mvhline(LINES/2-0, COLS/2-3, 0, 7);
	mvhline(LINES/2+2, COLS/2-3, 0, 7);
	mvhline(LINES/2+4, COLS/2-3, 0, 7);
	// Outer lines
	mvhline(LINES/2-6, COLS/2-3, 0, 7); //top
	mvvline(LINES/2-6, COLS/2-3, 0, 12); //left
	mvhline(LINES/2+6, COLS/2-3, 0, 7); //bottom
	mvvline(LINES/2-6, COLS/2+3, 0, 12); //right
	// Corners
	mvaddch(LINES/2-6, COLS/2-3, ACS_ULCORNER);
	mvaddch(LINES/2-6, COLS/2+3, ACS_URCORNER);
	mvaddch(LINES/2+6, COLS/2-3, ACS_LLCORNER);
	mvaddch(LINES/2+6, COLS/2+3, ACS_LRCORNER);
	// left-hand side t junctions
	mvaddch(LINES/2, COLS/2-3, ACS_LTEE);
	mvaddch(LINES/2-2, COLS/2-3, ACS_LTEE);
	mvaddch(LINES/2+2, COLS/2-3, ACS_LTEE);
	mvaddch(LINES/2-4, COLS/2-3, ACS_LTEE);
	mvaddch(LINES/2+4, COLS/2-3, ACS_LTEE);
	// right-hand side t junctions
	mvaddch(LINES/2, COLS/2+3, ACS_RTEE);
	mvaddch(LINES/2-2, COLS/2+3, ACS_RTEE);
	mvaddch(LINES/2+2, COLS/2+3, ACS_RTEE);
	mvaddch(LINES/2-4, COLS/2+3, ACS_RTEE);
	mvaddch(LINES/2+4, COLS/2+3, ACS_RTEE);
	move(LINES/2-5, COLS/2-2);
	refresh();

	return 0;
}

// Run the program
int run(){
	//TODO: impliment
	return 1;
}

// Clean up
int cleanup(){
	getch();
	return endwin();
}
