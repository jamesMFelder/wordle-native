// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// For controlled screen output
#include <output.h>
// Enabling wide characters with the users locale
#include <locale.h>

// Fill the buffer with the word of the day.
// It is not null-terminated!
// It is always fully uppercase.
// TODO: use an external wordlist
void getWord(char word[WORD_LEN]){
	word[0]='H';
	word[1]='E';
	word[2]='L';
	word[3]='L';
	word[4]='O';
}

int main(){
	// Use wide characters with the users locale
	setlocale(LC_ALL, "");

	int setup_error;
	setup_error=setup();
	if(setup_error!=0){
		cleanup();
		return setup_error;
	}

	int run_error;
	run_error=run();
	if(run_error!=0){
		cleanup();
		return run_error;
	}

	return cleanup();
}

void check_guess(const char guess[WORD_LEN], enum char_status results[WORD_LEN]){
	// Get the current word
	char word[5];
	getWord(word);

	unsigned short guess_position=0;

	// Iterate through and mark all the correct guesses
	for(guess_position=0; guess_position<WORD_LEN; guess_position++){
		if(guess[guess_position]==word[guess_position]){
			results[guess_position]=CORRECT;
			word[guess_position]='\0';
		}
		else{
			results[guess_position]=UNSPECIFIED;
		}
	}

	// Iterate through and mark all the guesses in the wrong place (not duplicating correct ones)
	for(guess_position=0; guess_position<WORD_LEN; guess_position++){
		for(unsigned short word_position=0; word_position<WORD_LEN; word_position++){
			if(guess[guess_position]==word[word_position]){
				results[guess_position]=BAD_PLACE;
				word[word_position]='\0';
			}
		}
	}

	// Everything else must be wrong
	for(guess_position=0; guess_position<WORD_LEN; guess_position++){
		if(results[guess_position]==UNSPECIFIED){
			results[guess_position]=WRONG;
		}
	}
}
