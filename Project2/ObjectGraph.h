#ifndef OBJECTGRAPH_H
#define OBJECTGRAPH_H
#include "Object.h"
class ObjectGraph {
public:
	vector <int> G;
	void push_constraint();
	void push_Object(Object Ob);
};
#endif