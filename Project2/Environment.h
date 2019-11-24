#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "Force.h"
#include "Visualize.h"
class Environment{
public:
	bool realtimerender=true;
    double dt;
	vector<Object*> OB;

    //generator
    Environment();
    Environment(double dt);
	Environment(double dt0, bool realtimerender);
    Environment(double dt0, vector<Object*> OB0);
	Environment(double dt0, vector<Object*> OB0, bool realtimerender);

	//funcs
	void push_back(Object* Object0);
	void push_back(vector<Object*> OBJECT);

    void step();

    void simulate(double t);
	vec AngMom();
	vec momentum();
	double KineticE();

    void print(int a);
};
#endif