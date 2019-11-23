#include "Object.h"
int main(){
    Object OB;
    Rigidbody RB = Rigidbody(2, vec(1,2,3), tensor(0.01,0.01,0.01));
    OB = Object(RB);
    cout<<OB.Ib;
    Object OB1 = OB;//오비원?엌ㅋ.ㅋ.ㅋ
    cout<<OB1.Ib;
    cout<<&OB1.Ib<<"\n";
    cout<<&OB.Ib<<"\n";//ㅇㅋ 포인터 같다고 해도 다른듯?
    

}