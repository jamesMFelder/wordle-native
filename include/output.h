// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

#ifndef OUTPUT_H
#define OUTPUT_H

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

#endif //OUTPUT_H
