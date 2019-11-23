#include"Geometry.h"
#include "Object.h"
int main() {
	Object ob = Object(Rigidbody(),vec(),vec(),vec(),tensor());
	tensor t(1, 1, 1);
	Triangle T(t*vec(0, 0, 0), t*vec(1, 0, 0), t*vec(0, 1, 0));
	Line L(t*vec(0, 1, 1), t*vec(0, 2, 2));
	vec P(0.25, 0.25, 0.49999999999999);
	P = t * P;
	vec cubecenter(0,0,1);
	cubecenter = t * cubecenter;
	BVV B = Intersect::tripoint(cubecenter, T, P);
	cout<<B;


}