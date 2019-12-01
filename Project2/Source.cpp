#include "Environment.h"
#include "Geometry.h"
#include "Lagrangian.h"
int main() {
	vector <Object*> OB;

	//OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, 0, 0), vec(0, 0, 0), vec(0, 0, 0), tensor(), vec(1, 2, 3)));

	for (int i = 1; i < 2 ; i++) {
		OB.push_back(new Object(Rigidbody(1, vec(1, 1, 3)), vec(0, 0, 4*i), vec(0.2, 0., 1), vec(0, -0.2, 0), tensor(), vec(1, 2, 3)));
	}
	Environment env(0.001,true);
	env.push_back(OB);
	env.simulate(10000);
}