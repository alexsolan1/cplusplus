#include "optional.h"

using namespace std;

class MyClass {
	public:
	MyClass(int x) { 
		cout << "Creating object " << endl;
		_x = x;
	}
	MyClass(const MyClass& myclass) {
		cout << "Copy ctor is called " << endl;
		_x = myclass.x();
	}
	~MyClass() {
		cout << "Dtor is called " << endl;
	}
	int x() const { 
		return _x;
	}	
private:
	int _x;
};


Optional<MyClass> create_my_class() {
	return Optional<MyClass>(100);
}

int main() {
	Optional<int> opt;
	assert(!opt);
	Optional<std::string> optstring("Wedensday");
	assert(optstring);
	assert(optstring.get() == "Wedensday");
	std::cout << "All is well. " << std::endl;	

	Optional<MyClass> myopt;
	assert(!myopt);

	MyClass my_class(10);

	Optional<MyClass> opt_my_class(my_class);
	assert(opt_my_class.get().x() == 10);
	Optional<MyClass> opt_my_class2 = create_my_class();

	return 0;
}
