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
	ob->Object_update(force, (workingpoint-ob->pos_f)*force, dt);
}

double collision_coefficient(Object *ob1, Object *ob2, vec N, vec interpoint) {
	vec w1_f = ob1->w_f();
	vec w2_f = ob2->w_f();
	vec v1_f = ob1->v_f;
	vec v2_f = ob2->v_f;
	vec r1_f = interpoint - ob1->pos_f;
	vec r2_f = interpoint - ob2->pos_f;
	double m1 = ob1->m;
	double m2 = ob2->m;
	tensor I1_f = coor_trans(ob1->rotmat_if, ob1->Inertia_i);
	tensor I2_f = coor_trans(ob2->rotmat_if, ob2->Inertia_i);

	double co_const = 2 * w1_f.dot(r1_f * N) + 2 * v1_f.dot(N)\
		- 2 * w2_f.dot(r2_f * N) - 2 * v2_f.dot(N);
	double co_p = (r1_f * N).dot(I1_f.inverse() * (r1_f * N)) + N.normsquare()\
		+ (r2_f * N).dot(I2_f.inverse() * (r2_f * N)) + N.normsquare();

	return co_const / co_p;
}

bool Force::CanCollide(Object* A, Object* B) {
	if ((A->cubesize + B->cubesize).normsquare() > 4*(A->pos_f - B->pos_f).normsquare()) {
		return true;
	}
	return false;
}
int Force::Collision_Triangle_Point(int indA, int indB, Object* A, Object* B, double dt) {//A: triangle, B:point
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
	for (int k = 0; k < B->C.indnum; k++) {
		vec V;
		for (int w = 0; w < 3; w++) {
			V.V[w] = B->C.vertices[B->C.indices[k] * 6 + w];
		}
		if (find(ckedvec.begin(), ckedvec.end(), V) != ckedvec.end())continue;
		ckedvec.push_back(V);
		Vj.push_back(B->pos_b_pos_f(V));
	}
	vec cubecenteri = A->pos_f;

	//�ʿ��� ��� �� ����.
	
	int interpointnum = 0;
	ckedvec.clear();
	vector<vec> normalarr;
	vector<vec> interpointarr;
	for (vec V : Vj) {
		if (find(ckedvec.begin(),ckedvec.end(),V) != ckedvec.end())continue;
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

			//IndexPointForce_f.push_back(IVV(indB, X.interpoint, X.normal*100));
			//IndexPointForce_f.push_back(IVV(indA, X.interpoint, vec() - X.normal*100));
		}
	}

	if (interpointnum == 0)return interpointnum;
	vec normal_tot;
	vec interpoint_tot;

	for (vec V : normalarr) {
		normal_tot += V;
	}
	for (vec V : interpointarr) {
		interpoint_tot += V;
	}

	normal_tot /= normalarr.size();
	interpoint_tot /= interpointarr.size();
	
	normal_tot = normal_tot/normal_tot.norm();

	double coe = collision_coefficient(A, B, normal_tot, interpoint_tot);//indexpointForce�� ������ ���� �׳� ���⿡ ���� �ﰢ������ update�� �ǵ��� �ϴ� �Լ��� �����߰ڴ�. �ʿ䰡 ���� ������ �浹���� ������ �߻���.
	update_Object(B, interpoint_tot, coe * normal_tot / dt, dt);
	update_Object(A, interpoint_tot, -coe * normal_tot / dt, dt);// stl�� �װ� �����..
	//IndexPointForce_f.push_back(IVV(indB, interpoint_tot, coe * normal_tot / dt));
	//IndexPointForce_f.push_back(IVV(indA, interpoint_tot, -coe * normal_tot / dt));
	
	return interpointnum;
}

void Force::Collision_Line_Line(int indA, int indB, Object A, Object B, double dt) {
	vector <Triangle> Ti;
	vector <vec> Vj;
	vector <Line> Lj;

	for (int k = 0; k < A.C.indnum / 3; k++) {
		vec V[3];
		for (int l = 0; l < 3; l++) {
			for (int w = 0; w < 3; w++) {
				V[l].V[w] = A.C.vertices[A.C.indices[k * 3 + l] * 6 + w];
			}
		}
		Ti.push_back(Triangle(A.pos_b_pos_f(V[0]), A.pos_b_pos_f(V[1]), A.pos_b_pos_f(V[2])));
	}

	for (int k = 0; k < B.C.indnum; k++) {
		vec V;
		for (int w = 0; w < 3; w++) {
			V.V[w] = B.C.vertices[B.C.indices[k] * 6 + w];
		}
		Vj.push_back(B.pos_b_pos_f(V));
	}
	vec cubecenteri = A.pos_f;

	for (int i = 0; i < Vj.size()/3; i++) {
		Lj.push_back(Line(Vj[i * 3 + 0], Vj[i * 3 + 1]));
		Lj.push_back(Line(Vj[i * 3 + 1], Vj[i * 3 + 2]));
		Lj.push_back(Line(Vj[i * 3 + 2], Vj[i * 3 + 0]));
	}

	//�ʿ��� ��� �� ����.

	for (Line L : Lj) {
		Triangle tri[2];
		int trind = 0;
		for (Triangle T : Ti) {
			if (Intersect::triline(T, L)) {
				tri[trind] = T;
				trind++;
			}
			if (trind == 2)break;
		}
		if (trind == 2) {
			Line L1 = Geomfunc::shareLineTT(tri[0], tri[1]);
			BVV X = Intersect::lineline(L, L1);
			IndexPointForce_f.push_back(IVV(indB, X.interpoint, X.normal*100));
			IndexPointForce_f.push_back(IVV(indA, X.interpoint, vec()-X.normal*100));
		}
	}
}

void Force::Gravity(int ind, Object* ob) {
	vec g = vec(0, 0, -gravity_acceleration);
	IndexPointForce_f.push_back(IVV(ind, ob->pos_f, ob->m * g));
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

void Force::Gravity_Object(vector<Object> OB) {
	for (int i = 0; i < OB.size(); i++) {
		for (int j = i + 1; j < OB.size(); j++) {
			vec r = OB[j].pos_f - OB[i].pos_f;
			vec F = gravitational_constant*r/r.norm()/r.dot(r);
			IndexPointForce_f.push_back(IVV(i, OB[i].pos_f, F));
			IndexPointForce_f.push_back(IVV(j, OB[j].pos_f, vec()-F));
		}
	}
}

void Force::GenIndexPointForce_f(vector <Object> OB, double dt){
    //vector <vec> B = trans_sf();
	IndexPointForce_f.clear();


	for (int i = 0; i < OB.size(); i++) {
		//Gravity(i, &OB[i]);
	}
	Gravity_Object(OB);

	//Colide Force
	for (int i = 0; i < OB.size(); i++) {
		for (int j = i + 1; j < OB.size(); j++) {
			if (CanCollide(&OB[i], &OB[j]) == false)continue;
			int collidenum = 0;
			collidenum += Collision_Triangle_Point(i, j, &OB[i], &OB[j], dt);
			collidenum += Collision_Triangle_Point(j, i, &OB[j], &OB[i], dt);
			//collision �Ŀ� �� ��ü�� �ſ� ��¦ ���������. �ȱ׷��� �ʹ� ���� collision���� �ν��ؼ� ���� ����µ�.->�׳� �� ��ü�� �ٰ������� �Ǵ��ؼ� �浹�Լ��� �����ϴ� ���� �´µ�.
			if (collidenum) {
				continue;
			}
			Collision_Line_Line(i, j, OB[i], OB[j], dt);
		}
	}
	return;
    //return A;
}

vector <vec> Force::Force_f(vector <Object> OB) {
	vector <vec> A = vector<vec>(OB.size());
	for (IVV ivv : IndexPointForce_f) {
		A[ivv.ind] += ivv.force;
	}
	return A;
}

vector <vec> Force::Torque_f(vector <Object> OB){
    vector <vec> A = vector<vec>(OB.size());
	for (IVV ivv : IndexPointForce_f) {
		A[ivv.ind] += (ivv.workingpoint - OB[ivv.ind].pos_f) * ivv.force;
	}
    return A;//return B;
	return A;
	return A;
}