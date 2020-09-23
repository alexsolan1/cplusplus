#include <cassert>
#include <iostream>
#include <string>

template <class T>
class Optional {
public:
	Optional() = default;
	Optional(const T& t) :
		m_value(t),
		m_valid(true)
	{ }
	operator bool() const {
		return m_valid;
	}

	T& get() {
		assert(m_valid);
		return m_value;
	}
		
	const T& get() const {
		assert(m_valid);
		return m_value;
	}
	
private:
	T m_value;
	bool m_valid;
};

