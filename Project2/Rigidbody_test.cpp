#include "Rigidbody.h"
int main(){
    //check Rigidbody()
    Rigidbody R;
    cout<<R;
    
    //check Rigidbody(2, vec, tensor)
    R = Rigidbody(2, vec(1,2,3), tensor(PI/2,PI/2,0));
    cout<<R;

    R.set_m(100);
    cout<<R;
}