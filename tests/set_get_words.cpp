#include <core.h>
#include <iostream>

int main(int argc, char *argv[]){
	// Load the wordlist
	int set_word=setWord(argv[1], argv[0]);
	if(set_word!=0){
		std::cerr << "Picking the word failed with error " << set_word << std::endl;
		return set_word;
	}
	// The only valid word should be 'hello'
	const std::array<char, WORD_LEN> expected_word{'h', 'e', 'l', 'l', 'o'};
	// Check what we get
	std::array<char, WORD_LEN> actual_word=return_word();
	// If it is wrong, abort with an error
	if(actual_word!=expected_word){
		std::clog << "Wrong word returned. Expected '";
		for(auto c : expected_word){
			std::clog << c;
		}
		std::clog << "' but got '";
		for(auto c : actual_word){
			std::clog << c;
		}
		std::clog << "' instead." << std::endl;
		return 1;
	}

	// Check if it returns the right response for a correct guess
	std::array<enum char_status, WORD_LEN> actual_guess=check_guess(expected_word);
	const std::array<enum char_status, WORD_LEN> correct_expected_guess={CORRECT, CORRECT, CORRECT, CORRECT, CORRECT};
	// If it doesn't, abort with an error message
	if(actual_guess!=correct_expected_guess){
		std::clog << "Wrong word returned. Expected '";
		for(auto c : correct_expected_guess){
			std::clog << c;
		}
		std::clog << "' but got '";
		for(auto c : actual_guess){
			std::clog << c;
		}
		std::clog << "' instead." << std::endl;
		return 1;
	}

	// Check if it returns the right response for a partially-correct guess
	actual_guess=check_guess("world");
	const std::array<enum char_status, WORD_LEN> incorrect_expected_guess={WRONG, BAD_PLACE, WRONG, CORRECT, WRONG};
	if(actual_guess!=incorrect_expected_guess){
		std::clog << "Wrong word returned. Expected '";
		for(auto c : incorrect_expected_guess){
			std::clog << c;
		}
		std::clog << "' but got '";
		for(auto c : actual_guess){
			std::clog << c;
		}
		std::clog << "' instead." << std::endl;
		return 1;
	}
}
