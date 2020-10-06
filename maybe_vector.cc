#include <vector>
#include <string>
#include <iostream>
#include <iterator>

template<typename T> struct is_vector : public std::false_type {};

template<typename T, typename A>
struct is_vector<std::vector<T, A>> : public std::true_type {};

template <typename T>
struct Printer {
    void print(const T& maybe_vector) {
        if constexpr (is_vector<T>::value) {
            std::cout << "I am a vector containing: ";
            std::copy(maybe_vector.begin(), maybe_vector.end(), 
                      std::ostream_iterator<typename T::value_type>(std::cout, ", "));
            std::cout << std::endl;
        }
        else {
           std::cout << "I am not a vector containing: " << maybe_vector;
           std::cout << std::endl;
        }
    }     
};

int main() {
    std::vector<double> v{2.3, 4.5, 2.888};
    Printer<std::vector<double>> printer;
    printer.print(v);
    Printer<std::string> string_printer;
    string_printer.print("ALEXA");
    
    return 0;
}