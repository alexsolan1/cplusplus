#include <iostream>
#include <type_traits>

template<size_t index, class... types> struct type_of_index { };
template<size_t index, class T, class... types> 
struct type_of_index<index, T, types...> {
    using type = typename type_of_index<index - 1, types...>::type;
};

template<class T, class... types> 
struct type_of_index<0, T, types...> {
    using type =  T;
};

template<> 
struct type_of_index<0> {
};



template <class... types>
struct mp_vector {
    static constexpr size_t size = sizeof...(types);
    template <size_t index>
    using at = typename type_of_index<index, types...>::type;
    template <class T>
    using push_back = mp_vector<types..., T>;
};
