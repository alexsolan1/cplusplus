#include <cassert>
#include <array>
#include "mp_vector.h"

using namespace std;
template <class T>
struct ArrayOf3 {
    using type = array<T, 3>;
};

int main() {
    using mp_vec_t = mp_vector<size_t, bool, char, array<char, 17>>;
    static_assert(mp_vector<bool, char, double>::size == 3);
    assert(sizeof(mp_vec_t::at<3>) == 17);
    using bigger_vec_t = mp_vec_t::push_back<array<char, 5>>;
    assert(sizeof(bigger_vec_t::at<4>) == 5);
    
    using arrays_of_t = for_each_t<mp_vec_t, ArrayOf3>;
    static_assert(is_same_v<arrays_of_t::at<0>, ArrayOf3<size_t>::type>);
    static_assert(is_same_v<arrays_of_t::at<1>, ArrayOf3<bool>::type>);
    static_assert(is_same_v<arrays_of_t::at<2>, ArrayOf3<char>::type>);
    static_assert(is_same_v<arrays_of_t::at<3>, ArrayOf3<array<char, 17>>::type>);
    cout << "All is well." << endl;
    return 0;
}
