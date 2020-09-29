#include <cassert>
#include <iostream>
#include <algorithm>
#include <type_traits>

using std::cout;
using std::endl;


template <typename... types>
struct max_size_helper { };

template <typename T, typename... types>
struct max_size_helper<T, types...> {
	static size_t constexpr value = std::max(sizeof(T), max_size_helper<types...>::value);
};

template <> 
struct max_size_helper<> {
	static size_t constexpr value = 0;
};


template <typename... types>
struct max_align_helper { };

template <typename T, typename... types>
struct max_align_helper<T, types...> {
	static size_t constexpr value = std::max(alignof(T), max_align_helper<types...>::value);
};

template <> 
struct max_align_helper<> {
	static size_t constexpr value = 0;
};

/**
* get index of type from parameter pack
*/

template <typename T, typename... Ts> struct get_index;

template <typename T, typename... Ts>
struct get_index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

template <typename T, typename Tail, typename... Ts>
struct get_index<T, Tail, Ts...> :
    std::integral_constant<std::size_t, 1 + get_index<T, Ts...>::value> {};

template <typename T>
struct get_index<T> {
	static_assert(sizeof(T) == 0, "Element is not found");
};

template <class Obj, class... types> struct destroy_all;
template <class T, class... types>
struct my_variant {
	my_variant() : m_member(new (&m_aligned_storage) T{}) { }

	my_variant(my_variant<T, types...>&) {
		assert(sizeof(T) == 0 && "Not implemented");
	}
	my_variant(my_variant<T, types...>&&) {
		assert(sizeof(T) == 0 && "Not implemented");
	}
	// need to add emplacement ctors...

	template <class U>
	my_variant(U&& u) :
		m_index(get_index<U, T, types...>::value),	
 		m_member(new (&m_aligned_storage) U{u} )
	{ }
	~my_variant() {
		destroy_all<decltype(*this), T, types...>::f(*this);	
	}			
	size_t index() const { return m_index; }


//	template <class U> friend U& get<U>(my_variant<T, types...>& my); //compilation issues should be solved
//private:
	std::aligned_storage_t<max_size_helper<types...>::value, max_align_helper<types...>::value> m_aligned_storage;
	void* m_member;
	size_t m_index;
};

template <class U, class... types>
U& get(my_variant<types...>& my) {
	size_t index = get_index<U, types...>::value;
	assert(index == my.index()); // throw bad_variant_access instead
	return *static_cast<U*>(my.m_member);
}

	
template <class T, class... types>
void destroy(my_variant<types...>& my) {
	size_t index = get_index<T, types...>::value;
	if (index != my.index()) {
		return;
	}
	(static_cast<T*>(my.m_member))->~T();
}

template <class Obj, class... types> struct destroy_all{};
template <class Obj, class T, class... types> struct destroy_all<Obj, T, types...>{
	static void f(Obj& in_obj) {
		destroy<T>(in_obj);
		destroy_all<Obj, types...>::f(in_obj);
	}	
};
template <class Obj> struct destroy_all<Obj> {
	static void f(Obj& in_obj) { }
};






