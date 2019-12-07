#include "Environment.h"
#include "Geometry.h"
#include "Lagrangian.h"
#include <random>
int main() {
	vector <Object*> OB;

	OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, 0, 4), vec(0.3, 0, 0), vec(0, 0.6, 0), tensor(), vec(0.5, 0.5, 4)));

	for (int i = 1; i < 4 ; i++) {
		OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416)), vec(0, 0, 4*(i+1)), vec(0, 0., 0), vec(0, 0, 0), tensor(), vec(0.5, 0.5, 4)));
	}
	Environment env(0.01,false);
	env.push_back(OB);
	env.simulate(10000);
}