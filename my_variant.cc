#include "my_variant.h"

using namespace std;

struct MyClass {
	double x;
	double y;
	~MyClass () {
		cout << "MyClass dtor is called " << endl;
	}

};
struct OtherClass {
	bool other_bool;
	~OtherClass () {
		cout << "OtherClass dtor is called " << endl;
	}

};
int main() {
	cout << max_size_helper<int, double, MyClass, char>::value << endl;
	cout << max_align_helper<int, double, MyClass, char>::value << endl;
	
	cout << get_index<float, int, MyClass, float>::value << endl;
	cout << get_index<int, int, MyClass, float>::value << endl;

	my_variant<int, string, MyClass> my(string("alex"));
	cout << my.index() << endl;
	cout << "The string is:  " << get<string>(my) << endl;

	my_variant<int, double, MyClass, OtherClass> my2(MyClass{});
	my_variant<int, double, MyClass, OtherClass> other(OtherClass{});
	return 0;
}
