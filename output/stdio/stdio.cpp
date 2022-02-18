// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// Standard output functions
#include <iostream>
using std::cout;
using std::endl;

// Our function definitions
#include <output.h>

// Setup the screen
int setup(){
	std::cout << "Welcome to wordle.\n" << "Type your first guess and press enter.\n" << "Only the first 5 characters will be counted." << std::endl;
	return 0;
}

// Run the program
int run(){
	//TODO: impliment
	return 1;
}

// Clean up
int cleanup(){
	return 0;
}
