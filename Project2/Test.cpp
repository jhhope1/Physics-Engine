#include"Visualize.h"
using namespace std;
int main() {
	cout << "asdf";
	cube C = cube();
	pushcube(C.vertices,C.indices,C.vertexnum,C.indnum,"Triangle");//��� ��ü�� ������ü.... ��� �� ���� ����.
	visualize();
}