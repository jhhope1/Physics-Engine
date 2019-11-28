#ifndef VISUALISE_H
#define VISUALISE_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PhyObject.h"
#include"camera.h"
#include "Object.h"
using namespace std;
const double LINE_WIDTH = 1.5;

class Visualize {
public:
	static const int SCR_W = 800, SCR_H = 600;
	static int axisnum;
	static double axis_unit_x;
	static double axis_unit_y;
	static double axis_thick;

	static Camera camera;// = Camera(glm::vec3(0.f, 0.f, 3.f));
	static float lastX;// = SCR_W / 2.0f;
	static float lastY;// = SCR_H / 2.0f;
	static bool firstMouse;// = true;

	static float deltaTime;// = 0.f;
	static float lastFrame;// = 0.f;

	static vector <int> indicesize;
	static vector <int> verticessize;
	static vector <string> geomtype;
	static vector <int> allindices;
	static vector <float> allvertices;

	static void draw_axis(unsigned int *VAO_axis, Shader *ourShader) {
		ourShader->use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
		ourShader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader->setMat4("view", view);

		//ourmodel
		glm::mat4 M4 = glm::mat4();
		for (int i = 0; i < 4; i++) {
			M4[i][i] = 1;
		}

		unsigned int modelLoc = glGetUniformLocation(ourShader->ID, "model");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(M4));


		glBindVertexArray(*VAO_axis);

		glDrawElements(GL_LINES, 8 * axisnum , GL_UNSIGNED_INT, (void*)(0));

		glBindVertexArray(0);
	}

	static void pushObject(Object ob) {
		pushcube(ob.C.vertices, ob.C.indices, ob.C.vertexnum, ob.C.indnum, "Triangle");
	}
	static void visual_Object_init(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO) {
		float* vertices = new float[allvertices.size()];
		unsigned int* indices = new unsigned int[allindices.size()];
		copyVerticesIndices(vertices, indices);

		glGenVertexArrays(1, VAO);
		glGenBuffers(1, VBO);
		glGenBuffers(1, EBO);

		glBindVertexArray(*VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * allvertices.size(), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * allindices.size(), indices, GL_STATIC_DRAW);



		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete[] vertices;
		delete[] indices;
	}

	static void render(vec pos, tensor R, Shader* ourShader, unsigned int VAO) {

		// get matrix's uniform location and set matrix
		//ourShader->use();
				// be sure to activate shader when setting uniforms/drawing objects

		//pos of light -> should change to static var
		glm::vec3 lightPos(10.f, 10.f, 10.f);

		ourShader->use();
		ourShader->setVec3("light.position", lightPos);
		ourShader->setVec3("viewPos", camera.Position);

		// light properties
		glm::vec3 lightColor = glm::vec3(1,1,1);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		ourShader->setVec3("light.ambient", ambientColor);
		ourShader->setVec3("light.diffuse", diffuseColor);
		ourShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		ourShader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		ourShader->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		ourShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		ourShader->setFloat("material.shininess", 32.0f);

		/////////////////////////

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
		ourShader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader->setMat4("view", view);

		glBindVertexArray(VAO);

		ourModel(pos, R, ourShader);

		glBindVertexArray(0);
	}

	static GLFWwindow* mkwindow() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "yes", NULL, NULL);

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glEnable(GL_DEPTH_TEST);

		return window;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
	}
	static void processinput(GLFWwindow* window, float deltaTime) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	static void clearwindow() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.ProcessMouseScroll(yoffset);
	}

	static void ourModel(vec pos, tensor R, Shader* ourShader) {
		glm::mat4 model = glm::mat4(1.f);

		model = glm::translate(model, glm::vec3(pos.V[0], pos.V[1], pos.V[2]));
		glm::mat4 M4 = glm::mat4();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				M4[i][j] = R.M[i][j];
			}
		}
		M4[3][3] = 1;
		model =  model*M4;//순서 주의 rot을 먼저 한 뒤에 위치를 박아야함. world space에 올리는거라 그렇지 않을까 싶다.

		unsigned int modelLoc = glGetUniformLocation(ourShader->ID, "model");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0));
	}
	static void pushcube(float* vertices, unsigned int* indices, unsigned int vs, unsigned int is, string S) {
		allindices.clear();
		allvertices.clear();
		//unsigned int OFFSET = allindices.size();

		if (vs == 0 || is == 0) {
			cout << "pushcube: vertices, indices are empty\n";
		}
		string gt = S;
		verticessize.push_back(vs);
		indicesize.push_back(is);
		geomtype.push_back(S);

		for (int i = 0; i < vs; i++) {
			allvertices.push_back(vertices[i]);
		}
		for (int i = 0; i < is; i++) {
			allindices.push_back(indices[i]);// + OFFSET);
		}
	}

	static void push_axis() {
		allvertices.clear();
		allindices.clear();
		int OFFSET = 0;
		for (int j = -axisnum ; j < axisnum; j++) {
			OFFSET = allindices.size();
			line L = line(vec(axis_unit_x * j, -100, 0), vec(axis_unit_x * j, 100, 0));
			for (int i = 0; i < L.vertexnum; i++) {
				allvertices.push_back(L.vertices[i]);
			}
			for (int i = 0; i < L.indnum; i++) {
				allindices.push_back(L.indices[i]+OFFSET);
			}
		}

		for (int j = -axisnum; j < axisnum; j++) {
			OFFSET = allindices.size();
			line L = line(vec(-100, axis_unit_y * j, 0), vec(100, axis_unit_y * j, 0));

			for (int i = 0; i < L.vertexnum; i++) {
				allvertices.push_back(L.vertices[i]);
			}
			for (int i = 0; i < L.indnum; i++) {
				allindices.push_back(L.indices[i]+OFFSET);
			}
		}
	}


	static void copyVerticesIndices(float* vertices, unsigned int* indices) {
		for (int i = 0; i < allvertices.size(); i++) {
			*(vertices + i) = allvertices[i];
		}
		for (int i = 0; i < allindices.size(); i++) {
			*(indices + i) = allindices[i];
		}
	}

	static void terminate(vector <unsigned int> VAO, vector <unsigned int> VBO, vector <unsigned int >EBO) {
		for(int i=0 ; i<VAO.size() ; i++)
			glDeleteVertexArrays(1, &VAO[i]);
		for (int i = 0; i < VBO.size(); i++)
			glDeleteBuffers(1, &VBO[i]);
		for (int i = 0; i < EBO.size(); i++)
			glDeleteBuffers(1, &EBO[i]);
		glfwTerminate();
	}
};

#endif