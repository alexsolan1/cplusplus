#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

// 1. Xor to each char
string xored_string(string s, char n) {
    return accumulate(s.begin(), s.end(), string(""), [n](
        const string& acc, const char& c){return acc + char(c^n);});
}
// reverse
string my_reverse(string s) {
    return accumulate(s.crbegin(), s.crend(), string(""), [](
        const string& acc, const char& c){return acc + c;});
}

// check all capitals
bool all_capitals(const vector<string>& names) {
    return accumulate(names.begin(), names.end(), true, [](
        bool acc, const string& name){
            return acc & 
                all_of(
                    name.begin(), 
                    name.end(), 
                    boost::is_upper()
                );});
}           

// flatten vector of vectors of int
vector<int> flat_vec(const vector<vector<int>>& vectors) {
    return accumulate(vectors.begin(), vectors.end(), vector<int>{},
        [](vector<int> acc, const vector<int>& cur) {
            move(cur.begin(), cur.end(), back_inserter(acc));
            return acc;
        });
}

int main() {
    string res = xored_string("ALEX1234", 1);
    cout << " res = " << res << endl;
    string my_reversed = my_reverse("COVID-19");
    cout << "covid reverse: " << my_reversed << endl;
    assert(all_capitals({"WHERE", "IS", "MY", "MIND"}));
    assert(!all_capitals({"WHERE", "IS", "MY", "MiND"}));
    assert(!all_capitals({"WHERE", "IS", "MY", "MIND1"}));
    vector<vector<int>> vs{{1,2,3}, {4,5,6}, {7,8,9}, {10}};
    vector<int> flat = flat_vec(vs);
    vector<int> res_vec{1,2,3,4,5,6,7,8,9, 10};
    assert(flat == res_vec);
    
    return 0;
}
