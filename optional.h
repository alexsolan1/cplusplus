#include <cassert>
#include <iostream>
#include <string>
#include <variant>

template <class T>
class Optional {
public:
	Optional() : m_value(nullptr), m_valid(false){
	} 
	
	Optional(const T& t) :
		m_value(new (buf) T(t)),
		m_valid(true)
	{ }

	~Optional() {
		m_value->~T();
	}
	
	operator bool() const {
		return m_valid;
	}

	T& get() {
		assert(m_valid);
		return *m_value;
	}
		
	const T& get() const {
		assert(m_valid);
		return *m_value;
	}
	
private:
	char buf[sizeof(T)];
	T* m_value;
	bool m_valid;
};

