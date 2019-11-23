#include "Visualize.h"
Camera Visualize::camera = Camera(glm::vec3(1.f, 0.f, 15.f));
float Visualize::lastX = SCR_W / 2.0f;
float Visualize::lastY = SCR_H / 2.0f;
bool Visualize::firstMouse = true;

float Visualize::deltaTime = 0.f;
float Visualize::lastFrame = 0.f;

vector <int> Visualize::indicesize;
vector <int> Visualize::verticessize;
vector <string> Visualize::geomtype;
vector <int> Visualize::allindices;
vector <float> Visualize::allvertices;