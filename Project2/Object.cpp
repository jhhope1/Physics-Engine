#include "Object.h"
using namespace std;

//generators
Object::Object(Rigidbody RB, vec posf, vec wb, vec vf, tensor rotmatif, vec cubesize) {
	Rigidbody& thisRB = *this;
	thisRB = RB;
	this->cubesize = cubesize;
	C = cube(cubesize);
	pos_f = posf;
	w_b = wb;
	v_f = vf;
	rotmat_if = rotmatif;
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
    rotmat_if = A.rotmat_if;
    Rigidbody &B = *this;
    Rigidbody &C = A;
    B = C;
}

vec Object::w_i(){
    vec wi = coor_trans(rotmat_bi,w_b);
    return wi;
}
vec Object::w_f(){
    vec wn = coor_trans(rotmat_if,w_i());
    return wn;
}
vec Object::IWW_b(){
    double a[3];
    a[0] = (Ib.V[1]-Ib.V[2])*w_b.V[1]*w_b.V[2];
    a[1] = (Ib.V[2]-Ib.V[0])*w_b.V[2]*w_b.V[0];
    a[2] = (Ib.V[0]-Ib.V[1])*w_b.V[0]*w_b.V[1];
    return vec(a);
}
void Object::Object_update(vec F_f, vec T_f, double dt){
    v_f += F_f*dt/m;
    pos_f += v_f*dt;
    rotmat_if *= (tensor()+cross(w_i(),tensor())*dt);//cross 함수는 헷갈린다. 정의를 바꿨다..
    // if th, ph, ps -> booooom(th = 0), 터지는 것 방지를 위해 quarternion 기법 써야할듯
    rotmat_if.torotmat();
    tensor rotmat_fi = rotmat_if.transpose();
    tensor rotmat_ib = rotmat_bi.transpose();

    vec T_b = coor_trans(rotmat_ib,coor_trans(rotmat_fi,T_f));

    w_b+=(IWW_b()+T_b)/Ib * dt;
}
double Object::Energy() {
	double E = 0.;
	E += 0.5*m * v_f.dot(v_f);
	E += 0.5 * w_f().dot( coor_trans(rotmat_if, Inertia_i)*w_f());
	E += 9.81 * m * pos_f.V[2];
	return E;
}

vec Object::AngMom_f() {
	vec L;
	L = coor_trans(rotmat_if, Inertia_i)*w_f();
	return L;
}


vec Object::pos_b_pos_f(vec V) {
	return pos_f + rotmat_if * rotmat_bi * V;
}
vec Object::pos_f_vel_f(vec V) {
	return v_f + w_f() * (V - pos_f);
}
ostream& operator<<(ostream& output, const Object& H){
    /*cout<<"PROPERTIES of Object\n\n";
    cout<<"pos_f = ";
    cout<<H.pos_f;
    cout<<"w_b = ";*/
    cout<<H.w_b;
    /*cout<<"v_f = ";
    cout<<H.v_f;
    cout<<"rotmat_if = ";
    cout<<H.rotmat_if;*/
    return output;
}
