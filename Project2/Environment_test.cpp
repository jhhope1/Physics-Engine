#include "Environment.h"
int main(){
    //ios_base::sync_with_stdio(0);cout.tie(0);
    Environment E(0.1);
    Rigidbody RB = Rigidbody(1,vec(1,2,3),tensor());
    Object ob1 = Object(RB, vec(0,0,0),vec(0.01,0,1),vec(),tensor());//marion example (I1, I2, I3)가 있을 때 안정인것, 불안정인것-> 테케 통과
    E.push_back(ob1);
    E.simulate(10);
}