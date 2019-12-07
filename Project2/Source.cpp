#include "Environment.h"
#include "Geometry.h"
#include "Lagrangian.h"
#include <random>
class Complex {
public:
	double re;
	double im;
	Complex() {
		re = 0;
		im = 0;
	}
	Complex(double re, double im) {
		this->re = re;
		this->im = im;
	}
	Complex operator *(Complex C) {
		return Complex(C.re * re - C.im * im, C.re * im + C.im * re);
	}
};
int main() {
	vector <Object*> OB;
	int n = 3;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (1 << n); j++) {

		}
	}
	OB.push_back(new Object());

	Environment env(0.01,false);
	env.push_back(OB);
	env.simulate(10000);
}