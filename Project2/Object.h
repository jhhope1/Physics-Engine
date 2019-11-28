#ifndef OBJECT_H
#define OBJECT_H
#include "Rigidbody.h"
#include "PhyObject.h"
using namespace std;
class Object:public Rigidbody{
    public:
    // Rigidbody에서는 불변량, Object에서는 변량을 instance로 가짐
    vec pos_f;//pos in fixed cart coord.
	vec cubesize;
	cube C;
    vec w_b;
    vec v_f;
    tensor rotmat_if;/*rotmat_in = initial cartesian coordinate -> fixed cartesian coordinate
    I = coor_trans(rotmat_if, coortrans(rotmat_bi, I_b));//notation is not as good in mat mul 
    */
    //shape of Object를 넣고싶은데 수식모양일 것 같아서 string으로 담다가 visualize할때 보여줄지 아니면 새로운 class를 만들어서 박아버릴까 고민
	//일단 지금은 cube로만.
   //generator
    Object();
    Object(Rigidbody RB, vec posf, vec wb, vec vf, tensor rotmatif);
    Object(Rigidbody RB);//initial condition of position,w,v,rotmat are 0,0,0,1
	Object(Rigidbody RB, vec posf, vec wb, vec vf, tensor rotmatif, vec cubesize);

    //ops
    void operator =(Object A);

    //funcs
    vec w_f();
    vec IWW_b();
    void Object_update_pos_rotmat(double dt);
	void Object_update_without_pos_rotmat(vec F_f, vec T_f, double dt);
	double Energy();
	double KineticE();
	vec AngMom_f();
	vec pos_b_pos_f(vec V);//position in b -> position in f
	vec pos_f_vel_f(vec V);//pos in b -> vel in f
	void ObjectPoints(vector <vec>* Pt);

    friend ostream& operator<<(ostream& output, const Object& H);
};
class Objects {
public:
	vector<Object*> OB;
	//generators
	Objects(vector<Object*> OB);

	//functions
	vector <double> f();//function pointer를 가지고 잘 놀아보면 Objects에서 f를 외부에서 가지고 놀 수 있을 것 같은데 어떻게 해야될지는 아직 모르겠음.
};
#endif