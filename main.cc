#include <iostream>
#include <string.h>
#include <cassert>

using namespace std;
constexpr unsigned INITIAL_CAPACITY = 16;

// roundup tp next mult of INITIAL_CAPACITY
inline unsigned calc_reserve(unsigned len) {
    if (len % INITIAL_CAPACITY) {
        len += INITIAL_CAPACITY - (len % INITIAL_CAPACITY);
    }
    return len;
}

class String {
public:
    
    String() {
        m_capacity = INITIAL_CAPACITY;
        m_data = new char[m_capacity];
        m_len = 0;
        m_data[m_len] = 0;
    }
    
    String(const char* data, size_t len) {
        m_capacity = calc_reserve(len + 1);
        m_data = new char[m_capacity];
        strncpy(m_data, data, len);
        m_len = len;
        m_data[m_len] = 0;
    }
    
    String(const char* data) {
        m_len = strlen(data);
        m_capacity = calc_reserve(m_len);
        m_data = new char[m_capacity];
        strncpy(m_data, data, m_len);
        m_data[m_len] = 0;
    }
    
    String(const String& s) {
         m_capacity = s.capacity();
         m_data = new char[m_capacity];
         strncpy(m_data, s.data(), s.len());
         m_len = s.len();     
         m_data[m_len] = 0;
    }
    
    String& operator=(const String& s) {
         // for x = x, ignoring more complicated cases where len/capacity differ
         if (m_data == s.data()) {
             return *this;
         }
         delete[] m_data; 
         m_capacity = s.capacity();
         m_data = new char[m_capacity];
         strncpy(m_data, s.data(), s.len());
         m_data[s.len()] = 0;
         m_len = s.len();
         return *this;
     } 
   
    ~String() {
        delete[] m_data;
    }
    
    void reserve(unsigned capacity) {
        if (capacity < m_capacity) { return; }
        m_capacity = capacity;
        char* tmp = new char[m_capacity];
        strcpy(tmp, m_data);
        tmp[m_len] = 0;
        delete[] m_data;
        m_data = tmp;
    }
    
    unsigned int len() const {
        return m_len;
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
 
    char operator[] (unsigned int i) const {
        assert(i < len());
        return m_data[i];
    }
    
    String& operator+=(const String& s) {
        unsigned oldlen = len();
        m_len = len() + s.len();
        if (m_len < m_capacity) {
            // no need to re-alloc
            strcpy(m_data + oldlen, s.data());
            m_data[m_len] = 0;
            return *this;
        }
            
        unsigned m_capacity = calc_reserve(m_len + 1);
        char* tmp = new char[m_capacity];
        strcpy(tmp, m_data);
        strcpy(tmp + oldlen, s.data());
        tmp[m_len] = 0;
        delete[] m_data;
        m_data = tmp;
        return *this;
    }
    
    char* begin() {
        return m_data;
    }
     
     char* end() {
        return m_data + len();
     }
 
   
private:
    char* m_data;    
    unsigned m_capacity;
    unsigned m_len;
};

ostream& operator<<(ostream& os, const String& str) {
    os << str.c_str();
    return os;
}
    

int main()
{
    String s("Hi Alex");
    cout << s << endl;    
    for(unsigned i = 0; i < s.len(); i++) {
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
    return 0;
}
