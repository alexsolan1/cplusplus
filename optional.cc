#include "optional.h"

int main() {
	Optional<int> opt;
	assert(!opt);
	Optional<std::string> optstring("Wedensday");
	assert(optstring);
	assert(optstring.get() == "Wedensday");
	std::cout << "All is well. " << std::endl;	

	return 0;
}
