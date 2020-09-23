#include <iostream>
#include <cstring>
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
    
    String(unsigned capacity) :
        m_data(new char[capacity]),
        m_capacity(capacity),
        m_length(0)
    { }
    
    String(String&& s) :
        m_data(s.m_data),
        m_capacity(s.capacity()),
        m_length(s.length()) 
    { 
        s.clean();
    }
         
    String& operator=(String s) {
        swap(*this, s);
        return *this;
    }
    
    String& operator=(String&& s) {
        // for x = x, ignoring more complicated cases where len/capacity differ
        if (m_data == s.data()) {
            return *this;
        }
        m_data = s.m_data;
        m_capacity = s.capacity();
        m_length = s.length();
        s.clean();
    }
        
    ~String() {
        delete[] m_data;
    }
    
    void reserve(unsigned capacity) {
        if (capacity <= m_capacity) { return; }
        reallocate(capacity);
    }
    
    unsigned int length() const {
        return m_length;
    }

    unsigned int capacity() const {
        return m_capacity;
    }

    const char* data() const {
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
        unsigned newlen = length() + s.length();
        if (newlen + 1 > m_capacity) {
            String tmp(newlen+1);
            tmp.unsafe_add(m_data, 0);
            tmp.unsafe_add(s.m_data, length());
            swap(*this, tmp);
            return *this;
        }
        // no need for new allocation, capacity doesn't change
        unsigned oldlen = length();
        m_length = newlen;
        unsafe_add(s, oldlen);
        return *this;
    }
    
    char* begin() {
        return m_data;
    }
     
    char* end() {
        return m_data + length();
    }
   
private:
    friend void swap(String& first, String& second) {
        using std::swap;
        swap(first.m_capacity, second.m_capacity);
        swap(first.m_length, second.m_length);
        swap(first.m_data, second.m_data);
    }
    
    void alloc_and_copy(const char* data, size_t length) {
        char* tmp = new char[length + 1];  
        m_capacity = length + 1;
        m_length = length;
        m_data = tmp;
        strncpy(m_data, data, length);
        m_data[m_length] = 0;
    }
    
    void reallocate(unsigned capacity) {
        String tmp(capacity);
        tmp.m_length = m_length;
        tmp.unsafe_add(*this, 0);
        swap(*this, tmp);
    }
    
    void unsafe_add(const String& s, unsigned location) {
        assert(location + s.length() + 1 <= m_capacity); 
        strcpy(m_data + location, s.data());
        m_data[location + s.length()] = 0;
        m_length = location + s.length();
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
    cout << "Small2 = " << small2 << " capacity: " << small2.capacity() << " len " << small2.length() << endl;
    
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
    
    const char* data = s5.data();
    // data[0] = 'B'; // Does not compile
    return 0;
}
