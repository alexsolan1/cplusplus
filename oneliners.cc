#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <sstream>

using namespace std;

int main()
{
    vector<int> v(100);
    iota(v.begin(), v.end(), 0);
    int num_of_4s = count_if(v.begin(), v.end(), [](int x){return x == 4;});
    cout << "Number of 4s in vector: " << num_of_4s << endl;
    int num_of_bigger_4 = count_if(v.begin(), v.end(), [](int x){return x > 4;});
    cout << "Number of bigger than 4 in vector: " << num_of_bigger_4 << endl;
    stringstream s;
    v.erase(remove_if(v.begin(), v.end(), [](int x){return x < 8;}), 
            v.end());
    cout << "After removal: " << endl;
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    vector<int> v2;
    transform(v.begin() + 1, v.end(), v.begin(), back_inserter(v2), std::minus<int>{});
    cout << endl << " minuses " << endl;
    copy(v2.begin(), v2.end(), ostream_iterator<int>(cout, " "));
    cout << endl << "sum of minuses " << accumulate(v2.begin(), v2.end(), 0) << endl;
    return 0;
}