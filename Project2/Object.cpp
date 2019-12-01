#include "Object.h"
using namespace std;

//generators
Object::Object(Rigidbody RB, vec posf, vec wb, vec vf, tensor rotmatif, vec cubesize) {
	Rigidbody& thisRB = *this;
	thisRB = RB;
	this->cubesize = cubesize;
	C = top();
	pos_f = posf;
	w_b = wb;
	v_f = vf;
	rotmat_bf = rotmatif;
}
Object::Object(Rigidbody RB, vec posf, vec wb, vec vf, tensor rotmatif){
	*this = Object(RB, posf, wb, vf, rotmatif, vec(1,1,1));
}
Object::Object(Rigidbody RB){
    *this = Object(RB, vec(),vec(),vec(),tensor());
}
Object::Object(){
}

//ops
void Object::operator =(Object A){
    pos_f = A.pos_f;
    w_b = A.w_b;
    v_f = A.v_f;
	cubesize = A.cubesize;
    rotmat_bf = A.rotmat_bf;
    Rigidbody &B = *this;
    Rigidbody &C = A;
    B = C;
}

vec Object::w_f(){
    vec wn = coor_trans(rotmat_bf,w_b);
    return wn;
}
vec Object::IWW_b(){
    double a[3];
    a[0] = (Ib.V[1]-Ib.V[2])*w_b.V[1]*w_b.V[2];
    a[1] = (Ib.V[2]-Ib.V[0])*w_b.V[2]*w_b.V[0];
    a[2] = (Ib.V[0]-Ib.V[1])*w_b.V[0]*w_b.V[1];
    return vec(a);
}
void Object::Object_update_pos_rotmat(double dt) {
	pos_f += v_f * dt;
	rotmat_bf *= tensor::fromaxis(w_b * dt);//cross 함수는 헷갈린다. 정의를 바꿨다..
	// if th, ph, ps -> booooom(th = 0), 터지는 것 방지를 위해 quarternion 기법 써야할듯
	rotmat_bf.torotmat();
}

void Object::Object_update_without_pos_rotmat(vec F_f, vec T_f, double dt) {
	v_f += F_f * dt / m;
	tensor rotmat_fi = rotmat_bf.transpose();

	vec T_b = coor_trans(rotmat_fi, T_f);

	w_b += ((T_b) / Ib) * dt;
}
double Object::Energy() {
	double E = 0.;
	E += 0.5 * m * v_f.dot(v_f);
	E += 0.5 * w_f().dot(coor_trans(rotmat_bf, Inertia_b) * w_f());
	E += 9.81 * m * pos_f.V[2];
	return E;
}
double Object::KineticE() {
	double E = 0.;
	E += 0.5 * m * v_f.dot(v_f);
	E += 0.5 * w_f().dot(coor_trans(rotmat_bf, Inertia_b) * w_f());
	return E;
}
 
vec Object::AngMom_f() {
	vec L;
	L = coor_trans(rotmat_bf, Inertia_b)*w_f();
	return L;
}


vec Object::pos_b_pos_f(vec V) {
	return pos_f + rotmat_bf * V;
}
vec Object::pos_f_vel_f(vec V) {
	return v_f + w_f() * (V - pos_f);
}

void Object::ObjectPoints(vector <vec>* Pt) {
	vector <vec> ckedvec;
	vector <vec> Vertexes;
	for (int k = 0; k < C.indnum; k++) {
		vec V;
		for (int w = 0; w < 3; w++) {
			V.V[w] = C.vertices[C.indices[k] * 6 + w];
		}
		Vertexes.push_back(pos_b_pos_f(V));
		if (Vertexes.size() % 3 == 0 && Vertexes.size() > 0) {
			vec tricm = (Vertexes[Vertexes.size() - 1] + Vertexes[Vertexes.size() - 2] + Vertexes[Vertexes.size() - 3]) / 3;
			Pt->push_back(tricm);
		}
		if (find(ckedvec.begin(), ckedvec.end(), V) != ckedvec.end())continue;
		ckedvec.push_back(V);
		Pt->push_back(pos_b_pos_f(V));
	}
	return;
}

ostream& operator<<(ostream& output, const Object& H){
    cout<<"PROPERTIES of Object\n\n";
    cout<<"pos_f = ";
    cout<<H.pos_f;
    cout<<"w_b = ";
    cout<<H.w_b;
    cout<<"v_f = ";
    cout<<H.v_f;
    cout<<"rotmat_bf = ";
    cout<<H.rotmat_bf;
    return output;
}
