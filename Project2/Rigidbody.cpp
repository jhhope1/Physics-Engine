#include "RIgidbody.h"
using namespace std;
//generators
Rigidbody::Rigidbody(){
    m = 1;
    double A[3]={1,1,1};//set (1,1,1)
    Ib = vec(A);
    set_inertia_b();
}
Rigidbody::Rigidbody(double m0, vec I){
    m = m0;
    Ib = I;
    set_inertia_b();
}

//operators
void Rigidbody::operator=(Rigidbody RB){
    m = RB.m;
    Ib = RB.Ib;
    Inertia_b = RB.Inertia_b;
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
    cout<<"Inertia_b = \n" << H.Inertia_b<<"\n";
    return output;
}
