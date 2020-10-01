#include "derive2.h"

int main() {
    /*
    func_t f('x');
    func_t g(5.);
    func_t constanta = 0.;
    func_t mult_const = g * constanta;
    func_t add_const = g + constanta;
    cout << "mult_const " << mult_const << "add_const " << add_const << endl;
    func_t sum = f+g;
    func_t h('x');
    func_t multi = h * sum;

    cout << sum << endl;
    cout << multi << " multi' " << deriviate(multi) << endl;
    func_t long_fn = (h ^ g) * 2. + sum + 1.3;
    func_t der = deriviate(sum);
    cout << "long_fn " << long_fn << " long_fn' " << deriviate(long_fn) << endl;
    cout << der << endl;
*/


    Expr exp1(5.);
    cout << "exp1 " << exp1 << "exp1'" << exp1.deriviate() << endl;  
    Expr sum = exp1 + 'x';
    cout << "sum = " << sum << "sum' = " << sum.deriviate ()<< endl;
    Expr f1 = sum * 'x';
    cout << "f1 = " << f1 << " f1' = " << f1.deriviate() << endl;
    return 0;
}
