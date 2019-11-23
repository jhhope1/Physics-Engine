#pragma once
#ifndef WORLDVERTICES
#define WORLDVERTICES
#include"Visualize.h"
using namespace std;//없어져도 될듯
class WorldVertices {
public:
	static void push_Objects(vector <Object> OB) {
		for (Object ob : OB) {
			pushcube(ob.pos_f,ob.rotmat_if()*ob.rotmat_bi());
		}
	}
};
#endif