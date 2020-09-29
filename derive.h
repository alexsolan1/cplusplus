#include <iostream>
#include <cassert>
#include <memory>
#include <optional>

using std::ostream;
using std::shared_ptr;
using std::optional;

namespace detail {
enum Operand {
	plus, 
	minus,
	mult,
	power
};


ostream& operator<<(ostream& os, const Operand& operand) {
	switch (operand) {
		case plus: os << "+"; break;
		case minus: os << "-"; break;
		case mult: os << "*"; break;
		case power: os << "^"; break;
		default: assert(0); 
	}
	return os;
}

class Expression {
public:
	virtual bool is_zero() { return true; }
	virtual ostream& printTo(ostream& os) const {
		os << "";
		return os;
	}
	virtual shared_ptr<Expression> derivative() const {
		return shared_ptr<Expression>{};
	}
protected:
	friend class BinaryExpression;
	virtual std::optional<char> opt_var() const { return optional<char>{}; }
	Expression() = default;
};

ostream& operator<<(ostream& os, const Expression& ex) {
	return ex.printTo(os);
}

class CoefExpression : public Expression {
public:
	CoefExpression(double coef): m_coef(coef) 
	{ }
	
	virtual bool is_zero() { return coef() == 0.; }
	virtual shared_ptr<Expression> derivative() const {
		return std::dynamic_pointer_cast<Expression>(
			shared_ptr<CoefExpression>(new CoefExpression{0})
		);
	}

	virtual ostream& printTo(ostream& os) const {
		os << coef();
		return os;
	}
	double coef() const { return m_coef;}
	friend class BinaryExpression;
protected:
	virtual optional<char> opt_var() const { return optional<char>{}; }
	double m_coef;
};


class XExpression : public Expression {
public:
	virtual ostream& printTo(ostream& os) const {
		os << x();
		return os;
	}

	virtual bool is_zero() { return false; }
	virtual shared_ptr<Expression> derivative() const {
		return std::dynamic_pointer_cast<Expression>(
			shared_ptr<CoefExpression>(new CoefExpression{1})
		);
	}
	char x() const { return m_x; }
	XExpression(char x) { m_x = x; }
	friend class BinaryExpression;
protected:
	virtual optional<char> opt_var() const { return optional<char>{m_x}; }
	char m_x;
};

class BinaryExpression;
shared_ptr<Expression> make_shared_expression(BinaryExpression* ptr);
shared_ptr<Expression> make_shared_expression(CoefExpression* ptr);
shared_ptr<Expression> multiply(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs);
shared_ptr<Expression> add(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs);
shared_ptr<Expression> pow(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs);
shared_ptr<Expression> subtract(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs);

class BinaryExpression : public Expression {
public:
	virtual ostream& printTo(ostream& os) const {
		os << "(";
		lhs->printTo(os);
	 	os << operand;
		rhs->printTo(os);
		os << ")";
		return os;
	}

	virtual bool is_zero() {
		switch (operand) {
			case plus: 
				return lhs->is_zero() && rhs->is_zero();
			case minus: 
				return lhs->is_zero() && rhs->is_zero();
			case mult: 
				return lhs->is_zero() || rhs->is_zero();
			case power:
				return lhs->is_zero();
		}
		assert(0);
		return false;
	}

	

	BinaryExpression(shared_ptr<Expression> a_lhs, shared_ptr<Expression> a_rhs, Operand a_operand):
		lhs(a_lhs),
		rhs(a_rhs),
		operand(a_operand)
	{ 
		opt_var();
	}
	
	virtual shared_ptr<Expression> derivative() const {
		switch (operand) {
			case plus: 
				return add(lhs->derivative(), rhs->derivative());
			case minus: 
				return subtract(lhs->derivative(), rhs->derivative());
			case mult: 
				return add(
					multiply(lhs->derivative(), rhs),
					multiply(rhs->derivative(), lhs)
				);
			case power: 
				{
					shared_ptr<CoefExpression> coefExp = std::dynamic_pointer_cast<CoefExpression>(rhs);
					assert(coefExp);
					auto be1 = multiply(
						rhs,
						pow(
							lhs, 
							make_shared_expression(
								new CoefExpression(coefExp->coef() - 1)
							)
						)
					);
					return multiply(be1, lhs->derivative());
				}
			default: assert(0); 
		}
	}

protected:

	virtual optional<char> opt_var() const { 
		if (!lhs->opt_var()) {
			return rhs->opt_var();
		}

		if (!rhs->opt_var()) {
			return lhs->opt_var();
		}
	
		assert(lhs->opt_var() == rhs->opt_var());
		return lhs->opt_var();
	}
	shared_ptr<Expression> lhs;
	shared_ptr<Expression> rhs;
	Operand operand;
};

shared_ptr<Expression> make_shared_expression(CoefExpression* ptr) {
	return std::dynamic_pointer_cast<Expression>(
		shared_ptr<CoefExpression>(ptr)
	);
}

shared_ptr<Expression> make_shared_expression(BinaryExpression* ptr) {
	return std::dynamic_pointer_cast<Expression>(
		shared_ptr<BinaryExpression>(ptr)
	);
}

shared_ptr<Expression> add(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs) {
	auto coef_left = std::dynamic_pointer_cast<CoefExpression>(lhs);
	auto coef_right = std::dynamic_pointer_cast<CoefExpression>(rhs);
	if (coef_left && coef_right) {
		return make_shared_expression(
			new CoefExpression{coef_left->coef() + coef_right->coef()}
		); 
	}

	if (lhs->is_zero()) {
		return rhs;
	}
	
	if (rhs->is_zero()) {
		return lhs;
	}
	
	return make_shared_expression(
		new BinaryExpression{lhs, rhs, plus}
	);
}

shared_ptr<Expression> subtract(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs) {
	auto coef_left = std::dynamic_pointer_cast<CoefExpression>(lhs);
	auto coef_right = std::dynamic_pointer_cast<CoefExpression>(rhs);
	if (coef_left && coef_right) {
		return make_shared_expression(
			new CoefExpression{coef_left->coef() - coef_right->coef()}
		); 
	}

	if (rhs->is_zero()) {
		return lhs;
	}
	
	return make_shared_expression(
		new BinaryExpression{lhs, rhs, minus}
	);
}

shared_ptr<Expression> multiply(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs) {
	auto coef_left = std::dynamic_pointer_cast<CoefExpression>(lhs);
	auto coef_right = std::dynamic_pointer_cast<CoefExpression>(rhs);
	if (coef_left && coef_right) {
		return make_shared_expression(
			new CoefExpression{coef_left->coef() * coef_right->coef()}
		); 
	}
	if (
		rhs->is_zero() || lhs->is_zero()
	) {
		return make_shared_expression(
			new CoefExpression{0.}
		); 
	}

	return make_shared_expression(
		new BinaryExpression{lhs, rhs, mult}
	);
}

shared_ptr<Expression> pow(shared_ptr<Expression> lhs, shared_ptr<Expression> rhs) {
	return make_shared_expression(
		new BinaryExpression{lhs, rhs, power}
	);
}

} // End of namespace detail

class Func {
public:
	Func(double coef) :
		m_expr(std::dynamic_pointer_cast<detail::Expression>(
			shared_ptr<detail::CoefExpression>(new detail::CoefExpression{coef})
		))
	{ }	

	Func(char x):
		m_expr(std::dynamic_pointer_cast<detail::Expression>(
			shared_ptr<detail::XExpression>(new detail::XExpression{x})
		))
	{ }	

	Func(Func&& func) : 
		m_expr(func.m_expr) {
		func.m_expr = nullptr;
	}
	Func derivative() {
		return Func(m_expr->derivative());
	}
private:
	Func (shared_ptr<detail::Expression> expr) : m_expr(expr)
	{ } 
	shared_ptr<detail::Expression> m_expr;

	friend Func operator+(const Func& lhs, const Func& rhs);
	friend Func operator-(const Func& lhs, const Func& rhs);
	friend Func operator*(const Func& lhs, const Func& rhs);
	friend Func pow(const Func& lhs, const Func& rhs);
	friend ostream& operator<<(ostream& os, const Func& func);
};

Func operator+(const Func& lhs, const Func& rhs) {
	return Func(add(lhs.m_expr, rhs.m_expr));
}

Func operator-(const Func& lhs, const Func& rhs) {
	return Func(subtract(lhs.m_expr, rhs.m_expr));
}

Func operator*(const Func& lhs, const Func& rhs) {
	return Func(multiply(lhs.m_expr, rhs.m_expr));
}

Func pow(const Func& lhs, const Func& rhs) {
	return Func(pow(lhs.m_expr, rhs.m_expr));
}
ostream& operator<<(ostream& os, const Func& func) {
	return func.m_expr->printTo(os);
}

