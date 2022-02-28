#include <core.h>
#include <limits>
#include <iostream>

// Test that is_valid returns correct for alphabetic characters, but not for anything else.
int main(){
	// Loop through each option
	for(unsigned char c=0; c<std::numeric_limits<unsigned char>::max(); c++){
		// If it is different than isalpha (TODO: what if the user has a different locale?)
		if(is_valid(c)!=static_cast<bool>(isalpha(c))){
			// Output an error and return failure
			std::clog << std::boolalpha;
			std::clog << "is_valid(" << c << ") returned " << is_valid(c);
			std::clog << " when it should have returned " << static_cast<bool>(isalpha(c)) << std::endl;
			return 1;
		}
	}
	// We didn't return in the loop, so it must have been correct for everything.
	return 0;
}
