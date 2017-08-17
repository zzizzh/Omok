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
#define A5		1000	// 5����(������)
#define D5		800		// 5���(������ ���� 4)
#define AF4		320		// 4����(������)
#define DF4		80		// 4���
#define A4		58		// 4����(������ ���� 4)
#define A3		42		// 3����
#define D4		30		// 4���(������ ���� 4)
#define D3		21		// 3���
#define AA3		15		// 3����(������ ���� 3)
#define A2		10		// 2����
#define DD3		6		// 3���(������ ���� 3)
#define D2		3		// 2���
#define NONE	0		// �ƹ� ���� ����

// ���� ����
#define HORIZONTAL	0	// ����
#define RDIAGONAL	1	// ��밢
#define VERTICAL	2	// ����
#define LDIAGONAL	3	// �´밢

// ���� ���� �迭
int stateZ[6] = { 1, };
int map[MAXXY][MAXXY] = {NONE, };
// 
int fPriority = NONE;

// ���� ���� �迭
int dx[] = { 1, 1, 0, -1 };
int dy[] = { 0, 1, 1, 1 };

// B�Լ��� �������� �ش� ��ġ�� ��Ȳ�� ����� �� ��, ����ϴ� �Լ��� ������ BLACK, WHITE, EMPTY, OUTBD�� �� ���� ������
// B(-3,6)�� ������ �ٱ��� �����ϹǷ� OUTBD(-2)�� ���ϰ����� ����
// B(-3,6,BLACK)�� ������ ���� �����ϳ�, ���ϰ��� def������ �Ѱ��� BLACK�� ���ϰ����� ����
extern int B(int x, int y, int def = OUTBD);
int checkCurState(int ix, int iy, int mc);
int checkStateArr(int mc, int cp, int emptyCount, int next);

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
	int ix, iy;

	memset(map, NONE, sizeof(map));

	for(ix=0; ix<MAXXY; ix++)
	{
		for(iy=0; iy<MAXXY; iy++)
		{
			priority= checkCurState(ix, iy, mc);

			map[ix][iy] = priority;

			if((rand() % 2 == 1) ? (maxPriority < priority) : (maxPriority <= priority)){
				maxPriority = priority;
				
				*NewX = ix;
				*NewY = iy;
			}
		}
	}

	if(CurTurn < 3 || maxPriority == NONE){
		do{
			*NewX = MAXXY / 2 - CurTurn / 2 +rand() % (CurTurn + 1);
			*NewY = MAXXY / 2 - CurTurn / 2 +rand() % (CurTurn + 1);
		}while(B(*NewX, *NewY) != EMPTY);
	}
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

	int i, n, d;
	int j, k;
	int min;
	int arr43[2] = { NONE, NONE };

	int priority = NONE;	// �켱����(���� ��)
	int iMaxPriority = NONE;

	int emptyCount = 0;		// empty ��
	int outCount = 0;		// outboard ��
	int temp;
	int next = 0;
	
	// ���� ��ǥ �˻�
	cp = B(ix, iy);

	// ��ǥ�� ������� ������ �켱���� ����. �������� NONE.
	if (cp == EMPTY){
		// �� ���⿡ ���� �˻�
		for (d = 0; d < 4; d++){
			next = 0;
			// ��ǥ���� + i ���⸸ŭ �̵� �� �˻�
			iMaxPriority = NONE;

			if((B(ix+dx[d], iy+dy[d]) == WHITE || B(ix+dx[d], iy+dy[d]) == BLACK) ||
				(B(ix-dx[d], iy-dy[d]) == WHITE || B(ix-dx[d], iy-dy[d]) == BLACK))
				next = 1;

			for (i = -3; i <= 0; i++){
				emptyCount = 0;
				// �� ���� ��ġ�� ��������� �ٸ����� �˻�
				// ���� ��ġ�� ���� ���� ��� cp�� �켱���� ������ ���� �� ������ ����
				cp = B(ix + i * dx[d], iy + i * dy[d]);
				
				if(i!=0){
					if(cp == EMPTY || cp == OUTBD)
						continue;
					stateZ[1] = cp;
				}
				else{
					if((cp = B(ix + dx[d], iy + dy[d])) == EMPTY || cp == OUTBD)
						break;
					stateZ[1] = EMPTY;
					next = 1;
					emptyCount++;
				}

				// �� 4�� �˻�
				for (n = 1; n < 4; n++){
					np = B(ix + (i+n) * dx[d], iy + (i+n) * dy[d]);

					// �ٸ� ���� ���� ���, �ٱ��� ���
					if (np != cp && np != EMPTY)
						break;

					else if (np == EMPTY)
						emptyCount++;

					stateZ[n+1] = np;
				}

				if (n == 4){
					stateZ[0] = B(ix + (i-1) * dx[d], iy + (i-1) * dy[d]);
					stateZ[5] = B(ix + (i+n) * dx[d], iy + (i+n) * dy[d]);

					temp = checkStateArr(mc, cp, emptyCount, next);

					if(iMaxPriority < temp)
						iMaxPriority = temp;
				}
			}
			if (iMaxPriority >= D3) {
				min = 9999;
				k = 0;
				for(j=0; j<2; j++)
				{
					if(arr43[j]<=min){
						min = arr43[j];
						k = j;
					}
				}
				if(min < iMaxPriority){
					priority -= min;
					arr43[k] = iMaxPriority;
					priority += iMaxPriority;
				}
			}

			else if (iMaxPriority > priority) {
				priority = iMaxPriority;
			}
		}
	}

	return priority;
}

int checkStateArr(int mc, int cp, int emptyCount, int next){
	int ec = mc == WHITE ? BLACK : WHITE;

	int priority = NONE;
	int ad;

	if (cp == mc)
		ad = ATTACK;
	else
		ad = DEFENCE;

	if (emptyCount == 4){
		priority = NONE;
	}
	else if (emptyCount == 3){
		priority = ad == ATTACK ? A2 : D2;
	}
	else if (emptyCount == 2){
		if (stateZ[0] == EMPTY){
			if (stateZ[5] == EMPTY){
				priority = ad == ATTACK ? A3 : D3;
			}
			else if (stateZ[5] == mc){
				priority = ad == ATTACK ? A4 : DD3;
			}
			else if (stateZ[5] == ec){
				priority = ad == ATTACK ? AA3 : D4;
			}
			else{
				priority = ad == ATTACK ? AA3 : DD3;
			}
		}
		else if (stateZ[0] == mc){
			if (stateZ[5] == EMPTY){
				priority = ad == ATTACK ? A4 : DD3;
			}
			else if (stateZ[5] == mc){
				priority = ad == ATTACK ? A4 : NONE;
			}
			else if (stateZ[5] == ec){
				priority = ad == ATTACK ? A4 : D4;
			}
			else{
				priority = ad == ATTACK ? NONE : D5;
			}
		}
		else if (stateZ[0] == ec){
			if (stateZ[5] == EMPTY){
				priority = ad == ATTACK ? AA3 : D4;
			}
			else if (stateZ[5] == mc){
				priority = ad == ATTACK ? A4 : D4;
			}
			else if (stateZ[5] == ec){
				priority = ad == ATTACK ? NONE : D4;
			}
			else{
				priority = ad == ATTACK ? NONE : D5;
			}
		}
		else
			priority = NONE;
	}
	else if (emptyCount == 1){
		if (stateZ[0] == EMPTY){
			if (stateZ[5] == EMPTY){
				priority = ad == ATTACK ? AF4 : DF4;
			}
			else if (stateZ[5] == mc){
				priority = ad == ATTACK ? A5 : D4;
			}
			else if (stateZ[5] == ec){
				priority = ad == ATTACK ? A4 : D5;
			}
			else{
				priority = ad == ATTACK ? A4 : D4;
			}
		}
		else if (stateZ[0] == mc){
			if (stateZ[5] == EMPTY){
				priority = ad == ATTACK ? A5 : D4;
			}
			else if (stateZ[5] == mc){
				priority = ad == ATTACK ? A5 : NONE;
			}
			else if (stateZ[5] == ec){
				priority = ad == ATTACK ? A5 : D4;
			}
			else{
				priority = ad == ATTACK ? A5 : NONE;
			}
		}
		else if (stateZ[0] == ec){
			if (stateZ[5] == EMPTY){
				priority = ad == ATTACK ? A4 : D5;
			}
			else if (stateZ[5] == mc){
				priority = ad == ATTACK ? A5 : D5;
			}
			else if (stateZ[5] == ec){
				priority = ad == ATTACK ? NONE : D5;
			}
			else{
				priority = ad == ATTACK ? NONE : D5;
			}
		}
		else{
			priority = NONE;
		}
	}
	else{
		priority = NONE;
	}

	if(next != 0)
		return priority + 1;
	else
		return priority;
}
