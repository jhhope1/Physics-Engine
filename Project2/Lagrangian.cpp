#include "Lagrangian.h"

vector<double> f(vector<Object*> OB) {//천장에 꼭대기점이 붙어있음
	vector <double> f;
	//구속조건 없음
	//return f;
	//chain
	for (int i = 0; i < OB.size() - 1; i++) {
		vec A = OB[i]->pos_b_pos_f(vec(0, 0, OB[i]->cubesize.V[2] / 2)) - OB[i + 1]->pos_b_pos_f(vec(0, 0, -OB[i + 1]->cubesize.V[2] / 2));
		for (int j = 0; j < 3; j++) {
			f.push_back(A.V[j]);
		}
	}
	return f;
	//팽이 구속조건
	for (int i = 0; i < OB.size(); i++) {
		vec A = OB[i]->pos_b_pos_f(vec(0, 0, -1));
		for (int j = 0; j < 3; j++) {
			f.push_back(A.V[j]);
		}
	}
	return f;
	//n중 진자 구속조건
	for (int i = 0; i < OB.size()-1; i++) {
		vec A = OB[i]->pos_b_pos_f(vec(0, 0, OB[i]->cubesize.V[2] / 2)) - OB[i + 1]->pos_b_pos_f(vec(0, 0, -OB[i + 1]->cubesize.V[2] / 2));
		for (int j = 0; j < 3; j++) {
			f.push_back(A.V[j]);
		}
	}
	vec A = OB[OB.size() - 1]->pos_b_pos_f(vec(0, 0, OB[OB.size() - 1]->cubesize.V[2] / 2));
	for (int i = 0; i < 3; i++) {
		f.push_back(A.V[i]);
	}
	return f;

	//n차 용수철 진자 구속조건->천장에 매달리기
	for (int i = 0; i < OB.size(); i++) {
		vec A = OB[i]->pos_b_pos_f(vec(0, 0, OB[i]->cubesize.V[2] / 2));
		for(int j=0 ; j<3 ; j++)f.push_back(A.V[j]);	
	}
	return f;
}
vector<double> operator +(vector<double> f, vector<double> g) {
	vector <double> h;
	for (int i = 0; i < f.size(); i++) {
		h.push_back(f[i] + g[i]);
	}
	return h;
}

vector<double> operator -(vector<double> f, vector<double> g) {
	vector <double> h;
	for (int i = 0; i < f.size(); i++) {
		h.push_back(f[i] - g[i]);
	}
	return h;
}

MatrixXd dfdq(vector<Object*> OBtemp, int lambdan, vector<double> f_origin) {
	MatrixXd B = MatrixXd(lambdan, 6 * OBtemp.size());
	int indbj = 0;
	for (int i = 0; i < OBtemp.size(); i++) {
		vec tempvec = OBtemp[i]->pos_f;

		for (int j = 0; j < 3; j++) {//x_i+=dX의 미분값
			OBtemp[i]->pos_f.V[j] += LAGRANGIAN_DX;
			vector <double> T = f(OBtemp) - f_origin;
			for (int k = 0; k < T.size(); k++) {
				B(k, indbj) = T[k] / LAGRANGIAN_DX;
			}
			OBtemp[i]->pos_f = tempvec;
			indbj++;
		}

		tensor temptensor = OBtemp[i]->rotmat_bf;
		for (int j = 0; j < 3; j++) {//theta_x_i+=dX의 미분값
			OBtemp[i]->rotmat_bf *= tensor(j, LAGRANGIAN_DX);//bi = bi*pb;
			vector <double> T = f(OBtemp) - f_origin;
			for (int k = 0; k < T.size(); k++) {
				B(k, indbj) = T[k] / LAGRANGIAN_DX;
			}
			OBtemp[i]->rotmat_bf = temptensor;
			indbj++;
		}
	}
	return B;
}

void DLDQ(VectorXd* dLdq, vector<Object*> OB){
	int dLdqn = 0;
	for (Object *ob : OB) {
		for (int i = 0; i < 3; i++) {
			(*dLdq)(dLdqn) = ob->Force_f.V[i];
			dLdqn++;
		}
		for (int i = 0; i < 3; i++) {
			(*dLdq)(dLdqn) = ob->Torque_b.V[i];
			dLdqn++;
		}
	}
}

void DLDQPM(VectorXd* dLdqPm, VectorXd* dLdq, vector<Object*> OB) {
	unsigned int ind = 0;
	for (int i = 0; i < OB.size(); i++) {
		for (int j = 0; j < 3; j++) {
			(*dLdqPm)(ind) = (*dLdq)(ind) / OB[i]->m;
			ind++;
		}
		for (int j = 0; j < 3; j++) {
			(*dLdqPm)(ind) = (*dLdq)(ind) / OB[i]->Ib.V[j];
			ind++;
		}
	}
}

void MAKEA(MatrixXd* A, MatrixXd* B, vector<Object*>OB, int lambdan) {
	*A = B->transpose();
	for (int i = 0; i < OB.size(); i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < lambdan; k++) {
				(*A)(i * 6 + j, k) /= OB[i]->m;
			}
		}
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < lambdan; k++) {
				(*A)(i * 6 + 3 + j, k) /= OB[i]->Ib.V[j];
			}
		}
	}
}

void QDOT(VectorXd* qdot, vector<Object*> OB) {
	int indqdot = 0;
	for (int i = 0; i < OB.size(); i++) {
		for (int j = 0; j < 3; j++) {
			(*qdot)(indqdot) = OB[i]->v_f.V[j];
			indqdot++;
		}
		for (int j = 0; j < 3; j++) {
			(*qdot)(indqdot) = OB[i]->w_b.V[j];
			indqdot++;
		}
	}
}

void MAKEC(VectorXd *C, MatrixXd B,vector<Object*> OB,vector<Object*> OBtemp, int lambdan,VectorXd qdot) {
	MatrixXd TEMP = MatrixXd::Zero(lambdan, 6 * OB.size());
	for (int i = 0; i < OB.size(); i++) {
		for (int j = 0; j < 3; j++) {
			OBtemp[i]->pos_f.V[j] += LAGRANGIAN_DX;
			vector<double> f_temp = f(OBtemp);
			MatrixXd T = dfdq(OBtemp, lambdan, f_temp);
			TEMP += (T - B) / LAGRANGIAN_DX * OBtemp[i]->v_f.V[j];
			OBtemp[i]->pos_f.V[j] = OB[i]->pos_f.V[j];
		}
		for (int j = 0; j < 3; j++) {
			OBtemp[i]->rotmat_bf *= tensor(j, LAGRANGIAN_DX);//bi = bi*pb;
			vector<double> f_temp = f(OBtemp);
			MatrixXd T = dfdq(OBtemp, lambdan, f_temp);
			TEMP += (T - B) / LAGRANGIAN_DX * OBtemp[i]->w_b.V[j];
			OBtemp[i]->rotmat_bf = OB[i]->rotmat_bf;
		}
	}
	*C = TEMP * qdot;//size = lambdan
}

VectorXd Lagrangian::qddot_FT(vector<Object*> OB) {//qi 순서: OB[i], x1, x2, x3, x1_b회전각, x2_b회전각, x3_b회전각 // 행렬 이름은 보고서 변수정의를 따릅니다.
	//변수 개수 6N개, lambda 개수 lambdan개.
	//강제 운동은 일단은 무시한다. 힘 작용하는 것만
	VectorXd dLdq(6 * OB.size());
	DLDQ(&dLdq, OB);

	VectorXd dLdqPm(6 * OB.size());
	DLDQPM(&dLdqPm, &dLdq,OB);

	int lambdan = f(OB).size();

	if (lambdan == 0) {
		return dLdqPm;
	}

	vector<Object*> OBtemp;
	for (Object* ob : OB) {
		OBtemp.push_back(new Object());
	}
	for (int i = 0; i < OB.size(); i++) {
		*OBtemp[i] = *OB[i];
	}

	vector<double> f_origin = f(OB);
	MatrixXd B = dfdq(OBtemp, lambdan, f_origin);



	MatrixXd A(6*OB.size(),lambdan);
	MAKEA(&A,&B,OB,lambdan);

	VectorXd qdot = VectorXd(OB.size()*6);
	QDOT(&qdot, OB);


	VectorXd lambda = -(B * A).inverse()* (B * dLdqPm);

	//cout << lambda.norm() << "\n";
	if(lambda.norm()>1)cout << lambda<<"\n";
	//cout << "\n\n";
	//cout << "dLdq = \n" << dLdq << "\n";
	//cout << "dLdqPm = \n" << dLdqPm << "\n";
	//cout << "lambda = \n" << lambda << "\n";
	VectorXd qddot = (A * lambda + dLdqPm);
	//cout << "B = \n"<<B << "\n";
	//cout<<"qddot = \n"<<qddot<<"\n";

	return qddot;
}

VectorXd Lagrangian::qddot_without_FT(vector<Object*> OB) {//qi 순서: OB[i], x1, x2, x3, x1_b회전각, x2_b회전각, x3_b회전각 // 행렬 이름은 보고서 변수정의를 따릅니다.
	//변수 개수 6N개, lambda 개수 lambdan개.
	//강제 운동은 일단은 무시한다. 힘 작용하는 것만
	int lambdan = f(OB).size();
	if (lambdan == 0) {
		VectorXd V = VectorXd::Zero(OB.size() * 6);
		int ind = 0;
		for (Object* ob : OB) {
			vec deltawb = (ob->IWW_b() / ob->Ib);
			ind += 3;
			for (int i = 0; i < 3; i++) {
				V(ind) = deltawb.V[i];
				ind++;
			}
		}
		return V;
	}

	vector<Object*> OBtemp;
	for (Object* ob : OB) {
		OBtemp.push_back(new Object());
	}
	for (int i = 0; i < OB.size(); i++) {
		*OBtemp[i] = *OB[i];
	}

	vector<double> f_origin = f(OB);
	MatrixXd B = dfdq(OBtemp, lambdan, f_origin);

	MatrixXd A(6 * OB.size(), lambdan);
	MAKEA(&A, &B, OB, lambdan);

	VectorXd qdot = VectorXd(OB.size() * 6);
	QDOT(&qdot, OB);

	VectorXd C(lambdan);
	MAKEC(&C, B, OB, OBtemp, lambdan, qdot);

	VectorXd lambda = -(B * A).inverse() * C;
	VectorXd qddot = A * lambda;

	return qddot;
}