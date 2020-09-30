#include <cassert>
#include <array>
#include "mp_vector.h"

using namespace std;

int main() {
    using mp_vec_t = mp_vector<bool, char, array<char, 17>>;
    static_assert(mp_vector<bool, char, double>::size == 3);
    assert(sizeof(mp_vec_t::at<2>) == 17);
    using bigger_vec_t = mp_vec_t::push_back<array<char, 5>>;
    assert(sizeof(bigger_vec_t::at<3>) == 5);
    cout << "All is well." << endl;
    return 0;
}
