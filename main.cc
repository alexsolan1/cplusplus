#include <iostream>
#include <string.h>
#include <cassert>

using namespace std;

class String {
public:
    
    String() :
        String("", 0)
    { }

    String(const char* data) :
        String(data, strlen(data))
    { }

    String(const String& s) :
        String(s.data(), s.length())
    { }
    
    String(const char* data, size_t length) {
        alloc_and_copy(data, length);
    }
    
    
    String(String&& s) :
        m_data(s.data()),
        m_capacity(s.capacity()),
        m_length(s.length()) 
    { 
        s.clean();
    }
         
    String& operator=(const String& s) {
         // for x = x, ignoring more complicated cases where len/capacity differ
         if (m_data == s.data()) {
             return *this;
         }
         delete[] m_data;
         alloc_and_copy(s.data(), s.length());
         return *this;
    }
    
    String& operator=(String&& s) {
        // for x = x, ignoring more complicated cases where len/capacity differ
        if (m_data == s.data()) {
            return *this;
        }
        m_data = s.data();
        m_capacity = s.capacity();
        m_length = s.length();
        s.clean();
    }
        
    ~String() {
        delete[] m_data;
    }
    
    void reserve(unsigned capacity) {
        if (capacity < m_capacity) { return; }
        reallocate(capacity);
    }
    
    unsigned int length() const {
        return m_length;
    }

    unsigned int capacity() const {
        return m_capacity;
    }

    char* data() const {
        return m_data;
    }
    
    const char* c_str() const {
        return m_data;
    }
 
    char& operator[] (unsigned int i) const {
        assert(i < length());
        return m_data[i];
    }
    
    String& operator+=(const String& s) {
        unsigned oldlen = length();
        m_length = length() + s.length();
        if (m_length + 1 >= m_capacity) {
            reallocate(m_length + 1);
        }
        add(s, oldlen);
        return *this;
    }
    
    char* begin() {
        return m_data;
    }
     
    char* end() {
        return m_data + length();
    }
   
private:
    
    void alloc_and_copy(const char* data, size_t length) {
        m_capacity = length + 1;
        m_length = length;
        m_data = new char[m_capacity];
        strncpy(m_data, data, length);
        m_data[m_length] = 0;
    }
    
    void reallocate(unsigned length) {
        unsigned m_capacity = length;
        char* tmp;
        try {
            tmp = new char[m_capacity];
        }
        catch (const std::bad_alloc& e) {
            cerr << "Caught bad_alloc " << e.what() << endl;
            throw(e);
        }
                
        strcpy(tmp, m_data);
        tmp[m_length] = 0;
        delete[] m_data;
        m_data = tmp;
    }
    
    void add(const String& s, unsigned location) {
        strcpy(m_data + location, s.data());
        m_data[location + s.length()] = 0;
    }
    
    void clean() {
        m_data = nullptr;
        m_length = 0;
        m_capacity = 0;
    }
    char* m_data;    
    unsigned m_capacity;
    unsigned m_length;
};

ostream& operator<<(ostream& os, const String& str) {
    os << str.c_str();
    return os;
}
    
String create_string() {
    return "ALEX";
}

int main()
{
    String s("Hi Alex");
    cout << s << endl;    
    for(unsigned i = 0; i < s.length(); i++) {
        cout << s[i] << " ";
    }
    cout << endl;
    String s2;
    String s3{};
    cout << "s2 = " << s2 << " s3 = " << s3 << endl;
    
    for (auto x : s) {
        cout << x << " ";
    }
    cout << endl;
    
    cout << "Iterator over empty string: ";
    for (auto x : s3) {
        cout << x << " ";
    }
    cout << endl;
    cout << "Iterator over empty string done. " << endl;
    
    s += String("Have a good one");
    cout << s << endl;
    String small("xxxx");
    String small2("yyyy");
    small2 += small;
    cout << "Small2 = " << small2 << " capacity: " << small2.capacity() << endl;
    
    String my_copy = small2;
    cout << "my_copy = " << my_copy << endl;
    my_copy = s;
    cout << "my_copy = " << my_copy << endl;

    my_copy = my_copy;
    cout << "copy self: my_copy = " << my_copy << endl;
    
    my_copy.reserve(100);
    cout << "my copy after reserve: " << my_copy << " capacity: " << my_copy.capacity() << endl;
    
    // As requested....     
    String s4 = "Hello!";
    s4[s4.length() - 1] = '?';
    assert('?' == s4[s4.length() - 1]);
    
    String s5(create_string());
    cout << "s5(move ctor) " << s5 << endl;
    
    char* data = s5.data();
    data[0] = 'B';
    assert('B' == s5[0]);
    return 0;
}
