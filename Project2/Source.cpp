#include "Environment.h"
#include "Geometry.h"
#include "Lagrangian.h"
int main() {
	vector <Object*> OB;
	OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, 0, 3.5), vec(0.5, 0, 0), vec(0, 0.5 * 1.5, 0), tensor(), vec(1, 2, 3)));
	for (int i = 1; i < 10 ; i++) {
		OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, i * 4, 3.5), vec(0.5*sin(i), 0, 0), vec(0, 0.5 * sin(i)*1.5, 0), tensor(), vec(1, 2, 3)));
	}

	vector<VV> A;
	for (int i = 0; i < OB.size(); i++) {
		A.push_back(VV(vec(),vec()));
	}

	Environment env(0.01,false);
	env.push_back(OB);
	env.simulate(10000);
}