#include"Visualize.h"
using namespace std;
int main() {
	cout << "asdf";
	cube C = cube();
	pushcube(C.vertices,C.indices,C.vertexnum,C.indnum,"Triangle");//모든 물체는 직육면체.... 어떻게 할 수가 없다.
	visualize();
}