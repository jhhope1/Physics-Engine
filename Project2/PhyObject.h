#ifndef PHYOBJECT_H
#define PHYOBJECT_H
#include<iostream>
#include<vector>
#include<string>
#include"Tensor.h"
using namespace std;
class cube {
public:
	static unsigned int vertexnum;
	static unsigned int indnum;
	static unsigned int stride;
	cube();
	cube(vec resize);
	float vertices[36*6];
	unsigned int indices[36];
};
#endif