#ifndef RIGID_BODY
#define RIGID_BODY
#include "Tensor.h"
using namespace std;
class Rigidbody{
    public:
    double m;
    tensor Inertia_i;//tensor in initial cartesian coordinate
    vec Ib;
    tensor Inertia_b;
    tensor rotmat_bi;/*body -> initial cartetian coordinate
    Inertia 
    = R.inv() Inertia0 R 
    = I in initial coordinate 
    = cartesian coordinate (when t=0) (nor body frame(I0), lab frame)*/
    
    //generator
    Rigidbody();
    Rigidbody(double m0, vec I, tensor R);

    //operators
    void operator= (Rigidbody A);

    //funcs
    void set_m(double m0);
    void set_inertia_b();

    friend ostream& operator<<(ostream& output, const Rigidbody& H);
};
#endif