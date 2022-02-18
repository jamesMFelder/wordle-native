// SPDX-License-Identifier: GPLv3-or-later
// Copyright (c) 2022 James McNaughton Felder

// For controlled screen output
#include <output.h>
// Enabling wide characters with the users locale
#include <locale.h>

// Fill the buffer with the word of the day.
// It is not null-terminated!
// It is always fully uppercase.
// TODO: use an external wordlist
void getWord(char word[5]){
	word[0]='H';
	word[1]='E';
	word[2]='L';
	word[3]='L';
	word[4]='O';
}

int main(){
	// Use wide characters with the users locale
	setlocale(LC_ALL, "");

	int setup_error;
	setup_error=setup();
	if(setup_error!=0){
		return setup_error;
	}

	// Get the word of the day
	char curWord[5];
	getWord(curWord);

	return cleanup();
}
