#include <iostream>
#include <type_traits>

namespace detail {
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
} // end of namespace detil


template <class... types>
struct mp_vector {
    static constexpr size_t size = sizeof...(types);
    template <size_t index>
    using at = typename detail::type_of_index<index, types...>::type;
    template <class T>
    using push_back = mp_vector<types..., T>;
};

template <class MetaVector, size_t index,  template<class> class MetaFunc>
struct for_each_struct {
    using cur_type = typename MetaVector::template at<index-1>;
    using push_type = typename MetaFunc<cur_type>::type;
    using prev_vector_t =  typename for_each_struct<MetaVector, index - 1, MetaFunc>::type;
    using type = typename prev_vector_t::template push_back<push_type>;
};

template <class MetaVector, template<class> class MetaFunc>
struct for_each_struct<MetaVector, 0, MetaFunc> {
    using type = mp_vector<>;
};
template <class MetaVector, template<class> class MetaFunc>
struct for_each {
    using type = typename for_each_struct<MetaVector, MetaVector::size, MetaFunc>::type;
};


template <class MetaVector, template<class> class MetaFunc>
using for_each_t = typename for_each<MetaVector, MetaFunc>::type;
