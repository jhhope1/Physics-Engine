#ifndef LAGRANGIAN_H
#define LAGRANGIAN_H
#include "Object.h"
#include <Eigen/Dense>
using namespace Eigen;
const double LAGRANGIAN_DX = 1e-6;

class VV{
public:
	vec Force;
	vec Torque;
	inline VV() {
		Force = vec();
		Torque = vec();
	}
	inline VV(vec Force, vec Torque) {
		this->Force = Force;
		this->Torque = Torque;
	}
};

class Lagrangian {
public:
	static VectorXd qddot_FT(vector<Object*> OB);
	static VectorXd qddot_without_FT(vector<Object*> OB);
};

#endif LAGRANGIAN_H