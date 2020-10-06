#include <iostream>
#include <boost/variant.hpp>

using namespace std;

namespace detail {
template <typename Operation> struct BinaryOperation;

struct OperationPlus { };
struct OperationMinus { };
struct OperationMultiply { };
struct OperationPower { };

constexpr OperationPlus operation_plus{};
constexpr OperationMultiply operation_multiply{};
constexpr OperationMinus operation_minus{};
constexpr OperationPower operation_power{};

using func_t = boost::variant<
    char,
    double,
    boost::recursive_wrapper<BinaryOperation<OperationPlus>>,
    boost::recursive_wrapper<BinaryOperation<OperationMinus>>,
    boost::recursive_wrapper<BinaryOperation<OperationMultiply>>,
    boost::recursive_wrapper<BinaryOperation<OperationPower>>
>;

bool is_zero(const func_t& func);
template <class Operation> 
struct BinaryOperation {
BinaryOperation(const func_t& a_lhs, const func_t& a_rhs, Operation a_operation) :
    lhs(a_lhs),
    rhs(a_rhs),
    operation(a_operation) { }

    func_t lhs;
    func_t rhs;
    Operation operation;
};

template <class Obj>
ostream& printTo(ostream& os, const Obj& obj) {
    os << obj;
    return os;
} 


template <>
ostream& printTo<OperationPower>(ostream& os, const OperationPower& obj) {
    os << " ^ ";
    return os;
} 
template <>
ostream& printTo<OperationPlus>(ostream& os, const OperationPlus& obj) {
    os << " + ";
    return os;
} 
template <>
ostream& printTo<OperationMinus>(ostream& os, const OperationMinus& obj) {
    os << " - ";
    return os;
} 
template <>
ostream& printTo<OperationMultiply>(ostream& os, const OperationMultiply& obj) {
    os << " * ";
    return os;
} 


template<class Operation>
ostream& printTo(ostream& os, const BinaryOperation<Operation>& binaryOp) {
    printTo(os, '(');
    printTo(os, binaryOp.lhs);
    printTo<Operation>(os, binaryOp.operation);
    printTo(os, binaryOp.rhs);
    printTo(os, ')');
    return os;
}

struct PrintVisitor {
    PrintVisitor(ostream& os) : m_os(os) { }
    ostream& m_os;
    void operator()(auto& printed) {
        printTo(m_os, printed);
    }
};

ostream& printTo(ostream& os, func_t func) {
    PrintVisitor print_visitor(os);
    boost::apply_visitor(print_visitor, func);
    return os;
}

ostream& operator<<(ostream& os, func_t func) {
    return printTo(os, func);
}

func_t operator+(const func_t& lhs, const func_t& rhs) {
    if (lhs.type() == typeid(double) && rhs.type() == typeid(double)) {
        return get<double>(lhs) + get<double>(rhs);
    }
    return BinaryOperation{lhs, rhs, operation_plus};
}

func_t operator-(const func_t& lhs, const func_t& rhs) {
    return BinaryOperation{lhs, rhs, operation_minus};
}

func_t operator*
(const func_t& lhs, const func_t& rhs) {
    if (lhs.type() == typeid(double) && rhs.type() == typeid(double)) {
        return get<double>(lhs) * get<double>(rhs);
    }
/*
    if ((lhs.type() == typeid(double) && get<double>(lhs) == 0.) ||
        (rhs.type() == typeid(double) && get<double>(rhs) == 0.)) {
        return 0.;
    }*/
    if (is_zero(lhs) || is_zero(rhs)) {
        return 0.;
    }
    return BinaryOperation{lhs, rhs, operation_multiply};
}

func_t operator^(const func_t& lhs, const func_t& rhs) {
    return BinaryOperation{lhs, rhs, operation_power};
}

func_t deriviate(const func_t& func);
struct DeriviateVisitor{
    func_t operator() (char x) {return 1.;}
    func_t operator() (double x) {return 0.;}
    func_t operator() (const BinaryOperation<OperationPlus>& bop) {
        return deriviate(bop.lhs) + deriviate(bop.rhs);
    }

    func_t operator() (const BinaryOperation<OperationMinus>& bop) {
        return deriviate(bop.lhs) + deriviate(bop.rhs);
    }

    func_t operator() (const BinaryOperation<OperationMultiply>& bop) {
            return deriviate(bop.lhs) * bop.rhs + bop.lhs * deriviate(bop.rhs);
    }
    
    func_t operator()(const BinaryOperation<OperationPower>& bop) {
        double power = get<double>(bop.rhs);
        return power * bop.lhs ^ (power - 1) * deriviate(bop.lhs);
    }
};

func_t deriviate(const func_t& func) {
    return boost::apply_visitor(DeriviateVisitor{}, func);
}
struct ZeroVisitor {
    bool operator() (double x) {return x == 0.;}
    bool operator() (const auto&) {return false;}
};

bool is_zero(const func_t& func) {
    return boost::apply_visitor(ZeroVisitor{}, func);
}

} // end of namespace detail

/*
 * Wrapper to func_t
 */

struct Expr : public  detail::func_t {
    
    using base_type = detail::func_t;
    using base_type::base_type;
    Expr(const base_type& base) : base_type(base) { }
    base_type base() const {
        return static_cast<const base_type&>(*this);
    }
    base_type deriviate() {return detail::deriviate(base());}

};


Expr operator+(const Expr& lhs, const Expr& rhs) {
       return lhs.base() + rhs.base();
}

Expr operator-(const Expr& lhs, const Expr& rhs) {
       return lhs.base() - rhs.base();
}

Expr operator*(const Expr& lhs, const Expr& rhs) {
       return lhs.base() * rhs.base();
}

Expr operator^(const Expr& lhs, const Expr& rhs) {
       return lhs.base() ^ rhs.base();
}
