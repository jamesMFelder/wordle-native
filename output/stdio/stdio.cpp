// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// Standard output functions
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <limits>
#include <map>

// Our function definitions
#include <output.h>

// Setup the screen
int setup(){
	cout << "Welcome to wordle.\n" << "Type your first guess and press enter.\n" << "Only the first 5 characters will be counted." << std::endl;
	return 0;
}

// Run the program
bool run(){
	static const std::string green="\033[92m";
	static const std::string yellow="\033[93m";
	static const std::string reset="\033[0m";
	static std::map<int, std::string> correct={{CORRECT, green+"█"+reset}, {BAD_PLACE, yellow+"█"+reset}, {WRONG, "█"}};
	std::array<char, WORD_LEN> guess;
	std::array<enum char_status, WORD_LEN> guess_correctness;

	// For each guess
	for(unsigned short guess_count=0; guess_count<NUM_GUESSES; guess_count++){
		// Prompt the user (+1 because normal people don't count from zero).
		std::cout << "Enter guess number " << guess_count+1 << std::endl;
		// Read in a char at a time
		for(unsigned short guess_char=0; guess_char<WORD_LEN; guess_char++){
			cin >> guess[guess_char];
			// End of file
			if(cin.eof()){
				// We're not getting anything else.
				return 1;
			}
			// Handle invalid characters
			if(!is_valid(guess[guess_char])){
				// By retrying
				// Clear the rest of the input line
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				// Error message
				std::cout << "Please only enter characters." << std::endl;
				// Retry guess instead of moving on to the next
				guess_count--;
				// Go to the outer for loop
				break;
			}
			// Lowercase it for simplicity
			guess[guess_char]=tolower(guess[guess_char]);
		}
		// Clear the rest of the input line
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		// Get the correctness of each character
		guess_correctness=check_guess(guess);
		// Keep track of if everything is good
		bool all_correct=true;
		// Output everything
		for(unsigned short guess_char=0; guess_char<WORD_LEN; guess_char++){
			std::cout << correct[guess_correctness[guess_char]];
			// If something is wrong, the whole thing can't be correct
			if(guess_correctness[guess_char]!=CORRECT){
				all_correct=false;
			}
		}
		// And add a newline (flushing the buffer)
		std::cout << std::endl;
		// If they entered the right word.
		if(all_correct){
			// Finish
			return true;
		}
	}
	return false;
}

// Clean up
int cleanup(bool attempt){
	// If the word was guessed
	if(attempt){
		// Output a message
		std::cout << "Congragulations! You guessed the word!" << std::endl;
	}
	else{
		std::cout << "The word was ";
		auto word=return_word();
		for(auto c: word){
			std::cout << c;
		}
		std::cout << '.'<< std::endl;
	}
	return 0;
}
