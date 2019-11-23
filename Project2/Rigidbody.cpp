#include "RIgidbody.h"
using namespace std;
//generators
Rigidbody::Rigidbody(){
    m = 1;
    double A[3]={1,1,1};//set (1,1,1)
    Ib = vec(A);
    set_inertia_b();
    rotmat_bi = tensor();//default of tensor is identity mat
    Inertia_i = coor_trans(rotmat_bi,Inertia_b);
}
Rigidbody::Rigidbody(double m0, vec I, tensor R){
    m = m0;
    Ib = I;
    set_inertia_b();
    rotmat_bi = R;
    Inertia_i = coor_trans(rotmat_bi,Inertia_b);
}

//operators
void Rigidbody::operator=(Rigidbody RB){
    m = RB.m;
    Inertia_i = RB.Inertia_i;
    Ib = RB.Ib;
    Inertia_b = RB.Inertia_b;
    rotmat_bi = RB.rotmat_bi;
}

//funcs//
void Rigidbody::set_inertia_b(){
    Inertia_b = tensor();
    for(int i=0 ; i<3 ; i++){
        Inertia_b.M[i][i] = Ib.V[i];
    }
}
void Rigidbody::set_m(double m0){
    m = m0;
}
ostream& operator<<(ostream& output, const Rigidbody& H){
    cout<<"\nPROPERTIES of Rigidbody\n\n";
    cout<<"m = "<<H.m<<"\n\n";
    cout<<"Ib = "<<H.Ib<<"\n";
    cout<<"rotmat_bi = "<<H.rotmat_bi<<"\n";
    cout<<"Inertia_b = \n" << H.Inertia_b<<"\n";
    cout<<"Inertia_i = \n" << H.Inertia_i<<"\n";
    return output;
}
