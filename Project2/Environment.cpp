#include "Environment.h"
Environment::Environment(){
    dt = 1e-4;//default dt = 1e-4 [s]
}
Environment::Environment(double dt0){
    dt = dt0;
}
Environment::Environment(double dt, bool realtimerender) {
	this->dt = dt;
	this->realtimerender = realtimerender;
}
Environment::Environment(double dt0, vector<Object> OB0) {
	dt = dt0;
	OB = OB0;
}
Environment::Environment(double dt0, vector<Object> OB0, bool realtimerender) {
	dt = dt0;
	OB = OB0;
	this->realtimerender = realtimerender;
}

void Environment::push_back(Object Object0){
    OB.push_back(Object0);
}

void Environment::push_back(vector<Object> OBJECT) {
	for (Object ob : OBJECT) {
		push_back(ob);
	}
}


void Environment::step(){
	Force::GenIndexPointForce_f(OB, Environment::dt);
    vector<vec> F_f = Force::Force_f(OB);
    vector<vec> T_f = Force::Torque_f(OB);
	if (Force::IndexPointForce_f.size() != 2&&Force::IndexPointForce_f.size()!=8) {
		int a = Force::IndexPointForce_f.size();
		int b = a;
	}
    int N = OB.size();
	for (int i = 0; i < N; i++) {
		OB[i].Object_update(F_f[i], T_f[i], dt);
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
		Visualize::pushObject(OB[i]);
		Visualize::visual_Object_init(&VAO[i], &VBO[i], &EBO[i]);
	}
	for(; (nowt<t)&&(!glfwWindowShouldClose(window)) ; nowt+=dt){

		step();
		//if (int(nowt * 1000) % 1000 == 0) {
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
			Visualize::render(OB[i].pos_f, (OB[i].rotmat_if * OB[i].rotmat_bi).transpose(), &ourShader[i], window, VAO[i]);	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Visualize::terminate(VAO, VBO, EBO);
}

void Environment::print(int stn){
    int obnum=0;
    cout<<"\n\n\nReal time = "<<stn*dt<<"\n";
    //cout<<"step "<<stn<<"\n";
    for(Object ob:OB){
        obnum++;
        //cout<<"\n\nObject "<<obnum<<"\n";
        cout<<ob;
    }
}