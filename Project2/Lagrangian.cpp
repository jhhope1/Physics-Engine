#include "Lagrangian.h"

vector<double> f(vector<Object*> OB) {//õ�忡 ��������� �پ�����
	vector <double> f;
	//return f;
	/*
	for (int i = 0; i < OB.size(); i++) {
		vec A = OB[i]->pos_b_pos_f(vec(0, 0, OB[i]->cubesize.V[2] / 2));
		for(int j=0 ; j<3 ; j++)f.push_back(A.V[j]);	
	}
	return f;
	*/
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

		for (int j = 0; j < 3; j++) {//x_i+=dX�� �̺а�
			OBtemp[i]->pos_f.V[j] += LAGRANGIAN_DX;
			vector <double> T = f(OBtemp) - f_origin;
			for (int k = 0; k < T.size(); k++) {
				B(k, indbj) = T[k] / LAGRANGIAN_DX;
			}
			OBtemp[i]->pos_f = tempvec;
			indbj++;
		}

		tensor temptensor = OBtemp[i]->rotmat_if;
		for (int j = 0; j < 3; j++) {//theta_x_i+=dX�� �̺а�
			OBtemp[i]->rotmat_if *= tensor(j, LAGRANGIAN_DX);//bi = bi*pb;
			vector <double> T = f(OBtemp) - f_origin;
			for (int k = 0; k < T.size(); k++) {
				B(k, indbj) = T[k] / LAGRANGIAN_DX;
			}
			OBtemp[i]->rotmat_if = temptensor;
			indbj++;
		}
	}
	return B;
}

void DLDQ(VectorXd* dLdq, vector<VV> FT){
	int dLdqn = 0;
	for (VV ft : FT) {
		for (int i = 0; i < 3; i++) {
			(*dLdq)(dLdqn) = ft.Force.V[i];
			dLdqn++;
		}
		for (int i = 0; i < 3; i++) {
			(*dLdq)(dLdqn) = ft.Torque.V[i];
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
			OBtemp[i]->rotmat_if *= tensor(j, LAGRANGIAN_DX);//bi = bi*pb;
			vector<double> f_temp = f(OBtemp);
			MatrixXd T = dfdq(OBtemp, lambdan, f_temp);
			TEMP += (T - B) / LAGRANGIAN_DX * OBtemp[i]->w_b.V[j];
			OBtemp[i]->rotmat_if = OB[i]->rotmat_if;
		}
	}
	*C = TEMP * qdot;//size = lambdan
}

VectorXd Lagrangian::qddot_FT(vector<Object*> OB, vector <VV> FT) {//qi ����: OB[i], x1, x2, x3, x1_bȸ����, x2_bȸ����, x3_bȸ���� // ��� �̸��� ������ �������Ǹ� �����ϴ�.
	if (OB.size() != FT.size()) {
		cout << "Warning in Lagrangian::qddot_FT OB.size()!=FT.size()\n";
	}
	//���� ���� 6N��, lambda ���� lambdan��.
	//���� ��� �ϴ��� �����Ѵ�. �� �ۿ��ϴ� �͸�

	VectorXd dLdq(6 * OB.size());
	DLDQ(&dLdq, FT);

	VectorXd dLdqPm(6 * OB.size());
	DLDQPM(&dLdqPm, &dLdq, OB);

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
	//cout << "\n\n";
	//cout << "dLdq = \n" << dLdq << "\n";
	//cout << "dLdqPm = \n" << dLdqPm << "\n";
	//cout << "lambda = \n" << lambda << "\n";
	VectorXd qddot = (A * lambda + dLdqPm);
	//cout << "B = \n"<<B << "\n";
	//cout<<"qddot = \n"<<qddot<<"\n";

	return qddot;
}

VectorXd Lagrangian::qddot_without_FT(vector<Object*> OB) {//qi ����: OB[i], x1, x2, x3, x1_bȸ����, x2_bȸ����, x3_bȸ���� // ��� �̸��� ������ �������Ǹ� �����ϴ�.
	//���� ���� 6N��, lambda ���� lambdan��.
	//���� ��� �ϴ��� �����Ѵ�. �� �ۿ��ϴ� �͸�
	int lambdan = f(OB).size();
	if (lambdan == 0) {
		return VectorXd::Zero(OB.size()*6);
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