#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <exception>

struct BadOptional : public std::exception {
	const char* what () const throw ()
    {
    	return "Bad access to optional";
    }
};

template <class T>
class Optional {
public:
	Optional() : m_value(nullptr), m_valid(false){
	} 
	
	Optional(const T& t) :
		m_value(new (buf) T(t)),
		m_valid(true)
	{
		std::cout << "In ctor: " <<   std::endl; 
	}


	template<typename... Args>
	Optional(Args&&... args) :
		m_value(new (buf) T(std::forward<Args>(args)...)),
		m_valid(true)
	{ }
	
	~Optional() {
		m_value->~T();
	}
	
	operator bool() const {
		return m_valid;
	}

	T& get() {
		if (!m_valid) {
			throw BadOptional();
		}
		return *m_value;
	}
		
	const T& get() const {
		if (!m_valid) {
			throw BadOptional();
		}
		return *m_value;
	}
	
private:
	alignas(alignof(T)) char buf[sizeof(T)];
	T* m_value;
	bool m_valid;
};

