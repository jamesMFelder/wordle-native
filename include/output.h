// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

#ifndef OUTPUT_H
#define OUTPUT_H

// Used by some functions
#include <array>

#define WORD_LEN 5
#define NUM_GUESSES 6

// Setup the screen
// Returns 0 on success, non-zero on failure.
// A non-zero error will be propogated out of main.
int setup();

// Run the program.
// Return true if it was guessed, false otherwise
// Should give the user NUM_GUESSES.
// I'm not explaining the rules here.
bool run();

// Clean up
// Returns 0 on success, non-zero on failure.
// The return value will be propogated out of main unless another error occured.
int cleanup(bool attempt);

// Guess correctness (for output: color info)
enum char_status{
	UNSPECIFIED,
	CORRECT,
	BAD_PLACE,
	WRONG,
};

// Called by the output when it gets a guess.
// The non null-terminated guess array is what should be checked.
// After calling, the result array will contain a char_status
// for each of the corresponding elements in the guess array.
std::array<enum char_status, WORD_LEN> check_guess(const std::array<char, WORD_LEN> guess);
std::array<enum char_status, WORD_LEN> check_guess(const char guess[WORD_LEN]);

std::array<char, WORD_LEN> return_word();

// Called by the output for each char to determine whether it is valid
bool is_valid(unsigned char c);

#endif //OUTPUT_H
