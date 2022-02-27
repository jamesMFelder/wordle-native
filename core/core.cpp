// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// Function prototypes
#include <core.h>
// Checking if characters are valid
#include <cctype>
// Comparing strings (argument parsing)
#include <cstring>
// Error messages
#include <iostream>
// Reading the wordlist
#include <fstream>
// Storing the wordlist
#include <vector>
// Picking the word from the wordlist
#include <random>
// std::string is not std::array<char, n>
#include <algorithm>
// Randomizing the random number generator
#include <chrono>

// The word of the day all lowercase.
static std::array<char, WORD_LEN> wordOfDay;

// Set the word of the day from fname.
// progname is used for errors (char* instead of std::string because it should just be argv[0])
int setWord(const std::string &fname, const char *progname){
	std::ifstream wordlist_file(fname);
	std::vector<std::array<char, 5>> wordlist;

	// Check if the wordlist file is open
	if(!wordlist_file.is_open()){
		std::cerr << progname << " error: could not open " << fname << std::endl;
		return 1;
	}
	// Iterate through line by line (infinite loop because we read characters in an inner loop)
	while(true){
		// Storage for the current word
		std::array<char, WORD_LEN> cur_word;
		// Read in characters until we have 5 or the file ends
		for(unsigned short cur_char=0; cur_char<WORD_LEN && !wordlist_file.eof(); cur_char++){
			// Read the character into the next spot
			wordlist_file >> cur_word[cur_char];
			// If it isn't valid, restart
			if(!is_valid(cur_word[cur_char])){
				cur_char=0;
			}
		}
		// If the file ended (we didn't get a full word)
		if(wordlist_file.eof()){
			// We have all the words we are going to get
			break;
		}
		// Otherwise (we have at least a full word)
		// Unless more than WORD_LEN characters in a row are valid we should store it
		else if(!is_valid(wordlist_file.peek())){
			// Store it
			wordlist.push_back(cur_word);
		}
		// There is a valid character after the word (it is part of a larger one)
		else{
			// Run to the end of it
			while(is_valid(wordlist_file.peek())){
				wordlist_file.ignore();
			}
		}
	}

	// If we got no words (empty file, not text or no five-letter strings of valid characters)
	if(wordlist.size()==0){
		// Quit
		std::cerr << progname << " error: no valid words in " << fname << std::endl;
		return 1;
	}

	//randomly pick word from wordlist and save it
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine engine(seed);
	std::uniform_int_distribution<unsigned> random(0, wordlist.size()-1);
	auto which=random(engine);
	wordOfDay=wordlist[which];

	//lowercase it
	for(unsigned int index=0; index<WORD_LEN; index++){
		wordOfDay[index]=tolower(static_cast<unsigned char>(wordOfDay[index]));
	}
	return 0;
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
	std::array<char, WORD_LEN> word=wordOfDay;

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
std::array<enum char_status, WORD_LEN> check_guess(const char guess[WORD_LEN]){
	// Create the array we are going to use
	std::array<enum char_status, WORD_LEN> result;
	// Get the current word
	std::array<char, WORD_LEN> word=wordOfDay;

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

// Return the full word if it was never guessed
std::array<char, WORD_LEN> return_word(){
	return wordOfDay;
}
