// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// Our function definitions
#include <output.h>
// Functions for controlling the screen
#include <ncurses.h>
// For exceptional errors
#include <exception>
#include <stdexcept>
// For keeping track of what letters are guessed for the on-screen keyboard
#include <array>

class onScreenKeyboard{
	public:
		onScreenKeyboard(){}
		void drawToScreen(int x, int y){
			// Add read-only keyboard on the left
			for(char c=0; c<26; c++){
				mvaddch(c/2+y, c%2+x, (c+'a') | COLOR_PAIR(type[c]));
			}
			refresh();
		}

		// Pass in lowercase! Do not pass in duplicates!
		void addLetter(char letter, char_status value){type[letter-'a']=value;}

	private:
		std::array<enum char_status, 26> type{UNSPECIFIED};
};

onScreenKeyboard virtKeyboard;

// Setup the screen
int setup(){
	// Initialize ncurses
	initscr();
	// Basic sanity check for a big enough screen
	if(LINES<13 || COLS<9){
		//Too small for anything fancy
		endwin();
		fputs("Error: too small screen.\n", stderr);
		fputs("Needs to be at least 9x13 characters.\n", stderr);
		return 1;
	}
	// We use color just like the official app
	start_color();
	use_default_colors();
	// Setup the colors
#define COLOR_GREY 8
	init_color(COLOR_GREY, 500, 500, 500);
	init_pair(CORRECT, COLOR_BLACK, COLOR_GREEN);
	init_pair(BAD_PLACE, COLOR_BLACK, COLOR_YELLOW);
	init_pair(WRONG, COLOR_BLACK, COLOR_GREY);
	init_pair(UNSPECIFIED, COLOR_BLACK, COLOR_WHITE);
	// Character at a time input
	cbreak();
	// Allow use of the keypad
	keypad(stdscr, TRUE);

	// Draw the board (TODO: don't hardcode the size?)
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
	// Add read-only keyboard on the left
	virtKeyboard.drawToScreen(COLS/2-5, LINES/2-6);
	move(LINES/2-5, COLS/2-2);
	refresh();

	return 0;
}

class Location{
	public:
		Location():column(0),row(0){}
		Location(unsigned short column, unsigned short row){
			if(!(row<NUM_GUESSES && column<WORD_LEN)){
				throw std::out_of_range("Cannot move outside of the grid.");
			}
			this->column=column;
			this->row=row;
			syncToScreen();
		}
		void moveLeft(){
			if(column!=0){
				column--;
				syncToScreen();
			}
		}
		void moveRight(){
			if(column!=WORD_LEN-1){
				column++;
				syncToScreen();
			}
		}
		void moveDown(){
			if(row!=NUM_GUESSES*2){
				row+=2;
				syncToScreen();
			}
		}
		void moveHome(){
			column=0;
			syncToScreen();
		}
		void moveEnd(){
			column=WORD_LEN-1;
			syncToScreen();
		}
		//Only call if you have moved the cursor, but not through this.
		void syncToScreen(){
			move(LINES/2-5+row, COLS/2-2+column);
		}
	private:
		unsigned short column, row;
};

// Run the program
int run(){
	char guess[WORD_LEN];
	std::array<enum char_status, WORD_LEN> guess_correctness;
	Location where; //Where we are typing
	// For each guess
	for(unsigned short guess_count=0; guess_count<NUM_GUESSES; guess_count++){
		// Read in a char at a time
		getnstr(guess, WORD_LEN);
		for(unsigned short guess_char=0; guess_char<WORD_LEN; guess_char++){
			// Handle invalid characters
			if(!is_valid(guess[guess_char])){
				// By retrying
				// Reset the line
				for(guess_char=0, where.moveHome(); guess_char<WORD_LEN; guess_char++, where.moveRight()){
					addch(' ');
				}
				// Retry guess instead of moving on to the next
				guess_count--;
				// Do the next run of the outer loop (counts as redoing because we deincremented guess_count)
				goto retry;
			}
			// Lowercase it for simplicity
			guess[guess_char]=tolower(guess[guess_char]);
		}
		{
			// Get the correctness of each character
			guess_correctness=check_guess(guess);
			// Keep track of if everything is good
			bool all_correct=true;
			// Output everything
			where.moveHome();
			for(unsigned short guess_char=0; guess_char<WORD_LEN; guess_char++, where.moveRight()){
				chgat(1, 0, guess_correctness[guess_char], NULL);
				virtKeyboard.addLetter(guess[guess_char], guess_correctness[guess_char]);
				// If something is wrong, the whole thing can't be correct
				if(guess_correctness[guess_char]!=CORRECT){
					all_correct=false;
				}
			}
			// If they entered the right word.
			if(all_correct){
				// Finish
				return 0;
			}
		}
		// Update the onscreen keyboard
		virtKeyboard.drawToScreen(COLS/2-5, LINES/2-6);
		where.moveDown();
		// Jump here if you need to retry the current guess (coming from inside another for loop)
		// Comes after the moveDown() because we are retrying, not moving on to the next guess.
		// I think this is the correct thing to do because the app doesn't even let you enter an invalid character.
		retry:
		where.moveHome();
	}
	return 0;
}

// Wait for a keypress and then clean up.
int cleanup(){
	auto cursor_visibility=curs_set(0);
	getch();
	if(cursor_visibility!=ERR){
		curs_set(cursor_visibility);
	}
	return endwin();
}
