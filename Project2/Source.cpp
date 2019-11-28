#include "Environment.h"
#include "Geometry.h"
#include "Lagrangian.h"
int main() {
	vector <Object*> OB;

	OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, 0, 0), vec(20, 0, 0), vec(0, 20*1.5, 0), tensor(), vec(0.5, 0.5, 3)));

	for (int i = 1; i < 3 ; i++) {
		OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, 0, 3*i), vec(0, 0, 0), vec(0, 0, 0), tensor(), vec(0.5, 0.5, 3)));
	}
	vector<VV> A;
	for (int i = 0; i < OB.size(); i++) {
		A.push_back(VV(vec(),vec()));
	}

	Environment env(0.001,false);
	env.push_back(OB);
	env.simulate(10000);
}