#include "Tensor.h"
using namespace std;
//Class vec

//generator
vec::vec(){
    for(int i=0 ; i<3 ; i++)this->V[i]=0.;
}
vec::vec(double A[3]){
    for(int i=0 ; i<3 ; i++){
        V[i]=A[i];
    }
}
vec::vec(double a, double b, double c){
    V[0] = a;
    V[1] = b;
    V[2] = c;
}

//operators
void vec::operator=(vec A){
    for(int i=0 ; i<3 ; i++)V[i] = A.V[i];
}
bool vec::operator==(vec A){
    for(int i=0 ; i<3 ; i++)if(V[i] != A.V[i])return false;
    return true;
}

vec vec::operator*(double x){
    double A[3]={0};
    for(int i=0 ; i<3 ; i++){
        A[i] = V[i]*x;
    }
    return vec(A);
}
vec vec::operator/(double x){
    if(x==0){
        cout<<"warning: devide 0 in vec operator /\n";
        return vec();
    }
    double A[3]={0};
    for(int i=0 ; i<3 ; i++){
        A[i] = V[i]/x;
    }
    return vec(A);
}
vec vec::operator*(vec B){
    double A[3];
    A[0] = V[1]*B.V[2]-V[2]*B.V[1];
    A[1] = V[2]*B.V[0]-V[0]*B.V[2];
    A[2] = V[0]*B.V[1]-V[1]*B.V[0];
    return vec(A);
}
vec vec::operator+(vec B){
    double A[3];
    for(int i=0 ; i<3 ; i++)A[i] = V[i]+B.V[i];
    return vec(A);
}
vec vec::operator-(vec B){
    double A[3];
    for(int i=0 ; i<3 ; i++)A[i] = V[i]-B.V[i];
    return vec(A);
}
vec vec::operator/(vec B){
    double A[3];
    for(int i=0 ; i<3 ; i++){
        if(B.V[i]==0){
            cout<<"warning: devide 0 in vec / vec\n";
            continue;
        }
        A[i] = V[i]/B.V[i];
    }
    return vec(A);
}
vec operator *(double x, vec A){
    return A*x;
}
    
void vec::operator +=(vec B){
    *this = *this+B;
}
void vec::operator *=(vec B){
    *this = *this*B;
}
void vec::operator *=(double x){
    *this = *this*x;
}
void vec::operator /=(double x){
    *this = *this/x;
}
void vec::operator -=(vec B){
    *this = *this-B;
}
bool vec::operator !=(vec V) {
	return !(*this == V);
}


ostream& operator<<(ostream& output, const vec& H){
    cout<<"vec(";
    cout<<H.V[0]<<", "<<H.V[1]<<", "<<H.V[2];
    cout<<")\n";
    return output;
}

double vec::norm() {
	double norm = 0;
	for (int i = 0; i < 3; i++) {
		norm += V[i] * V[i];
	}
	return sqrt(norm);
}

double vec::normsquare(){
	double normsq = 0;
	for (int i = 0; i < 3; i++) {
		normsq += V[i] * V[i];
	}
	return normsq;
}

double vec::dot(vec A) {
	double re = 0;
	for (int i = 0; i < 3; i++) {
		re += A.V[i] * V[i];
	}
	return re;
}



tensor::tensor(){//set default tensor() = 1
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            M[i][j]=(i==j)?1:0;
        }
    }
}
tensor::tensor(vec A, vec B, vec C){
    for(int i=0 ; i<3 ; i++){
        M[0][i] = A.V[i];
        M[1][i] = B.V[i];
        M[2][i] = C.V[i];
    }
}
tensor::tensor(double A[][3]){
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            M[i][j] = A[i][j];
        }
    }
}
tensor::tensor(double ph, double th, double ps) {//coordinate transformation rot frame -> body frame.
	double A[3][3] = { 0 }, B[3][3] = { 0 }, C[3][3] = { 0 };
	A[0][0] = cos(ph);
	A[0][1] = sin(ph);
	A[1][0] = -sin(ph);
	A[1][1] = cos(ph);
	A[2][2] = 1;

	B[0][0] = 1;
	B[1][1] = cos(th);
	B[1][2] = sin(th);
	B[2][1] = -sin(th);
	B[2][2] = cos(th);

	C[0][0] = cos(ps);
	C[0][1] = sin(ps);
	C[1][0] = -sin(ps);
	C[1][1] = cos(ps);
	C[2][2] = 1;
	tensor D = tensor(C) * tensor(B) * tensor(A);
	*this = D.transpose();
}
tensor::tensor(int n, double th) {//coordinate transformation (')body -> body
	//1베보다 0베가 나으면 나중에 수정.-> 0베로 수정됨
	double A[3][3] = { 0 };
	if (n == 0) {
		A[0][0] = 1;
		A[1][1] = cos(th);
		A[1][2] = -sin(th);
		A[2][2] = cos(th);
		A[2][1] = sin(th);
	}
	if (n == 1) {
		A[1][1] = 1;
		A[0][0] = cos(th);
		A[0][2] = sin(th);
		A[2][2] = cos(th);
		A[2][0] = -sin(th);
	}
	if (n == 2) {
		A[2][2] = 1;
		A[0][0] = cos(th);
		A[0][1] = -sin(th);
		A[1][1] = cos(th);
		A[1][0] = sin(th);
	}
	*this = tensor(A);
}
tensor::tensor(string S, double ph, double th, double ps){
    tensor A = tensor(ph,th,ps);
    if(S=="r->b"){
        *this = A;
    }
    else if(S=="b->r"){
        A = A.transpose();
        *this = A;
    }
    else{
        cout<<"invalid string input<<\n";
    }
}

tensor::tensor(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22) {
	M[0][0] = m00;
	M[0][1] = m01;
	M[0][2] = m02;
	M[1][0] = m10;
	M[1][1] = m11;
	M[1][2] = m12;
	M[2][0] = m20;
	M[2][1] = m21;
	M[2][2] = m22;
}

void tensor::operator=(tensor A){
    for(int i=0 ; i<3 ; i++)for(int j=0 ; j<3 ; j++)M[i][j] = A.M[i][j];
}
bool tensor::operator==(tensor A){
    for(int i=0 ; i<3 ; i++)for(int j=0 ; j<3 ; j++)if(M[i][j] != A.M[i][j])return 0;
    return 1;
}

tensor tensor::operator*(double x){
    double A[3][3]={0};
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            A[i][j] = M[i][j]*x;
        }
    }
    return tensor(A);
}
tensor tensor::operator/(double x){
    double A[3][3]={0};
    if(x==0){
        cout<<"warning: devide 0 in tensor operator /\n";
        return tensor();
    }
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            A[i][j] = M[i][j]/x;
        }
    }
    return tensor(A);
}

vec tensor::operator*(vec B){
    double A[3]={0};
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            A[i] += M[i][j]*B.V[j];
        }
    }
    return vec(A);
}
tensor tensor::operator-(tensor A){
    double B[3][3];
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            B[i][j] = M[i][j]-A.M[i][j];
        }
    }
    return tensor(B);
}
tensor tensor::operator+(tensor A){
    double B[3][3];
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            B[i][j] = M[i][j]+A.M[i][j];
        }
    }
    return tensor(B);
}
tensor tensor::operator*(tensor B){
    double A[3][3]={0};
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            for(int k=0 ; k<3 ; k++){
                A[i][j] += M[i][k]*B.M[k][j];
            }
        }
    }
    return tensor(A);
}
double tensor::det(){//determinant of tensor
    return M[0][0]*M[1][1]*M[2][2] - M[0][0]*M[1][2]*M[2][1] + M[0][1]*M[1][2]*M[2][0] - M[0][1]*M[1][0]*M[2][2] + M[0][2]*M[1][0]*M[2][1] - M[0][2]*M[1][1]*M[2][0];
}
tensor tensor::transpose(){
    tensor A = tensor();
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            A.M[i][j] = M[j][i];
        }
    }
    return A;
}
tensor tensor::inverse(){
    double A[3][3];
	double invdet = 1 / det();
    if(invdet==0){
        cout<<"warning: det of tensor is 0\n";
        return tensor();
    }
    A[0][0] = (M[1][1] * M[2][2] - M[2][1] * M[1][2]) * invdet;
    A[0][1] = (M[0][2] * M[2][1] - M[0][1] * M[2][2]) * invdet;
    A[0][2] = (M[0][1] * M[1][2] - M[0][2] * M[1][1]) * invdet;
    A[1][0] = (M[1][2] * M[2][0] - M[1][0] * M[2][2]) * invdet;
    A[1][1] = (M[0][0] * M[2][2] - M[0][2] * M[2][0]) * invdet;
    A[1][2] = (M[1][0] * M[0][2] - M[0][0] * M[1][2]) * invdet;
    A[2][0] = (M[1][0] * M[2][1] - M[2][0] * M[1][1]) * invdet;
    A[2][1] = (M[2][0] * M[0][1] - M[0][0] * M[2][1]) * invdet;
    A[2][2] = (M[0][0] * M[1][1] - M[1][0] * M[0][1]) * invdet;
    return tensor(A);
}
void tensor::torotmat(){
    tensor trans = tensor();
    while(1.0 + M[0][0] + M[1][1] + M[2][2]<1e-8){
        trans *= tensor(1,1,1);
        *this = tensor(1,1,1)* *this;
    }
	double w = sqrt(1.0 + M[0][0] + M[1][1] + M[2][2]) / 2.0;
	double w4 = (4.0 * w);
	double x = (M[2][1] - M[1][2]) / w4 ;
	double y = (M[0][2] - M[2][0]) / w4 ;
	double z = (M[1][0] - M[0][1]) / w4 ;

    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;

    double invs = 1 / (sqx + sqy + sqz + sqw);
    M[0][0] = ( sqx - sqy - sqz + sqw)*invs ; // since sqw + sqx + sqy + sqz =1/invs*invs
    M[1][1] = (-sqx + sqy - sqz + sqw)*invs ;
    M[2][2] = (-sqx - sqy + sqz + sqw)*invs ;
    
    double tmp1 = x*y;
    double tmp2 = z*w;
    M[1][0] = 2.0 * (tmp1 + tmp2)*invs ;
    M[0][1] = 2.0 * (tmp1 - tmp2)*invs ;
    
    tmp1 = x*z;
    tmp2 = y*w;
    M[2][0] = 2.0 * (tmp1 - tmp2)*invs ;
    M[0][2] = 2.0 * (tmp1 + tmp2)*invs ;
    tmp1 = y*z;
    tmp2 = x*w;
    M[2][1] = 2.0 * (tmp1 + tmp2)*invs ;
    M[1][2] = 2.0 * (tmp1 - tmp2)*invs ;   

    *this = trans.transpose()* *this;
}

tensor cross(tensor T, vec V){
     vec A = vec(T.M[0])*V;
     vec B = vec(T.M[1])*V;
     vec C = vec(T.M[2])*V;
     return tensor(A,B,C);
}
tensor cross(vec B, tensor T){// 주의: vec cross tensor*vec가 잘 정의되게 하기 위함임. 따라서 열벡터로 생각해서 cross를 해줌
    double A[3][3] = \
    {{0,-B.V[2],B.V[1]},{B.V[2],0,-B.V[0]},{-B.V[1],B.V[0],0}};
    return tensor(A)*T;
}

tensor operator *(double x, tensor A){
    return A*x;
}

    
void tensor::operator +=(tensor T){
    *this = *this+T;
}
void tensor::operator *=(tensor T){
    *this = (*this)*T;
}
void tensor::operator -=(tensor T){
    *this = *this-T;
}
void tensor::operator *=(double x){
    *this = *this*x;
}
void tensor::operator /=(double x){
    *this = *this/x;
}

ostream& operator<<(ostream& output, const tensor& H){
    cout<<"tensor = \n";
    for(int i=0 ; i<3 ; i++){
        for(int j=0 ; j<3 ; j++){
            cout<<H.M[i][j]<<", ";
        }
        cout<<"\n";
    }
    return output;
}
void Rotwarning(tensor R){
    if(abs(R.det()-1)>1e-9){
        cout<<"Warning: det of Rotation mat in coor_trans is "<<R.det()<<" NOT 1\n";
    }
}
tensor coor_trans(tensor R, tensor A){
    Rotwarning(R);
    return R*A*R.transpose();
}

vec coor_trans(tensor R, vec A){
    Rotwarning(R);
    return R*A;
}


tensor tensor::fromaxis(vec Normal) {
	double angle = Normal.norm();
	if (angle == 0)return tensor();
	Normal = Normal / angle;
	return fromaxis(Normal, angle);
}
tensor tensor::fromaxis(vec Normal, double angle) {
	double m00, m01, m02, m10, m11, m12, m20, m21, m22;
	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	//  if axis is not already normalised then uncomment this
	// double magnitude = Math.sqrt(Normal.V[0]*Normal.V[0] + Normal.V[1]*Normal.V[1] + Normal.V[2]*Normal.V[2]);
	// if (magnitude==0) throw error;
	// Normal.V[0] /= magnitude;
	// Normal.V[1] /= magnitude;
	// Normal.V[2] /= magnitude;

	m00 = c + Normal.V[0] * Normal.V[0] * t;
	m11 = c + Normal.V[1] * Normal.V[1] * t;
	m22 = c + Normal.V[2] * Normal.V[2] * t;


	double tmp1 = Normal.V[0] * Normal.V[1] * t;
	double tmp2 = Normal.V[2] * s;
	m10 = tmp1 + tmp2;
	m01 = tmp1 - tmp2;
	tmp1 = Normal.V[0] * Normal.V[2] * t;
	tmp2 = Normal.V[1] * s;
	m20 = tmp1 - tmp2;
	m02 = tmp1 + tmp2;    tmp1 = Normal.V[1] * Normal.V[2] * t;
	tmp2 = Normal.V[0] * s;
	m21 = tmp1 + tmp2;
	m12 = tmp1 - tmp2;
	return tensor(m00, m01, m02, m10, m11, m12, m20, m21, m22);
}

vector<vec> operator+ (vector<vec> A, vector <vec> B){
    vector <vec> C;
    if(A.size()!=B.size()){
        cout<<"size of two vectors are different in vector<vec> + operator\n";
        return A;
    }
    for(int i=0 ; i<A.size() ; i++){
        C.push_back(A[i]+B[i]);
    }
    return C;
} 