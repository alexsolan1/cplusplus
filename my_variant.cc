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
	
	using my_variant_t = my_variant<int, double, MyClass, OtherClass>;
	my_variant_t my2(MyClass{});
	my_variant_t other(OtherClass{});

	my_variant<int> int_variant(2);
	cout << "Int in signle variant is: " << get<int>(int_variant) << endl;

	struct MyVisitor {
		void operator()(int& x) {cout << "Int = " << x << endl;}
		void operator()(double& x) {cout << "Double = " << x << endl;}
		void operator()(MyClass& other) {cout << "I don't care about MyClass " << endl; }
		void operator()(OtherClass& other) {cout << "I don't care about OtherClass " << endl; }
	};
	
	visitor_struct<MyVisitor,my_variant_t, int, double, MyClass, OtherClass> ::visit(MyVisitor{}, move(my2));
	visitor_struct<MyVisitor,my_variant_t, int, double, MyClass, OtherClass> ::visit(MyVisitor{}, move(other));
	return 0;
}
