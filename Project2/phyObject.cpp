#include"PhyObject.h"
#include"Geometry.h"
cube::cube() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	unsigned int indices[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35
	};
	for (int i = 0; i < vertexnum; i++) {
		this->vertices[i] = vertices[i];
	}
	for (int i = 0; i < indnum; i++) {
		this->indices[i] = indices[i];
	}
}
cube::cube(vec resize) {
	cube C = cube();
	for (int i = 0; i < vertexnum; i += stride) {
		for (int j = 0; j < 3; j++) {
			vertices[i + j] = C.vertices[i + j] * resize.V[j];
		}
		for (int j = 3; j < stride; j++) {
			vertices[i + j] = C.vertices[i + j];
		}
	}
	for (int i = 0; i < indnum; i++) {
		indices[i] = C.indices[i];
	}
}
unsigned int cube::vertexnum = 36 * 6;
unsigned int cube::indnum = 36;
unsigned int cube::stride = 6;


line::line() {
	float vertices[] = {
		-0.5f, 0.f, 0.f, 1.f, 0.f, 0.f,
		 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f
	};
	unsigned int indices[] = {
		0,1
	};
	for (int i = 0; i < vertexnum; i++) {
		this->vertices[i] = vertices[i];
	}
	for (int i = 0; i < indnum; i++) {
		this->indices[i] = indices[i];
	}
}

unsigned int top::vertexnum = 18 * 100;
unsigned int top::indnum = 3*100;
unsigned int top::stride = 6;

top::top() {
	int n = 100;
	double z = 1;
	double R = 1;
	vec bottom(0, 0, -1);
	vec A[100];
	for (int i = 0; i < n; i++) {
		A[i] = vec(sin(i * 2 * PI / n) * R, cos(i * 2 * PI / n), z);
	}

	Triangle T[100];
	for (int i = 0; i < n-1; i++) {
		T[i].a = bottom;
		T[i].b = A[i];
		T[i].c = A[i + 1];
	}
	T[n-1].a = bottom;
	T[n-1].b = A[n - 1];
	T[n-1].c = A[0];
	for (int i = 0; i < n; i++) {
		vec N = (T[i].a - T[i].b) * (T[i].c - T[i].b);
		N /= N.norm();
		for (int j = 0; j < 3; j++) {
			vertices[18*i+j] = T[i].a.V[j];
		}
		for (int j = 0; j < 3; j++) {
			vertices[18*i+6+j] = T[i].b.V[j];
		}
		for (int j = 0; j < 3; j++) {
			vertices[18*i+12+j] = T[i].c.V[j];
		}
		if (i != 0) {
			for (int j = 0; j < 3; j++) {
				vertices[18 * i + j + 3] = N.V[j];
			}
			for (int j = 0; j < 3; j++) {
				vertices[18 * i + 6 + j + 3] = N.V[j];
			}
			for (int j = 0; j < 3; j++) {
				vertices[18 * i + 12 + j + 3] = N.V[j];
			}
		}
	}
	for (int i = 0; i < indnum; i++) {
		indices[i] = i;
	}
}

line::line(vec s, vec e) {
	for (int i = 0; i < 3; i++) {
		vertices[i] = s.V[i];
	}
	for (int i = 0; i < 3; i++) {
		vertices[i+6] = e.V[i];
	}
	vertices[3] = 1;
	vertices[9] = 1;
	indices[0] = 0;
	indices[1] = 1;
}
unsigned int line::vertexnum = 2 * 6;
unsigned int line::indnum = 2;
unsigned int line::stride = 6;

