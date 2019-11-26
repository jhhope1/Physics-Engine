#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "Tensor.h"
class BVV {
public:
	bool intersect;
	vec interpoint;
	vec normal;
	inline BVV(bool intersect, vec interpoint, vec normal):intersect(intersect),interpoint(interpoint),normal(normal){}

	inline friend ostream& operator<<(ostream& output, const BVV& H) {
		cout << "BVV = \n";
		cout << "Bool = " << H.intersect << "\n";
		cout << "interpoint = " << H.interpoint;
		cout << "normal = " << H.normal << "\n";
		return output;
	}
};
class BD {
public:
	bool B;
	double d;
	inline BD(bool B, double d) :B(B), d(d) {}
};
class Triangle {
public:
	vec a, b, c;
	inline Triangle(vec a, vec b, vec c):a(a),b(b),c(c) {}
	inline Triangle() : a(vec()), b(vec()), c(vec()) {}
	inline vec Nhat() {
		return (b - a) * (c - a);
	}
	inline vec N() {
		vec hat = Nhat();
		return hat / hat.norm();
	}
	inline bool pointin(vec A) {
		vec x = a - A;
		vec y = b - A;
		vec z = c - A;
		vec x0 = x * y;
		vec y0 = y * z;
		vec z0 = z * x;
		if (x0.dot(y0) < 0) {
			return false;
		}
		else if (y0.dot(z0) < 0) {
			return false;
		}
		else if (z0.dot(x0) < 0) {
			return false;
		}
		return true;
	}
};
class Line {
public:
	vec a, b;
	inline Line(vec a, vec b):a(a),b(b) {}
	inline bool pointin(vec V) {
		if (((a - b) * (V - a)).norm() < INFIMUM && (b - V).dot(V - a) > 0) {
			return true;
		}
		return false;
	}
	inline bool pointinfinite(vec V) {
		if (((a - b) * (V - a)).norm() < INFIMUM) {
			return true;
		}
		return false;
	}
};

class Geomfunc {
public:
	static bool OrderedLine(vec A, vec B, vec C) {
		if ((B - A).dot(C - B) > 0) {
			return true;
		}
		return false;
	}
	static Line shareLineTT(Triangle T1, Triangle T2) {
		vec a, b;
		if (T1.a != T2.a && T1.a != T2.b && T1.a != T2.c) {
			a = T1.b;
			b = T1.c;
		}
		else if (T1.b != T2.a && T1.b != T2.b && T1.b != T2.c) {
			a = T1.c;
			b = T1.a;
		}
		else{
			a = T1.a;
			b = T1.b;
		}
		return Line(a, b);
	}
};

class Intersect {
public:
	static BVV tripoint(vec cubecenter, Triangle A, vec B) {//norm: direction of norm is taken out of an object Triangle A.
		vec interpoint = trilineinfinate(A, Line(cubecenter, B));
		if(A.pointin(interpoint)==false || Geomfunc::OrderedLine(cubecenter,B,interpoint)==false){
			return BVV(false, vec(), vec());
		}
		vec N = A.Nhat();
		if (N.dot(cubecenter - B) > 0)N = vec()-N;
		return BVV(true, B, N);
	}
	static vec trilineinfinate(Triangle T, Line L) {
		vec N = T.N();
		double parent = N.dot(L.b - L.a);
		vec interpoint = N.dot(T.a - L.a) / parent * (L.b - L.a) + L.a;
		return interpoint;

	}
	static bool triline(Triangle T, Line L) {
		vec interpoint = trilineinfinate(T, L);
		if (Geomfunc::OrderedLine(L.a, interpoint, L.b)&&T.pointin(interpoint))return true;
		return false;
	}
	static BVV lineline(Line A, Line B) {//주의: true만 return. triline으로 미리 검열 해줘야함.
		vec x = A.a - A.b;
		vec y = B.a - B.b;
		vec c = x * y;
		vec N = c / c.norm();
		double t1 = (((B.a - A.a)  - N * (B.a - A.a).dot(N))*y).dot(N) / (x * y).dot(N);
		vec D = t1 * x + A.a + N * (B.a - A.a).dot(N)/2;//nearest들의 중간
		double E = (B.a - A.a).dot(N);
		if (N.dot(B.a) > N.dot(A.a)) {
			return BVV(true,D,N);
		}
		else {
			return BVV(true,D,-1*N);
		}
	}
};

#endif