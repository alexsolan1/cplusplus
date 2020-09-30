#include "derive.h"

using namespace std;
int main() {
	Func f('x');
	Func x('x');
	Func y('y');
	Func y_func = y * 3.;
	//Func mixed = x * y; // Fails on assert
	Func h = f * 5.;
	Func const1(23.);
	Func const2(2.);
	Func mult_consts = const1 * const2;
	cout << mult_consts << endl;
	cout << "h=" << h << "derivative: " << h.derivative() << endl;
	Func long_func = 2. * h + 5. * pow((f + 20.), 2.);
	cout << "long_func = " << long_func << "derivative: " << long_func.derivative() << endl;
	Func g = f*0.;
	cout << "g(x)=" << g << " g'(x) " << g.derivative() << endl;	
	Func f_mult_g = f * g;
	cout << "h is: " << h << " derivative is: " << h.derivative() << endl;
	cout << "f_mult_g is: " << f_mult_g << " derivative is: " << f_mult_g.derivative() << endl;
	Func sub_func = f * 3. - x;
	cout << "sub_func(x) = " << sub_func << " sub_func'(x) = " << sub_func.derivative() << endl;
	Func zero_func = 0. * mult_consts * h;
	cout << "zero_func(x) = " << zero_func << " zero_func'(x) = " << zero_func.derivative() << endl;
	Func small_coeff = 0.000001 * mult_consts * h;
	cout << "small_coeff(x) = " << small_coeff << " small_coeff'(x) = " << small_coeff.derivative() << endl;
	return 0;
}
