#include "Tensor.h"
using namespace std;
int main(){
    double A[3][3];
    double B[3];
    double A1[3][3];
    double B1[3];
    double x = 2;

    for(int i=0 ; i<3 ; i++){
        B[i] = i+1;
        B1[i] = i*2;
    }

    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            A[i][j] = i*j;
            A1[i][j] = i-j;
        }
    }
    cout<<"A\n";
    tensor a = tensor(A);
    tensor a1 = tensor(A1);
    vec b = vec(B);
    vec b1 = vec(B1);


    //our vecs && tensors
    cout<<"a,a1,b,b1 = \n";
    cout<<a;
    cout<<a1;
    cout<<b;
    cout<<b1;



    //check vec func && ops;
    cout<<"check vec func && ops\n";

    //check vec +
    cout<<"b+b1\n";
    cout<<(b+b1)<<"\n";

    //check vec -
    cout<<"b-b1\n";
    cout<<(b-b1)<<"\n";
    
    //check vec /(vec B)
    b.V[0]=1;
    b.V[1]=2;
    b.V[2]=3;
    b1.V[0]=1;
    b1.V[1]=10;
    b1.V[2]=9;
    cout<<"b1,b,b1/b\n";
    cout<<b1<<b<<b1/b;

    //check vec / (vec B)
    b.V[0] = 0;
    cout<<"b1,b,b1/b";
    cout<<b1<<b<<b1/b;


    //check vec+=
    cout<<"check vec +=\n";
    vec d=b;
    cout<<d;
    d+=d;
    cout<<d;

    //check vec*vec;
    cout<<"b*b1\n";
    cout<<b*b1<<"\n";

    //check vec*vec;
    cout<<"b1*b\n";
    cout<<b1*b<<"\n";

    //check vec*double;
    cout<<"b*x\n";
    cout<<b*x<<"\n";

    //check double*vec;
    cout<<"x*b\n";
    cout<<x*b<<"\n";




    //check tensor func && ops;


    cout<<"check tensor func && ops\n";

    //check tensor()
    cout<<"tensor() = \n";
    cout<<tensor();

    //check tensor(ph, th, ps);
    cout<<"tensor(0,PI/4,0) = "<<"\n";
    cout<<tensor(0,PI/4,0);

    //check tensor(r->b, ph, th, ps);
    cout<<"r->b, b->r = \n";
    cout<<tensor("r->b", 0, PI/4, 0);
    cout<<tensor("b->r", 0, PI/4, 0);

    //check tensor(double A[3][3])
    cout<<tensor(A);

    //check det(tensor);
    cout<<"det(A), det(A1), det(A*A1) = \n";
    cout<<a.det()<<" "<<a1.det()<<" "<<(a*a1).det()<<"\n";
    cout<<"det(tensor(1,1,1))\n";
    cout<<(tensor(10,1,1)).det()<<"\n";

    //check transpose();
    cout<<"A, A transpose = \n";
    cout<<a1<<a1.transpose();

    //check inverse();
    cout<<"A, A inverse = \n";
    cout<<tensor(1,1,1)<<tensor(1,1,1).inverse();
    cout<<"a, a inverse = \n";
    cout<<a<<a.inverse();

    //check cross();
    cout<<"a1,b1,a.cross(b1) = \n";
    cout<<a1<<b1;
    cout<<cross(a1,b1);
    
    //check cross();
    cout<<"b1,a1,a.cross(b1) = \n";
    cout<<b1<<a1;
    cout<<cross(b1,a1);

    //check =
    cout<<"c = a = \n";
    tensor c = a;
    cout<<c;

    //check ==
    cout<<"a==a, a==a1\n";
    cout<<(a==a)<<" "<<(a==a1)<<"\n";

    //check +
    cout<<"a+a1\n";
    cout<<a+a1<<"\n";

    //check -;
    cout<<"a-a1 = \n";
    cout<<a-a1<<"\n";

    //check * tensor();
    cout<<"tensor(1,1,1)*tensor(1,-1,1) = \n";
    cout<<tensor(1,1,1)*tensor(1,-1,1);

    //check * double;
    cout<<"tensor(1,1,1)*x = \n";
    cout<<tensor(1,1,1)<<tensor(1,1,1)*x;

    //check *vec;
    cout<<"a, b, a*b = \n";
    cout<<a<<b<<a*b;

    //check *coor_trans of tensor
    cout<<"a, R && R*a*R.transpose()\n";
    cout<<a<<tensor(1,1,1)<<coor_trans(tensor(1,1,1), a);
    //error check in coor_trans of tensor
    cout<<"if R is not a rotation matrix\n";
    cout<<coor_trans(a,tensor(1,1,1));

    //check coor_trans of vec (overloading)
    cout<<"b, R && R*a\n";
    cout<<b<<tensor(1,1,1)<<coor_trans(tensor(1,1,1),b);
    
    //check torotmat
    cout<<"check torotmat\n";
    tensor T = tensor(1,1,1);
    cout<<T;
    T.torotmat();
    cout<<T;
    tensor T0 = tensor(1,2,1);
    cout<<T0;
    T0.torotmat();
    cout<<T0;
    cout<<T+T0*0.0001;
    tensor T1 = T+T0*0.0001;
    T1.torotmat();
    cout<<T1;


    return 0;
}