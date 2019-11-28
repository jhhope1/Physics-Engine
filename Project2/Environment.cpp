#include "Environment.h"
Environment::Environment(double dt0, vector<Object*> OB0, bool realtimerender) {
	dt = dt0;
	for (Object* ob : OB0) {
		OB.push_back(ob);
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
	Force::GenIndexPointForce_f(OB, Environment::dt);

	for (int i = 0; i < OB.size(); i++) {
		OB[i]->Object_update_pos_rotmat(dt);
    }
}

void Environment::simulate(double t){
    double nowt = 0;
	GLFWwindow* window = Visualize::mkwindow();
	vector<Shader> ourShader;

	for (int i = 0; i < OB.size(); i++)
		ourShader.push_back(Shader("material.vs", "material.fs"));
	vector <unsigned int> VBO(OB.size()),VAO(OB.size()),EBO(OB.size());

	unsigned int VAO_axis, VBO_axis, EBO_axis;
	Shader ourShader_axis = Shader("shader.vs", "shader.fs");
	if(axis)glLineWidth(Visualize::axis_thick);

	if(axis)Visualize::push_axis();
	if(axis)Visualize::visual_Object_init(&VAO_axis, &VBO_axis, &EBO_axis);

	for (int i = 0; i < OB.size(); i++) {
		Visualize::pushObject(*(OB[i]));
		Visualize::visual_Object_init(&VAO[i], &VBO[i], &EBO[i]);
	}

	for(; (nowt<t)&&(!glfwWindowShouldClose(window)) ; nowt+=dt){

		step();

		float currentFrame = glfwGetTime();
		Visualize::deltaTime = currentFrame - Visualize::lastFrame;
		Visualize::lastFrame = currentFrame;
		if (realtimerender) {
			dt = Visualize::deltaTime;
		}

		Visualize::processinput(window, Visualize::deltaTime);
		Visualize::clearwindow();


		if(axis)Visualize::draw_axis(&VAO_axis, &ourShader_axis);
		for (int i = 0; i < OB.size(); i++)
			Visualize::render(OB[i]->pos_f, (OB[i]->rotmat_if).transpose(), &ourShader[i], VAO[i]);	
		
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

void Environment::print(double t){
    int obnum=0;
    cout<<"\n\n\nReal time = "<<t<<"\n";
    for(Object* ob: OB){
        obnum++;
        cout<<"\n\nObject "<<obnum<<"\n";
        cout<<*ob;
    }
}