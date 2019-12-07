#ifndef FORCE_H
#define FORCE_H
#include "Geometry.h"
#include "Lagrangian.h"
#include <stack>
using namespace std;
typedef pair<int, int> pii;
class IVV {//index point Force
public:
	int ind;
	vec workingpoint;
	vec force;
	inline IVV() :ind(0), workingpoint(0), force(0) {}
	inline IVV(int ind, vec workingpoint, vec force):ind(ind),workingpoint(workingpoint),force(force){};
};
class Force{
public:
	static vector <VV> FT_tot;

	static const double gravity_acceleration;
	static const double gravitational_constant;
	static const double spring_constant;
	static const double REPULSIVE_COEFFICIENT;

	static bool CanCollide(Object *A, Object *B);
	static int Collision_Triangle_Point(Object *A, Object *B, double dt);
	static int Collision_Line_Line(Object* A, Object* B, double dt);
	static void GenIndexPointForce_f(vector <Object*> OB, double dt);
	static void Gravity(int ind, Object *ob, double dt);
	static void Gravity_Object(vector<Object*> OB, double dt);
	static void Spring_Objects_two_body(Object* ob1, Object* ob2, vec workingpoint1_b, vec workingpoint2_b, double Natural_lenght, double dt);
	static void Spring_Objects(vector<Object*>OB, double dt);
	static void Collide_Force(vector<Object*> OB, double dt);
	static void update_Object_Force(Object* ob, vec workingpoint, vec force, double dt);
	static void update_Object_Force(vector<Object*> OB, double dt);
	static void update_Object_without_Force(Object* ob, double dt);
	static void update_Object_without_Force(vector<Object*> ob, double dt);
	static void update_Object(vector<Object*> OB, VectorXd qddot, double dt);
	static void collide_update(vector <vec>* normalarr, vector<vec>* interpointarr, Object* A, Object* B, double dt);
	static void collide_update(vec normal_tot, vec interpoint_tot, Object* A, Object* B, double dt);
	static void wall(vector <Object*> OB, vec point, vec plainnormal);
	static void collide_update(vec normal_tot, vec interpoint_tot, Object* A);
	static double collision_coefficient_wall(Object* ob, vec N, vec interpoint);
	static double collision_coefficient_wall_inelastic(Object* ob1, vec N, vec interpoint);
	static double collision_coefficient_Repulsive_coefficient_wall(Object* ob1, vec N, vec interpoint, double Repulsive_coefficient);
	static void resistance(Object ob1, Object ob2, double mu_s, double mu_k, vec normal, vec interpoint);
	static double collision_coefficient_inelastic(Object* ob1, Object* ob2, vec N, vec interpoint);
	static double collision_coefficient(Object* ob1, Object* ob2, vec N, vec interpoint);
	static double collision_coefficient_Repulsive_coefficient(Object* ob1, Object* ob2, vec N, vec interpoint, double Repulsive_coefficient);
	static void avoid_overlap(vector <Object*> OB, double dt);
	static void avoid_overlap_wall(vector <Object*> OB, vec point, vec plainnormal);
};
#endif