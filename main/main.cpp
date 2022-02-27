// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// Function prototypes
#include <core.h>
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
		return word_error;
	}

	int setup_error;
	setup_error=setup();
	if(setup_error!=0){
		cleanup(false);
		return setup_error;
	}

	bool attempt;
	attempt=run();

	return cleanup(attempt);
}
