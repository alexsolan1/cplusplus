#include "optional.h"

using namespace std;

class MyClass {
	public:
	MyClass(int) {}
	~MyClass() {
		cout << "Dtor is called " << endl;
	}
};

int main() {
	Optional<int> opt;
	assert(!opt);
	Optional<std::string> optstring("Wedensday");
	assert(optstring);
	assert(optstring.get() == "Wedensday");
	std::cout << "All is well. " << std::endl;	

	Optional<MyClass> myopt;
	assert(!myopt);
	return 0;
}
