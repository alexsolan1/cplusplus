#include <iostream>
#include <type_traits>
#include "utils.h"

using std::cout;
using std::endl;
using std::boolalpha;

struct MyStruct {
	int x;
};

int main() {
	static_assert(fibonacci<7>::value == 13);

	cout << boolalpha << divided_by_range<10, 4>::value << " " <<  divided_by_range<10, 2>::value << endl;
	cout << boolalpha << divided_by_range<15, 4>::value << " " <<  divided_by_range<15, 2>::value << endl;
	cout << boolalpha << divided_by_range<121, 11>::value << " " <<  divided_by_range<721, 30>::value << endl;


	cout << "13? " << boolalpha << is_prime<13>::value << endl;
	cout << "17? " << boolalpha << is_prime<17>::value << endl;
	cout << "19? " << boolalpha << is_prime<19>::value << endl;
	cout << "21? " << boolalpha << is_prime<21>::value << endl;

	static_assert(is_prime<13>::value);
	static_assert(is_prime<17>::value);
	static_assert(is_prime<19>::value);
	static_assert(!is_prime<21>::value);

	using size_t_ptr = size_t*;
	static_assert(std::is_same<add_pointer<size_t>::type, size_t_ptr>::value);
		
	using MyStructP = MyStruct*;
	using MyStructPP = MyStructP*;
	static_assert(std::is_same<add_pointer<MyStruct>::type, MyStructP>::value);
		
	static_assert(std::is_same<remove_pointer<MyStructP>::type, MyStruct>::value);
	static_assert(std::is_same<remove_pointer<MyStructPP>::type, MyStructP>::value);
	static_assert(std::is_same<remove_pointer<const MyStructP>::type, MyStruct>::value);
	static_assert(std::is_same<remove_pointer<const volatile MyStructP>::type, MyStruct>::value);

	return 0;
}
