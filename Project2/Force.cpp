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


void Force::update_Object(Object* ob, vec workingpoint, vec force, double dt) {
	ob->Object_update_without_pos_rotmat(force, (workingpoint - ob->pos_f) * force, dt);
}

double collision_coefficient(Object *ob1, Object *ob2, vec N, vec interpoint) {
	/*vec w1_f = ob1->w_f();
	vec w2_f = ob2->w_f();
	vec v1_f = ob1->v_f;
	vec v2_f = ob2->v_f;
	vec r1_f = interpoint - ob1->pos_f;
	vec r2_f = interpoint - ob2->pos_f;
	double m1 = ob1->m;
	double m2 = ob2->m;
	tensor I1_f = coor_trans(ob1->rotmat_if, ob1->Inertia_i);
	tensor I2_f = coor_trans(ob2->rotmat_if, ob2->Inertia_i);


	double co_const = 2 * w1_f.dot(r1_f * N) + 2 * v1_f.dot(N) - 2 * w2_f.dot(r2_f * N) - 2 * v2_f.dot(N);
	double co_p = (r1_f * N).dot(I1_f.inverse() * (r1_f * N)) + (r2_f * N).dot(I2_f.inverse() * (r2_f * N)) + 1 / m1 + 1 / m2;

	return co_const / co_p;*/

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

bool Force::CanCollide(Object* A, Object* B) {
	if ((A->cubesize + B->cubesize).normsquare() > 4*(A->pos_f - B->pos_f).normsquare()) {
		return true;
	}
	return false;
}

void Force::collide_update(vec normal_tot, vec interpoint_tot, Object* A, Object* B, double dt) {
	double coe = collision_coefficient(A, B, normal_tot, interpoint_tot);//indexpointForce를 보내지 말고 그냥 여기에 들어가면 즉각적으로 update가 되도록 하는 함수를 만들어야겠다. 필요가 없고 순차적 충돌에서 에러가 발생함.

	if ((A->pos_f_vel_f(interpoint_tot) - B->pos_f_vel_f(interpoint_tot)).dot(normal_tot) <= 0) {
		return;
	}
	update_Object(B, interpoint_tot, coe * normal_tot , 1);
	update_Object(A, interpoint_tot, -coe * normal_tot , 1);// stl이 그걸 못잡네..
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

	normal_tot /= normalarr->size();
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

	vector <vec> ckedvec;
	vector <vec> Vertexes;
	for (int k = 0; k < B->C.indnum; k++) {
		vec V;
		for (int w = 0; w < 3; w++) {
			V.V[w] = B->C.vertices[B->C.indices[k] * 6 + w];
		}
		Vertexes.push_back(B->pos_b_pos_f(V));
		if (Vertexes.size() % 3 == 0 && Vertexes.size()>0) {
			vec tricm = (Vertexes[Vertexes.size() - 1] + Vertexes[Vertexes.size() - 2] + Vertexes[Vertexes.size() - 3]) / 3;
			Vj.push_back(tricm);
		}
		if (find(ckedvec.begin(), ckedvec.end(), V) != ckedvec.end())continue;
		ckedvec.push_back(V);
		Vj.push_back(B->pos_b_pos_f(V));
	}
	vec cubecenteri = A->pos_f;

	//필요한 모든 점 구함.
	
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
				//continue;
			}

			ckedvec.push_back(V);
			interpointnum++;

			//collide_update(X.normal,X.interpoint, A, B, dt);

			interpointarr.push_back(X.interpoint);
			normalarr.push_back(X.normal);
		}
	}
	if(interpointnum)
		collide_update(&normalarr,&interpointarr,A,B,dt);
	return interpointnum;
}

void Force::Collision_Line_Line(Object* A, Object* B, double dt) {
	vector <Triangle> Ti;
	vector <vec> Vj;
	vector <Line> Lj;

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
			//if (trind == 2)break;
		}
		if (trind >= 1) {
			Line L1 = Geomfunc::shareLineTT(tri[0], tri[1]);
			BVV X = Intersect::lineline(L, L1);

			collide_update(X.normal, X.interpoint, A, B, dt);
			return;//모르겠다.
		}
	}
}

void Force::Gravity(int ind, Object* ob, double dt) {
	vec g = vec(0, 0, -gravity_acceleration);
	update_Object(ob, ob->pos_f, ob->m * g, dt);
	tensor TENBF = ob->rotmat_if * ob->rotmat_bi;
	vector <vec> Vob;
	for (int k = 0; k < ob->C.indnum; k++) {
		vec V;
		for (int w = 0; w < 3; w++) {
			V.V[w] = ob->C.vertices[ob->C.indices[k] * 6 + w];
		}
		Vob.push_back(TENBF * V + ob->pos_f);
	}
	vector<vec> ckedvec;
	for (int i = 0; i < Vob.size(); i++) {
		if (find(ckedvec.begin(), ckedvec.end(), Vob[i]) != ckedvec.end())continue;
		ckedvec.push_back(Vob[i]);
		if (Vob[i].V[2] < 0) {
			IndexPointForce_f.push_back(IVV(ind,Vob[i],-100*ob->m*vec(0,0,Vob[i].V[2])));
		}
	}
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

int Force::GenIndexPointForce_f(vector <Object*> OB, double dt){
    //vector <vec> B = trans_sf();
	IndexPointForce_f.clear();


	for (int i = 0; i < OB.size(); i++) {
		//Gravity(i, &OB[i]);
	}
	Gravity_Object(OB, dt);

	int collidetot = 0;
	//Colide Force
	for (int i = 0; i < OB.size(); i++) {
		for (int j = i + 1; j < OB.size(); j++) {
			if (CanCollide(OB[i], OB[j]) == false)continue;
			int collidenum = 0;
			collidenum += Collision_Triangle_Point(OB[i], OB[j], dt);
			collidenum += Collision_Triangle_Point(OB[j], OB[i], dt);
			collidetot += collidenum;
			//collision 후에 두 물체를 매우 살짝 띄워놔야함. 안그러면 너무 많은 collision으로 인식해서 문제 생기는듯.->그냥 두 물체가 다가오는지 판단해서 충돌함수를 수정하는 것이 맞는듯.
			if (collidenum) {
				continue;
			}
			Collision_Line_Line(OB[i], OB[j], dt);
		}
	}
	return collidetot;
    //return A;
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