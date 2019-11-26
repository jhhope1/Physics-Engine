#include "Environment.h"
#include "Geometry.h"
int main() {
	vector <Object*> OB;

	for (int i = 0; i < 10; i++) {
		OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416), tensor(i,i,0)), vec(0., 0, i * 5 + 3.5), vec(0.0, 0.0, 0), vec(0., 0., 0.), tensor(), vec(1, 2, 3)));
	}

	Environment env(0.01,false);
	env.push_back(OB);
	env.simulate(100);
}