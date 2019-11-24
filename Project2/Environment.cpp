#include "Environment.h"
Environment::Environment(double dt0, vector<Object*> OB0, bool realtimerender) {
	dt = dt0;
	for (Object* ob : OB0) {
		OB.push_back(ob);//ㅋㅋ이렇게 하면 안될걸 알지만 아몰랑 난 입문자니깐~ 응 안돼.
	}
	this->realtimerender = realtimerender;
}
Environment::Environment(double dt0, vector<Object*> OB0) {
	Environment(dt0,OB0,false);
}
Environment::Environment(double dt, bool realtimerender) {
	this->dt = dt;
	this->realtimerender = realtimerender;
}
Environment::Environment(double dt0){
	Environment(dt0, false);
}
Environment::Environment(){
    Environment(1e-4);//default dt = 1e-4 [s]
}

void Environment::push_back(Object* Object0){
    OB.push_back(Object0);
}

void Environment::push_back(vector<Object*> OBJECT) {
	for (Object *ob : OBJECT) {
		push_back(ob);
	}
}


void Environment::step(){
	double pre_E = KineticE();
	vec pre_mom = momentum();
	vec pre_angmom = AngMom();
	if (Force::GenIndexPointForce_f(OB, Environment::dt)) {
		cout << pre_E << " " << KineticE() << "\n";
		cout << pre_mom << " " << momentum() << "\n";
		cout << pre_angmom << " " << AngMom() << "\n";

	}
    //vector<vec> F_f = Force::Force_f(OB);
    //vector<vec> T_f = Force::Torque_f(OB);
	for (int i = 0; i < OB.size(); i++) {
		OB[i]->Object_update_pos_rotmat(dt);//업데이트를 너무 여러번 해서 발생하는 문제같기도 함. 문제를 어떻게 해결해야할지 모르겠음.
		//위치랑 rotmat만 놔두고 다 업데이트 한 다음에 이걸 따로 업데이트 하면 안되나?
    }
}

void Environment::simulate(double t){
    double nowt = 0;
	GLFWwindow* window = Visualize::mkwindow();
	vector<Shader> ourShader;

	for (int i = 0; i < OB.size(); i++)
		ourShader.push_back(Shader("material.vs", "material.fs"));
	vector <unsigned int> VBO(OB.size()),VAO(OB.size()),EBO(OB.size());
	//Shader lampShader("lamp.vs", "lamp.fs");

	

	for (int i = 0; i < OB.size(); i++) {
		Visualize::pushObject(*(OB[i]));
		Visualize::visual_Object_init(&VAO[i], &VBO[i], &EBO[i]);
	}

	for(; (nowt<t)&&(!glfwWindowShouldClose(window)) ; nowt+=dt){

		step();
		//if (int(nowt * 1000) % 1000 == 0) {
			//cout << KineticE()<<"\n";
			//cout << OB[0].Energy() << "\n";
			//cout << OB[0].AngMom_f() << "\n";
		//}
		float currentFrame = glfwGetTime();
		Visualize::deltaTime = currentFrame - Visualize::lastFrame;
		Visualize::lastFrame = currentFrame;
		if (realtimerender) {
			dt = Visualize::deltaTime;
		}


		Visualize::processinput(window, Visualize::deltaTime);
		Visualize::clearwindow();

		for (int i = 0; i < OB.size(); i++)
			Visualize::render(OB[i]->pos_f, OB[i]->rotmat_if * OB[i]->rotmat_bi.transpose(), &ourShader[i], window, VAO[i]);	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Visualize::terminate(VAO, VBO, EBO);
}

vec Environment::AngMom() {
	vec angmom;
	for (Object *ob :OB) {
		angmom += ob->AngMom_f();
		angmom += ob->pos_f * ob->v_f * ob->m;
	}
	return angmom;
}
vec Environment::momentum() {
	vec mom;
	for (Object* ob : OB) {
		mom += ob->v_f * ob->m;
	}
	return mom;
}

double Environment::KineticE() {
	double K=0;
	for (Object* ob : OB) {
		K += ob->KineticE();
	}
	return K;
}

void Environment::print(int stn){
    int obnum=0;
    cout<<"\n\n\nReal time = "<<stn*dt<<"\n";
    //cout<<"step "<<stn<<"\n";
    for(Object* ob: OB){
        obnum++;
        cout<<"\n\nObject "<<obnum<<"\n";
        cout<<*ob;
    }
}