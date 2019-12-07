#include <iostream>
#include <Eigen/Dense>
#define PI 3.14159265359
using namespace Eigen;
using namespace std;

int main()
{
	MatrixXd m1 = MatrixXd(8, 8);
	MatrixXd m2, m3;

	//8*8행렬 m1에 값을 넣어줍니다.
	double* alpha = new double[8];
	for (int i = 0; i < 8; i++) {
		if (i == 0)
			alpha[i] = 1. / (2. * sqrt(2));
		else {
			alpha[i] = 0.5;
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m1(i, j) = alpha[i] * cos((PI * (2. * j + 1) * i) / 16);
		}
	}
	delete alpha;

	m2 = m1.transpose();//m2에 m1을 transpose한 값을 넣어줍니다.
	m3 = m1 * m2;//m3에 m1과 m2를 곱한 값을 넣어줍니다.
	cout << "m1" << endl << m1 << endl << "m2" << endl << m2 << endl << "m3" << endl << m3 << endl;//출력!
	return 0;
}

