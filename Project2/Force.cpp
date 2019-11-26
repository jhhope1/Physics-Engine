#include "Force.h"
#include "Geometry.h"
/*
Force::trans_sf(){
    for(string S:scaned_ForceField){
        for(int i=0 ; i<S.size() ; i++){
            
        }
    }
}*//*
vector <vec> Force::generate_Force(vector <Object> OB){
    //vector <vec> B = trans_sf();
    vector <vec> A;
    for(int i=0 ; i<OB.size() ; i++)A.push_back(vec());
    return A;//returna B;
}*/
const double Force::gravity_acceleration = 1.;
const double Force::gravitational_constant = 10.;

vector<IVV> Force::IndexPointForce_f = vector<IVV>(0);


void Force::avoid_overlap(vector <Object*> OB, double dt) {//이거 고칠라면 전부 다 뜯어야되서 일단 보류. 
	//나중에는 이런식으로 0.1배 해서 스택으로 안쌓고 값 계산해서 한번에 떨어뜨려놓는 걸 목표로 하는게 맞다.
	stack <pii> CollideOB;
	for (int i = 0; i < OB.size(); i++) {
		for (int j = i + 1; j < OB.size(); j++) {
			if (CanCollide(OB[i], OB[j])) {
				int collidenum = 0;
				collidenum += Collision_Triangle_Point(OB[i], OB[j], 1);
				collidenum += Collision_Triangle_Point(OB[j], OB[i], 1);
				if (collidenum==0)collidenum += Collision_Line_Line(OB[i], OB[j], dt);
				if (collidenum)CollideOB.push(pii(i, j));
			}
		}
	}

	if (CollideOB.size()>=1) {
		int a = 1;
	}

	while (CollideOB.size()) {
		pii P = CollideOB.top();
		CollideOB.pop();
		
		double dx = 0;
		for (int i = 0; i < 3; i++) {
			dx += abs(OB[P.first]->v_f.V[i]);
			dx += abs(OB[P.second]->v_f.V[i]);
		}
		dx *= dt;
		vec r = (OB[P.first]->pos_f - OB[P.second]->pos_f);
		r = r / r.norm();
		vec dn = r * dx*0.1;

		OB[P.first]->pos_f += dn;
		OB[P.second]->pos_f -= dn;
		
		for (int i = 0; i < OB.size(); i++) {
			if (i == P.first)continue;
			if (CanCollide(OB[i], OB[P.first])) {
				int collidenum = 0;
				collidenum += Collision_Triangle_Point(OB[i], OB[P.first], 1);
				collidenum += Collision_Triangle_Point(OB[P.first], OB[i], 1);
				if (collidenum==0)collidenum += Collision_Line_Line(OB[i], OB[P.first], dt);
				if (collidenum)CollideOB.push(pii(i, P.first));
			}
		}
		for (int i = 0; i < OB.size(); i++) {
			if (i == P.second || i == P.first)continue;
			if (CanCollide(OB[i], OB[P.second])) {
				int collidenum = 0;
				collidenum += Collision_Triangle_Point(OB[i], OB[P.second], 1);
				collidenum += Collision_Triangle_Point(OB[P.second], OB[i], 1);
				if (collidenum)collidenum += Collision_Line_Line(OB[i], OB[P.second], dt);
				if (collidenum)CollideOB.push(pii(i, P.second));
			}
		}
	}	
}


void Force::avoid_overlap_wall(vector <Object*> OB, vec point, vec plainnormal) {
	for (Object* ob : OB) {
		vector <vec>* Pt = new vector<vec>(0);
		ob->ObjectPoints(Pt);
		for (vec P : *Pt) {
			if (P.dot(plainnormal) < point.dot(plainnormal)) {
				ob->pos_f += -P.dot(plainnormal) * plainnormal;
				break;
			}
		}
	}
}


void Force::update_Object(Object* ob, vec workingpoint, vec force, double dt) {
	ob->Object_update_without_pos_rotmat(force, (workingpoint - ob->pos_f) * force, dt);
}

double Force::collision_coefficient_wall(Object *ob, vec N, vec interpoint) {
	vec w1_b = ob->w_b;
	vec v1_f = ob->v_f;
	vec r1_f = interpoint - ob->pos_f;
	double m1 = ob->m;
	tensor I1_b = ob->Inertia_b;

	tensor R_1 = ob->rotmat_if * ob->rotmat_bi;

	vec q1 = I1_b.inverse() * R_1.transpose() * (r1_f * N);

	double co_const = v1_f.dot(N) + w1_b.dot(I1_b * q1);
	double co_p = 1 / m1 + q1.dot(I1_b * q1);

	return 2 * co_const / co_p;
}

double Force::collision_coefficient(Object *ob1, Object *ob2, vec N, vec interpoint) {
	vec w1_b = ob1->w_b;
	vec w2_b = ob2->w_b;
	vec v1_f = ob1->v_f;
	vec v2_f = ob2->v_f;
	vec r1_f = interpoint - ob1->pos_f;
	vec r2_f = interpoint - ob2->pos_f;
	double m1 = ob1->m;
	double m2 = ob2->m;
	tensor I1_b = ob1->Inertia_b;
	tensor I2_b = ob2->Inertia_b;

	tensor R_1 = ob1->rotmat_if * ob1->rotmat_bi;
	tensor R_2 = ob2->rotmat_if * ob2->rotmat_bi;

	vec q1 = I1_b.inverse() * R_1.transpose() * (r1_f * N);
	vec q2 = I2_b.inverse() * R_2.transpose() * (r2_f * N);

	double co_const = v1_f.dot(N)-v2_f.dot(N)+w1_b.dot(I1_b*q1)-w2_b.dot(I2_b*q2);
	double co_p = 1 / m1 + 1 / m2 + q1.dot(I1_b * q1) + q2.dot(I2_b * q2);

	return 2*co_const / co_p;
}

double Force::collision_coefficient_inelastic(Object* ob1, Object* ob2, vec N, vec interpoint) {
	vec w1_b = ob1->w_b;
	vec w2_b = ob2->w_b;
	vec v1_f = ob1->v_f;
	vec v2_f = ob2->v_f;
	vec r1_f = interpoint - ob1->pos_f;
	vec r2_f = interpoint - ob2->pos_f;
	double m1 = ob1->m;
	double m2 = ob2->m;
	tensor I1_b = ob1->Inertia_b;
	tensor I2_b = ob2->Inertia_b;
	tensor I1_f = coor_trans(ob1->rotmat_if, ob1->Inertia_i);
	tensor I2_f = coor_trans(ob2->rotmat_if, ob2->Inertia_i);

	double co_const = (ob1->pos_f_vel_f(interpoint) - ob2->pos_f_vel_f(interpoint)).dot(N);
	double co_p = (N / m1 +  ((I1_f.inverse() * (r1_f * N)) * r1_f) + (N / m2 + ((I2_f.inverse() * (r2_f * N)) * r2_f))).dot(N);
	return co_const / co_p;
}


double Force::collision_coefficient_wall_inelastic(Object* ob1, vec N, vec interpoint) {
	vec w1_b = ob1->w_b;
	vec v1_f = ob1->v_f;
	vec r1_f = interpoint - ob1->pos_f;
	double m1 = ob1->m;
	tensor I1_b = ob1->Inertia_b;
	tensor I1_f = coor_trans(ob1->rotmat_if, ob1->Inertia_i);

	double co_const = ob1->pos_f_vel_f(interpoint).dot(N);
	double co_p = (N / m1 + ((I1_f.inverse() * (r1_f * N)) * r1_f)).dot(N);
	return co_const / co_p;
}

double Force::collision_coefficient_Repulsive_coefficient(Object* ob1, Object* ob2, vec N, vec interpoint, double Repulsive_coefficient) {
	double co_elastic = collision_coefficient(ob1, ob2, N, interpoint);
	double co_inelastic = collision_coefficient_inelastic(ob1, ob2, N, interpoint);
	return co_elastic * Repulsive_coefficient + co_inelastic * (1-Repulsive_coefficient);
}

double Force::collision_coefficient_Repulsive_coefficient_wall(Object* ob1, vec N, vec interpoint, double Repulsive_coefficient) {
	double co_elastic = collision_coefficient_wall(ob1, N, interpoint);
	double co_inelastic = collision_coefficient_wall_inelastic(ob1, N, interpoint);

	return co_elastic * Repulsive_coefficient + co_inelastic * (1-Repulsive_coefficient);
}

bool Force::CanCollide(Object* A, Object* B) {
	return (A->cubesize + B->cubesize).normsquare() > 4 * (A->pos_f - B->pos_f).normsquare();
}

void Force::collide_update(vec normal_tot, vec interpoint_tot, Object* A, Object* B, double dt) {//object
	double coe = collision_coefficient_Repulsive_coefficient(A, B, normal_tot, interpoint_tot, 0.0);//indexpointForce를 보내지 말고 그냥 여기에 들어가면 즉각적으로 update가 되도록 하는 함수를 만들어야겠다. 필요가 없고 순차적 충돌에서 에러가 발생함.

	if ((A->pos_f_vel_f(interpoint_tot) - B->pos_f_vel_f(interpoint_tot)).dot(normal_tot) < 0) {
		return;
	}

	update_Object(B, interpoint_tot, coe * normal_tot, 1);
	update_Object(A, interpoint_tot, -coe * normal_tot, 1);// stl이 그걸 못잡네..
}

void Force::collide_update(vec normal_tot, vec interpoint_tot, Object* A) {//wall
	double coe = collision_coefficient_Repulsive_coefficient_wall(A, normal_tot, interpoint_tot, 0.0);//indexpointForce를 보내지 말고 그냥 여기에 들어가면 즉각적으로 update가 되도록 하는 함수를 만들어야겠다. 필요가 없고 순차적 충돌에서 에러가 발생함.

	if (A->pos_f_vel_f(interpoint_tot).dot(normal_tot) > 0) {
		return;
	}

	update_Object(A, interpoint_tot, -coe * normal_tot, 1);// 부호 결정
}

void Force::collide_update(vector <vec>* normalarr, vector<vec>* interpointarr, Object* A, Object* B, double dt) {
	vec normal_tot;
	vec interpoint_tot;

	for (vec V : *normalarr) {
		normal_tot += V;
	}
	for (vec V : *interpointarr) {
		interpoint_tot += V;
	}

	interpoint_tot /= interpointarr->size();
	normal_tot = normal_tot / normal_tot.norm();

	collide_update(normal_tot, interpoint_tot, A, B, dt);
}

int Force::Collision_Triangle_Point(Object* A, Object* B, double dt) {//A: triangle, B:point
	vector <Triangle> Ti;
	vector <vec> Vj;
	for (int k = 0; k < A->C.indnum / 3; k++) {
		vec V[3];
		for (int l = 0; l < 3; l++) {
			for (int w = 0; w < 3; w++) {
				V[l].V[w] = A->C.vertices[A->C.indices[k * 3 + l] * 6 + w];
			}
		}
		Ti.push_back(Triangle(A->pos_b_pos_f(V[0]), A->pos_b_pos_f(V[1]), A->pos_b_pos_f(V[2])));
	}

	B->ObjectPoints(&Vj);
	vec cubecenteri = A->pos_f;

	//필요한 모든 점 구함.
	
	vector<vec> ckedvec;
	int interpointnum = 0;
	ckedvec.clear();
	vector<vec> normalarr;
	vector<vec> interpointarr;
	for (vec V : Vj) {
		if (find(ckedvec.begin(), ckedvec.end(), V) != ckedvec.end()) continue;
		for (Triangle T : Ti) {
			BVV X = Intersect::tripoint(cubecenteri, T, V);
			if (X.intersect == false) {
				continue;
			}
			if ((A->pos_f_vel_f(X.interpoint)-B->pos_f_vel_f(X.interpoint)).dot(X.normal)<0) {
				continue;
			}

			ckedvec.push_back(V);
			interpointnum++;

			interpointarr.push_back(X.interpoint);
			normalarr.push_back(X.normal);
		}
	}
	if (interpointnum) {
		collide_update(&normalarr, &interpointarr, A, B, dt);
	}
	return interpointnum;
}

int Force::Collision_Line_Line(Object* A, Object* B, double dt) {
	vector <Triangle> Ti;
	vector <vec> Vj;
	vector <Line> Lj;
	int collnum = 0;

	for (int k = 0; k < A->C.indnum / 3; k++) {
		vec V[3];
		for (int l = 0; l < 3; l++) {
			for (int w = 0; w < 3; w++) {
				V[l].V[w] = A->C.vertices[A->C.indices[k * 3 + l] * 6 + w];
			}
		}
		Ti.push_back(Triangle(A->pos_b_pos_f(V[0]), A->pos_b_pos_f(V[1]), A->pos_b_pos_f(V[2])));
	}

	for (int k = 0; k < B->C.indnum; k++) {
		vec V;
		for (int w = 0; w < 3; w++) {
			V.V[w] = B->C.vertices[B->C.indices[k] * 6 + w];
		}
		Vj.push_back(B->pos_b_pos_f(V));
	}
	vec cubecenteri = A->pos_f;

	for (int i = 0; i < Vj.size()/3; i++) {
		Lj.push_back(Line(Vj[i * 3 + 0], Vj[i * 3 + 1]));
		Lj.push_back(Line(Vj[i * 3 + 1], Vj[i * 3 + 2]));
		Lj.push_back(Line(Vj[i * 3 + 2], Vj[i * 3 + 0]));
	}

	//필요한 모든 점 구함.

	for (Line L : Lj) {
		Triangle tri[10];
		int trind = 0;
		for (Triangle T : Ti) {
			if (Intersect::triline(T, L)) {
				tri[trind] = T;
				trind++;
			}
		}
		if (trind >= 2) {
			Line L1 = Geomfunc::shareLineTT(tri[0], tri[1]);
			BVV X = Intersect::lineline(L, L1);

			collide_update(X.normal, X.interpoint, A, B, dt);
			collnum++;
		}
	}
	return collnum;
}

void Force::Gravity(int ind, Object* ob, double dt) {
	vec g = vec(0, 0, -gravity_acceleration);
	update_Object(ob, ob->pos_f, ob->m * g, dt);
	return;
}

void Force::Gravity_Object(vector<Object*> OB, double dt) {
	for (int i = 0; i < OB.size(); i++) {
		for (int j = i + 1; j < OB.size(); j++) {
			vec r = OB[j]->pos_f - OB[i]->pos_f;
			vec F = gravitational_constant*r/r.norm()/r.dot(r);
			update_Object(OB[i], OB[i]->pos_f, F, dt);
			update_Object(OB[j], OB[j]->pos_f, -1*F, dt);
		}
	}
}

void Force::GenIndexPointForce_f(vector <Object*> OB, double dt){
	IndexPointForce_f.clear();

	//-z direction gravity
	for (int i = 0; i < OB.size(); i++) {
		Gravity(i, OB[i], dt);
	}

	//gravity along objects
	Gravity_Object(OB, dt);

	//Colide Force
	for (int i = 0; i < OB.size(); i++) {
		for (int j = i + 1; j < OB.size(); j++) {
			if (CanCollide(OB[i], OB[j]) == false)continue;
			int collidenum = 0;
			collidenum += Collision_Triangle_Point(OB[i], OB[j], dt);
			collidenum += Collision_Triangle_Point(OB[j], OB[i], dt);
			if (collidenum) {
				continue;
			}
			collidenum+=Collision_Line_Line(OB[i], OB[j], dt);
		}
	}

	//plain wall
	wall(OB, vec(0, 0, 0), vec(0, 0, 1));
	avoid_overlap(OB,dt);
	avoid_overlap_wall(OB, vec(0, 0, 0), vec(0, 0, 1));
}

vector <vec> Force::Force_f(vector <Object*> OB) {
	vector <vec> A = vector<vec>(OB.size());
	for (IVV ivv : IndexPointForce_f) {
		A[ivv.ind] += ivv.force;
	}
	return A;
}

vector <vec> Force::Torque_f(vector <Object*> OB){
    vector <vec> A = vector<vec>(OB.size());
	for (IVV ivv : IndexPointForce_f) {
		A[ivv.ind] += (ivv.workingpoint - OB[ivv.ind]->pos_f) * ivv.force;
	}
    return A;//return B;
}

void Force::wall(vector <Object*> OB, vec point, vec plainnormal) {
	for (Object* ob : OB) {
		vector <vec>* Pt = new vector<vec>(0);
		ob->ObjectPoints(Pt);
		for (vec P : *Pt) {
			if (P.dot(plainnormal) < point.dot(plainnormal)) {
				collide_update(plainnormal, P, ob);
			}
		}
	}
}

//object - object // object와 wall을 동시에 다루고 싶은데 class가 달라서 이전에 신경 안썼던 메모리 누수가 걱정된다
void Force::resistance(Object ob1, Object ob2, double mu_s, double mu_k, vec normal, vec interpoint) {
	bool relatavely_move = false;
	if (((ob1.pos_f_vel_f(interpoint) - ob2.pos_f_vel_f(interpoint)) * normal).normsquare() > INFIMUM_RELATAVELY_MOVE) {
		relatavely_move = true;
	}


	if (relatavely_move) {
		vec tangent = (ob1.pos_f_vel_f(interpoint) - ob2.pos_f_vel_f(interpoint)) * normal;
		tangent = tangent / tangent.norm();
	}
}