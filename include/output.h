// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

#ifndef OUTPUT_H
#define OUTPUT_H

#define WORD_LEN 5
#define NUM_GUESSES 6

// Setup the screen
// Returns 0 on success, non-zero on failure.
// A non-zero error will be propogated out of main.
int setup();

// Run the program.
// Return 0 on success, non-zero on failure.
// A non-zero error will be propogated out of main.
// Should give the user NUM_GUESSES.
// I'm not explaining the rules here.
int run();

// Clean up
// Returns 0 on success, non-zero on failure.
// The return value will be propogated out of main unless another error occured.
int cleanup();

// Guess correctness (for output: color info)
enum char_status{
	CORRECT,
	BAD_PLACE,
	WRONG,
	UNSPECIFIED
};

// Called by the output when it gets a guess.
// The non null-terminated guess array is what should be checked.
// After calling, the result array will contain a char_status
// for each of the corresponding elements in the guess array.
void check_guess(char guess[WORD_LEN], enum char_status result[WORD_LEN]);

#endif //OUTPUT_H
