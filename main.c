// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// For controlled screen output
#include <ncurses.h>
// Enabling wide characters with the users locale
#include <locale.h>

// Fill the buffer with the word of the day.
// It is not null-terminated!
// It is always fully uppercase.
// TODO: use an external wordlist
void getWord(char word[5]){
	word[0]='H';
	word[1]='E';
	word[2]='L';
	word[3]='L';
	word[4]='O';
}

int main(){
	// Use wide characters with the users locale
	setlocale(LC_ALL, "");

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

	// Get the word of the day
	char curWord[5];
	getWord(curWord);

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
	move(LINES/2-5, COLS/2-2);

	//TODO: loop reading keypresses and displaying them
	getch();

	// Clean up the terminal
	// (should be already setup as sigint and sigterm handler by ncurses)
	endwin();
	return 0;
}
