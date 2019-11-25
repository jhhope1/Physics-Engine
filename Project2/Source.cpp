#include "Environment.h"
#include "Geometry.h"
int main() {
	vector <Object*> OB;
	for (int i = 0; i < 10; i++) {
		OB.push_back(new Object(Rigidbody(1, vec(1.0833, 0.833, 0.416), tensor(i,i,0)), vec(0., 0, i * 5 + 3.5), vec(0.0, 0.0, 0), vec(0., 0., 0.), tensor(), vec(1, 2, 3)));
	}
	//OB.push_back(Object(Rigidbody(1, vec(1, 2, 3), tensor()), vec(0, 0, 5), vec(0, 0.2, 10), vec(0, 0, 0), tensor(),vec(1,2,3)));
	
	Environment env(0.001,true);
	//OB.push_back(Object(Rigidbody(),vec(),vec(0,0,1),vec(0,0,0),tensor()));
	env.push_back(OB);
	env.simulate(100);
	//Object ob1 = Object(Rigidbody(1, vec(1, 2, 3), tensor()), vec(0, 0, 0), vec(0, 0, 0), vec(0, 0, 0), tensor(), vec(1, 2, 3));
	//Object ob2 = Object(Rigidbody(1, vec(1, 2, 3), tensor()), vec(2, 0, 0), vec(0, 0, 0), vec(0, 0, 0), tensor(), vec(1, 2, 3));
	
	/*Triangle A = Triangle(vec(1,0,0),vec(-1,1,0),vec(-1,-1,0));
	Line L = Line(vec(0,0,1), vec(2.1,0,-1));
	bool a = Intersect::triline(A, L);*/
}