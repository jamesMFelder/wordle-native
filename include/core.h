// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

#ifndef CORE_H
#define CORE_H

// Used by some functions
#include <array>
#include <string>

#define WORD_LEN 5
#define NUM_GUESSES 6

// Set the word of the day
int setWord(const std::string &fname, const char *progname);

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

#endif //CORE_H
