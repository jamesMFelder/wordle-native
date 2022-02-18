// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

#ifndef OUTPUT_H
#define OUTPUT_H

// Setup the screen
// Returns 0 on success, non-zero on failure.
// A non-zero error will be propogated out of main.
int setup();

// Clean up
// Returns 0 on success, non-zero on failure.
// The return value will be propogated out of main unless another error occured
int cleanup();

#endif //OUTPUT_H
