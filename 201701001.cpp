#include "stdafx.h"
#include <vector>
#include <algorithm>
/*
��Ʒ��� �������� �����ϼ���.
���и�: �����������б�
��  ��: ¡»
��  ��: �����(��ǻ���к� 4�г�)
������: 2017.5.17
������ȣ:201701013
�����ڵ�:01148742

���ۼ� �� ���ǻ���
- C++�⺻ ������ ���Ͽ� �ۼ�
- �ܺ� ���̺귯�� ��� ���� (dll, STL��)
- for�� �ε��� ������ �ܺο� �̸� ������ �� (�Ʒ��� ���� ���� �� ��)
for (int i = 0; i < 10...
- ���� ���� global����, �Լ�, class�� ���� ����, �� �ٸ� ���� �ߺ����� �ʵ��� ���
- ����� �� ������ �Լ� f201701001()�� �ش� ����ȣ�� �����ϰ� "����ȣ.cpp"�� �����Ͽ� ����
*/

// ��� ����
#define MAXXY	16		//	���� ���� �ʺ�
#define CENTER	8		//	�߽� ��ǥ
#define BLACK	1		//	����
#define WHITE	0		//	���
#define EMPTY	-1		//	�������
#define OUTBD	-2		//	�ٱ�

#define ATTACK	0		
#define DEFENCE 1

// �켱����(����)
#define A5		150		// 5����(������)
#define D5		30		// 5���(������ ���� 4)
//#define A43		21		// 43����(������)
#define AF4		20		// 4����(������)
//#define D43		13		// 43���
#define DF4		13		// 4���
//#define A33		10		// 33����
//#define D33		9		// 33���
#define A4		8		// 4����(������ ���� 4)
#define A3		7		// 3����
#define D4		6		// 4���(������ ���� 4)
#define D3		4		// 3���
#define A2		3		// 2����
#define D2		1		// 2���
#define NONE	0		// �ƹ� ���� ����

// ���� ����
#define HORIZONTAL	0	// ����
#define RDIAGONAL	1	// ��밢
#define VERTICAL	2	// ����
#define LDIAGONAL	3	// �´밢

using namespace std;
// �θƽ� Ʈ���� �� ���
class Node{
public :
	int maxPriority;
	int result;
	int depth;
	int x;
	int y;

	vector<Node> child;

	Node(){
		result = 0;
		x = -1;
		y = -1;
	}

	Node(int p, int r, int d){
		maxPriority = p;
		result = r;
		depth = d;
		x = -1;
		y = -1;
	}
};

class PointZ{

public:
	int priority;
	int sx, sy;		// ���ƾ� �� ��
	int color;

	PointZ(){
		priority	= NONE;
		sx = -1;
		sy = -1;
		color = -1;
	}

	PointZ(int x, int y){
		priority = NONE;
		sx = x;
		sy = y;
	}
};

// ���� ���� �迭
int stateZ[6] = { 1, };

// 
int fPriority = NONE;

// �� ��ǥ���� �켱���� ����. 
int priorityMapZ[MAXXY][MAXXY] = { 0, };
// �� ��ǥ�� ������ Point Ŭ����
PointZ ***pointMapZ = NULL;

// ���� ���� �迭
int dx[] = { 1, 1, 0, -1 };
int dy[] = { 0, 1, 1, 1 };

// B�Լ��� �������� �ش� ��ġ�� ��Ȳ�� ����� �� ��, ����ϴ� �Լ��� ������ BLACK, WHITE, EMPTY, OUTBD�� �� ���� ������
// B(-3,6)�� ������ �ٱ��� �����ϹǷ� OUTBD(-2)�� ���ϰ����� ����
// B(-3,6,BLACK)�� ������ ���� �����ϳ�, ���ϰ��� def������ �Ѱ��� BLACK�� ���ϰ����� ����
extern int B(int x, int y, int def = OUTBD);
Node createNode(int depth, int mc);
int checkCurState(int ix, int iy, int mc);
int checkState(int ix, int iy, int mc, int depth);
int checkStateArr(int mc, int cp, int emptyCount);
int alphabeta(Node node, int curTurn, int depth, int a, int b, int mc, BOOL maxPlayer, int *NewX, int *NewY);


// AI �ۼ���, B(int x,int y,int)�Լ��� ���� �Ǽ��� �м��ϰ� ���� �� ��ġ ���� ���� *NewX, *NewY�� ���� ȣ�� �Լ��� �ǵ�����.
// NewX,NewY : AI�� ���� ���� �ΰ��� �ϴ� ���� ���� ���� ������ ����
// mc : AI�� �� ���� �� (BLACK �Ǵ� WHITE)
// CurTurn : ���� ����� ��
// 4�� ���������� ���� ������ ����, COmDevDlg::IsGameOver() �Լ��� ���� ����

void f201701001(int *NewX, int *NewY, int mc, int CurTurn)
{
	int ec = mc == WHITE ? BLACK : WHITE;	//	���� �� Ȯ��

	int maxPriority = NONE;
	int priority;
	int temp;
	int my = 0, mx = 0;						// �켱������ ���� ���� ��ǥ
	int i, j, k;
	int ix, iy;

	for(ix=0; ix<MAXXY; ix++)
	{
		for(iy=0; iy<MAXXY; iy++)
		{
			priority= checkCurState(ix, iy, mc);

			if(maxPriority <= priority){
				maxPriority = priority;
				
				*NewX = ix;
				*NewY = iy;
			}
		}
	}

	if(CurTurn < 3 || maxPriority == NONE){
		do{
			*NewX = MAXXY / 2 - CurTurn / 2;// +rand() % (CurTurn + 1);
			*NewY = MAXXY / 2 - CurTurn / 2;// +rand() % (CurTurn + 1);
			//*NewX = 0;
			//*NewY = 15;
		}while(B(*NewX, *NewY) != EMPTY);
	}
	temp = B(0, 15);
}

/*
���ڷ� ���� ��ǥ�� ���� �˻� ����. �·ῡ ���� ����, ����, �´밢, ��밢�� ���� �˻�.
��ǥ�� ���ذ��鼭 �˻��ϴµ�, ���� ó�� ���� ���� �ٸ� ���� ����� ��� break�� ��������.
*/
int checkCurState(int ix, int iy, int mc)
{
	int ec = mc == WHITE ? BLACK : WHITE;
	int cp;		// ���� ��ǥ 
	int np;		// ���� ��ǥ

	int i, j, n, d;
	int arr43[2] = { NONE, NONE };

	int priority = NONE;	// �켱����(���� ��)

	int emptyCount = 0;		// empty ��
	int addCount = 0;
	int temp;
	
	// ���� ��ǥ �˻�
	cp = B(ix, iy);

	// ��ǥ�� ������� ������ �켱���� ����. �������� NONE.
	if (cp == EMPTY){
		// �� ���⿡ ���� �˻�
		for (d = 0; d < 4; d++){
			// ��ǥ���� + i ���⸸ŭ �̵� �� �˻�
			cp = EMPTY;
				
			// �� ���� ��ġ�� ��������� �ٸ����� �˻�
			// ���� ��ġ�� ���� ���� ��� cp�� �켱���� ������ ���� �� ������ ����
			if((cp = B(ix + dx[d], iy + dy[d])) == EMPTY)
				continue;

			stateZ[1] = cp;

			emptyCount = 0;
			// �� 4�� �˻�
			for (n = 2; n < 5; n++){

				np = B(ix + n*dx[d], iy + n*dy[d]);

				// �ٸ� ���� ���� ��� 
				if (np != cp && np != EMPTY)
					break;

				else if (np == EMPTY)
					emptyCount++;

				stateZ[n] = np;
			}

			if (n == 5){
				stateZ[0] = B(ix - dx[d], iy - dy[d]);
				stateZ[5] = B(ix + n*dx[d], iy + n*dy[d]);

				temp = checkStateArr(mc, cp, emptyCount);


				if (temp >= D3) {
					priority += temp;
				}

				else if (temp > priority) {
					priority = temp;
				}
			}
		}
	}

	return priority;
}

int checkStateArr(int mc, int cp, int emptyCount){
	int ec = mc == WHITE ? BLACK : WHITE;

	int priority = NONE;
	int ad;

	if (cp == mc)
		ad = ATTACK;
	else
		ad = DEFENCE;

	if (emptyCount == 4){
		return NONE;
	}
	else if (emptyCount == 3){
		return ad == ATTACK ? A2 : D2;
	}
	else if (emptyCount == 2){
		if (stateZ[0] == EMPTY){
			if (stateZ[5] == EMPTY){
				return ad == ATTACK ? A3 : D3;
			}
			else if (stateZ[5] == mc){
				return ad == ATTACK ? A4 : D3;
			}
			else if (stateZ[5] == ec){
				return ad == ATTACK ? A3 : D4;
			}
			else{
				return ad == ATTACK ? A3 : D4;
			}
		}
		else if (stateZ[0] == mc){
			if (stateZ[5] == EMPTY){
				return ad == ATTACK ? A4 : D3;
			}
			else if (stateZ[5] == mc){
				return ad == ATTACK ? A4 : NONE;
			}
			else if (stateZ[5] == ec){
				return ad == ATTACK ? A4 : D4;
			}
			else{
				return ad == ATTACK ? NONE : D5;
			}
		}
		else if (stateZ[0] == ec){
			if (stateZ[5] == EMPTY){
				return ad == ATTACK ? A3 : D4;
			}
			else if (stateZ[5] == mc){
				return ad == ATTACK ? A4 : D4;
			}
			else if (stateZ[5] == ec){
				return ad == ATTACK ? NONE : D4;
			}
			else{
				return ad == ATTACK ? NONE : D5;
			}
		}
		else
			return NONE;
	}
	else if (emptyCount == 1){
		if (stateZ[0] == EMPTY){
			if (stateZ[5] == EMPTY){
				return ad == ATTACK ? AF4 : DF4;
			}
			else if (stateZ[5] == mc){
				return ad == ATTACK ? A5 : D4;
			}
			else if (stateZ[5] == ec){
				return ad == ATTACK ? A4 : DF4;
			}
			else{
				return ad == ATTACK ? A4 : D4;
			}
		}
		else if (stateZ[0] == mc){
			if (stateZ[5] == EMPTY){
				return ad == ATTACK ? A5 : D4;
			}
			else if (stateZ[5] == mc){
				return ad == ATTACK ? A5 : NONE;
			}
			else if (stateZ[5] == ec){
				return ad == ATTACK ? A5 : D5;
			}
			else{
				return ad == ATTACK ? A5 : D5;
			}
		}
		else if (stateZ[0] == ec){
			if (stateZ[5] == EMPTY){
				return ad == ATTACK ? A4 : DF4;
			}
			else if (stateZ[5] == mc){
				return ad == ATTACK ? A5 : D5;
			}
			else if (stateZ[5] == ec){
				return ad == ATTACK ? NONE : D5;
			}
			else{
				return ad == ATTACK ? NONE : D5;
			}
		}
		else{
			return NONE;
		}
	}
	else{
		return NONE;
	}
}
