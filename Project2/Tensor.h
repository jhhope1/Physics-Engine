#ifndef TENSOR_H
#define TENSOR_H
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;
const double PI = 3.141592653589793238462643383279;
const double INFIMUM = 1e-9;
class vec{
    public:
    double V[3];

    //generators
    vec();
    vec(double A[3]);
    vec(double a, double b, double c);

    //operators
    void operator =(vec V);
    bool operator ==(vec V);
    vec operator +(vec V);
    vec operator *(vec V);
    vec operator *(double x);
    vec operator -(vec V);
    vec operator /(double x);
    vec operator /(vec V);
    
    void operator +=(vec V);
    void operator *=(vec V);
    void operator *=(double x);
    void operator /=(double x);
	void operator -=(vec V);
	bool operator !=(vec V);

    friend ostream& operator<<(ostream& output, const vec& H);
	
	//funcs
	double norm();
	double normsquare();
	double dot(vec A);
};
vec operator*(double, vec V);

class tensor{
    public: 
    double M[3][3];

    //generators
    tensor();
    tensor(vec A, vec B, vec C);
    tensor(double ph, double th, double ps);//coordinate transformation rot frame -> body frame.
    tensor(string S, double ph, double th, double ps);
    tensor(double A[][3]);
    
    //operators
    void operator =(tensor T);
    bool operator ==(tensor T);
    tensor operator +(tensor T);
    tensor operator *(tensor T);
    tensor operator -(tensor T);
    tensor operator *(double x);
    tensor operator /(double x);
    vec operator * (vec V);
    
    void operator +=(tensor T);
    void operator *=(tensor T);
    void operator -=(tensor T);
    void operator *=(double x);
    void operator /=(double x);

    //funcs
    double det();
    tensor transpose();
    tensor inverse();
    void torotmat();
    //pair<tensor,vec> diag(tensor T);//lambda, (I0,I1,I2), lambda*T*lambda.inv()
    
    friend ostream& operator<<(ostream& output, const tensor& H);

};

vector<vec> operator+ (vector<vec> A, vector <vec> B);

//funcs
void Rotwarning(tensor R);
tensor coor_trans(tensor R, tensor A);/*
R = cood transformation in A's coor to another
*/
vec coor_trans(tensor R, vec A);
tensor operator*(double x, tensor V);
tensor cross(vec V, tensor T);
tensor cross(tensor T, vec V);
#endif