#pragma once
#ifndef WORLDVERTICES
#define WORLDVERTICES
#include"Visualize.h"
using namespace std;//�������� �ɵ�
class WorldVertices {
public:
	static void push_Objects(vector <Object> OB) {
		for (Object ob : OB) {
			pushcube(ob.pos_f,ob.rotmat_if()*ob.rotmat_bi());
		}
	}
};
#endif