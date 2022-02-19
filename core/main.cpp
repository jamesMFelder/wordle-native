// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// For controlled screen output
#include <output.h>
// Enabling wide characters with the users locale
#include <locale.h>
// Checking if characters are valid
#include <cctype>

// Return the word of the day.
// All lowercase
std::array<char, WORD_LEN> getWord(){
	return {'h', 'e', 'l', 'l', 'o'};
}

// Check if a char is legal
bool is_valid(unsigned char c){
	return isalpha(c);
}

// Check if a guess is valid
std::array<enum char_status, WORD_LEN> check_guess(const std::array<char, WORD_LEN> guess){
	// Create the array we are going to use
	std::array<enum char_status, WORD_LEN> result;
	// Get the current word
	std::array<char, WORD_LEN> word=getWord();

	unsigned short guess_position=0;

	// Iterate through and mark all the correct guesses
	for(guess_position=0; guess_position<WORD_LEN; guess_position++){
		if(guess[guess_position]==word[guess_position]){
			result[guess_position]=CORRECT;
			word[guess_position]='\0';
		}
		else{
			result[guess_position]=UNSPECIFIED;
		}
	}

	// Iterate through and mark all the guesses in the wrong place (not duplicating correct ones)
	for(guess_position=0; guess_position<WORD_LEN; guess_position++){
		if(result[guess_position]==UNSPECIFIED){
			for(unsigned short word_position=0; word_position<WORD_LEN; word_position++){
				if(guess[guess_position]==word[word_position]){
					result[guess_position]=BAD_PLACE;
					word[word_position]='\0';
					break;
				}
			}
		}
	}

	// Everything else must be wrong
	for(guess_position=0; guess_position<WORD_LEN; guess_position++){
		if(result[guess_position]==UNSPECIFIED){
			result[guess_position]=WRONG;
		}
	}
	return result;
}

// Run the program
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
