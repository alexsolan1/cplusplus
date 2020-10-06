#include <cmath>
/**
* Fibonacci
*/
template <unsigned n>
struct fibonacci{
	static const unsigned value = fibonacci<n - 1>::value + fibonacci<n - 2>::value;
};

template <>
struct fibonacci<1> {
	static const unsigned value = 1;
};

template <>
struct fibonacci<0> {
	static const unsigned value = 0;
};
/**
* Check for Prime
*/
template <unsigned n, unsigned k>
struct divided_by_range {
	static const bool value = (n % k == 0) || divided_by_range<n, k-1>::value;
};

template<unsigned n>
struct divided_by_range<n, 1> {
	static const bool value = false;
};

template <unsigned n>
struct is_prime {
	static const bool value = !divided_by_range<n, int(sqrt(n) + 1)>::value;
};

/**
* Check for add pointer
*/
template <class T>
struct add_pointer {
	using type = T*;
};

/**
* remove_pointer
*/
template <class T>
struct remove_pointer {
	using type = T;
};

template <class T>
struct remove_pointer<T*> {
	using type = T;
};


template <class T>
struct remove_pointer<T* const> {
	using type = T;
};

template <class T>
struct remove_pointer<T* volatile> {
	using type = T;
};


template <class T>
struct remove_pointer<T* const volatile> {
	using type = T;
};

