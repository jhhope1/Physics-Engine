#ifndef FORCE_H
#define FORCE_H
#include "Object.h"
using namespace std;
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
    //변수가 없는 것이 맞을라나, c++프로젝트를 처음해봐서 잘 모르겠다.
    //vector <string> scaned_ForceField_f;//cin으로 Force Field를 받아서 계산해준다. 근데 일단 이건 나중의 일. 일단은 그냥 돌아가기만 하게 만들어야겠다.
    //vector <string> formed_ForceField_f;
    //vector <string> scaned_Force_OB_f;//cin으로 Object들간의 Force Field를 받아서 계산해준다.
    //vector <string> formed_Force_OB_f;
    //generator.. 가 필요가 없어져버렸네..
    //formed_ForceField와 formed_Force_OB에서 뽑아온 것. 지금은 F를 수동으로 채워 넣는 것이 좋을 듯 하다. 계산기는 뭐 백준 뒤져서 나오면 풀고 박아야지..

    //change ForceField string vec to formed_ForceField;
    //vector <vec> trans_sF_f();
    //vector <vec> cal_T_f();f좌표계에서 계산된 토크사실상 Torque를 계산해야되기 때문에 Object가 변수로 들어가서 cross지랄을 해줘야한다. 나중에 구현해야쥐^^ 일단 one body problem example code를 성공적으로 구현한 뒤 생각한다.

	static const double gravity_acceleration;
	static const double gravitational_constant;
	static vector<IVV> IndexPointForce_f;
	static bool CanCollide(Object *A, Object *B);
	static int Collision_Triangle_Point(Object *A, Object *B, double dt);
	static void Collision_Line_Line(Object* A, Object* B, double dt);
	static void GenIndexPointForce_f(vector <Object*> OB, double dt);
	static void Gravity(int ind, Object *ob, double dt);
	static void Gravity_Object(vector<Object*> OB, double dt);
	static void update_Object(Object* ob, vec workingpoint, vec force, double dt);
	static void collide_update(vector <vec>* normalarr, vector<vec>* interpointarr, Object* A, Object* B, double dt);
	static void collide_update(vec normal_tot, vec interpoint_tot, Object* A, Object* B, double dt);
	static void wall(vector <Object*> OB, vec point, vec plainnormal);
	static void collide_update(vec normal_tot, vec interpoint_tot, Object* A);
	static double collision_coefficient_wall(Object* ob, vec N, vec interpoint);

	static vector<vec> Force_f(vector <Object*> OB);//지금은 수동 나중에는 바꿔줘야함->안바꿀까.
    static vector<vec> Torque_f(vector <Object*> OB);//윗 줄과 마찬가지. 그런데 frame을 b로 하는 것이 편한지 아니면 반대가 편한지는 나중에 봐야함. 어차피 b로 변환해야되기는 함.
    //솔직히 바닥에 pivot이나 link되어있는 물체들, 또는 연결된 강체들에 대해서는 자신이 없기는 함... 그래도 식을 세워서 구현할 수 있는데까지는 구현해봐야지..
};
#endif