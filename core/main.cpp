// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// For controlled screen output
#include <output.h>
// Enabling wide characters with the users locale
#include <locale.h>
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
		// Otherwise (we have a full word)
		else{
			// Store it
			wordlist.push_back(cur_word);
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

// Run the program
int main(int argc, char *argv[]){
	// Use wide characters with the users locale
	setlocale(LC_ALL, "");
	// A string containing basic helptext (terminated by and contains newlines)
	std::string usage="wordle [args]\n" \
	"\t-h | --help\t\tsee this help\n"
	"\t-f | --file=FILE\tuse words from FILE\n";
	// The file containing a wordlist
	std::string wordlist_file_name="wordlist";

	// Handle arguments
	if(argc>1){
		for(int cur_arg=1; cur_arg<argc; cur_arg++){
			// Show help
			if(std::strcmp(argv[cur_arg], "-h")==0 || std::strcmp(argv[cur_arg], "--help")==0){
				std::cout << usage;
				return 0;
			}
			// Set the wordlist file
			else if(std::strncmp(argv[cur_arg], "-f", 2)==0){
				// If the filename is not a seperate argument
				if(argv[cur_arg][2]!='\0'){
					// Don't save the '-f'
					wordlist_file_name=argv[cur_arg]+2;
				}
				// It is a seperate argument, so check that the next argument exists
				else if(argc>cur_arg+1 && argv[cur_arg+1]!=NULL){
					// Save it
					wordlist_file_name=argv[cur_arg+1];
					// And ignore it for parsing the next option
					cur_arg++;
				}
				// It is not passed
				else{
					std::cerr << argv[0] << " error: the '-f' option requires a filename.\n";
					std::cerr << usage;
					return 1;
				}
			}
			else if(std::strncmp(argv[cur_arg], "--file", 6)==0){
				// If the filename is not a seperate argument
				if(argv[cur_arg][6]=='='){
					// Don't save the '--file='
					wordlist_file_name=argv[cur_arg]+7;
				}
				// It is a seperate argument, so check that the next argument exists
				else if(argc>cur_arg+1 && argv[cur_arg+1]!=NULL){
					// Save it
					wordlist_file_name=argv[cur_arg+1];
					// And ignore it for parsing the next option
					cur_arg++;
				}
				// It is not passed
				else{
					std::cerr << argv[0] << " error: the '--file' option requires a filename\n";
					std::cerr << usage;
					return 1;
				}
			}
			// Handle unexpected command line arguments
			else{
				std::cerr << argv[0] << " error: unknown option " << argv[cur_arg] << std::endl;
				std::cerr << usage;
				return 1;
			}
		}
	}

	// Set the word of the day
	int word_error;
	word_error=setWord(wordlist_file_name, argv[0]);
	if(word_error!=0){
		cleanup();
		return word_error;
	}

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
