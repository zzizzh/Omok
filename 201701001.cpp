#include "stdafx.h"
#include <vector>
#include <algorithm>
/*
■아래에 팀정보를 기재하세요.
대학명: 지역선도대학교
팀  명: 징쨩
팀  원: 지재민(컴퓨터학부 4학년)
제출일: 2017.5.17
접수번호:201701013
제출코드:01148742

■작성 시 주의사항
- C++기본 문법에 의하여 작성
- 외부 라이브러리 사용 금지 (dll, STL등)
- for문 인덱스 변수는 외부에 미리 선언할 것 (아래와 같이 하지 말 것)
for (int i = 0; i < 10...
- 파일 내에 global변수, 함수, class등 선언 가능, 단 다른 팀과 중복되지 않도록 명명
- 제출시 본 파일의 함수 f201701001()를 해당 팀번호로 수정하고 "팀번호.cpp"로 저장하여 제출
*/

// 상수 지정
#define MAXXY	16		//	가로 세로 너비
#define CENTER	8		//	중신 좌표
#define BLACK	1		//	검정
#define WHITE	0		//	흰색
#define EMPTY	-1		//	비어있음
#define OUTBD	-2		//	바깥

#define ATTACK	0		
#define DEFENCE 1

// 우선순위(상태)
#define A5		150		// 5공격(마무리)
#define D5		30		// 5방어(한쪽이 막힌 4)
//#define A43		21		// 43공격(마무리)
#define AF4		20		// 4공격(마무리)
//#define D43		13		// 43방어
#define DF4		13		// 4방어
//#define A33		10		// 33공격
//#define D33		9		// 33방어
#define A4		8		// 4공격(한쪽이 막힌 4)
#define A3		7		// 3공격
#define D4		6		// 4방어(한쪽이 막힌 4)
#define D3		4		// 3방어
#define A2		3		// 2공격
#define D2		1		// 2방어
#define NONE	0		// 아무 상태 없음

// 오목 방향
#define HORIZONTAL	0	// 가로
#define RDIAGONAL	1	// 우대각
#define VERTICAL	2	// 세로
#define LDIAGONAL	3	// 좌대각

using namespace std;
// 민맥스 트리에 쓸 노드
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
	int sx, sy;		// 놓아야 할 곳
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

// 상태 저장 배열
int stateZ[6] = { 1, };

// 
int fPriority = NONE;

// 각 좌표마다 우선순위 설정. 
int priorityMapZ[MAXXY][MAXXY] = { 0, };
// 각 좌표에 저장할 Point 클래스
PointZ ***pointMapZ = NULL;

// 오목 방향 배열
int dx[] = { 1, 1, 0, -1 };
int dy[] = { 0, 1, 1, 1 };

// B함수는 오목판의 해당 위치의 상황을 얻고자 할 때, 사용하는 함수로 리턴은 BLACK, WHITE, EMPTY, OUTBD중 한 값을 리턴함
// B(-3,6)는 오목판 바깥을 지시하므로 OUTBD(-2)를 리턴값으로 얻음
// B(-3,6,BLACK)는 오목판 밖을 지시하나, 리턴값은 def값으로 넘겨준 BLACK를 리턴값으로 얻음
extern int B(int x, int y, int def = OUTBD);
Node createNode(int depth, int mc);
int checkCurState(int ix, int iy, int mc);
int checkState(int ix, int iy, int mc, int depth);
int checkStateArr(int mc, int cp, int emptyCount);
int alphabeta(Node node, int curTurn, int depth, int a, int b, int mc, BOOL maxPlayer, int *NewX, int *NewY);


// AI 작성시, B(int x,int y,int)함수를 통해 판세를 분석하고 알을 둘 위치 값을 인자 *NewX, *NewY를 통해 호출 함수로 되돌린다.
// NewX,NewY : AI에 의해 알을 두고자 하는 값에 대한 리턴 포인터 변수
// mc : AI가 둘 알의 색 (BLACK 또는 WHITE)
// CurTurn : 현재 진행된 수
// 4개 방향으로의 전술 적용을 위해, COmDevDlg::IsGameOver() 함수의 구현 참조

void f201701001(int *NewX, int *NewY, int mc, int CurTurn)
{
	int ec = mc == WHITE ? BLACK : WHITE;	//	적의 색 확인

	int maxPriority = NONE;
	int priority;
	int temp;
	int my = 0, mx = 0;						// 우선순위가 가정 높은 좌표
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
인자로 받은 좌표에 대한 검사 수행. 좌료에 대해 가로, 세로, 좌대각, 우대각에 대해 검사.
좌표를 더해가면서 검사하는데, 만약 처음 돌의 색과 다른 돌이 검출될 경우 break로 빠져나옴.
*/
int checkCurState(int ix, int iy, int mc)
{
	int ec = mc == WHITE ? BLACK : WHITE;
	int cp;		// 현재 좌표 
	int np;		// 다음 좌표

	int i, j, n, d;
	int arr43[2] = { NONE, NONE };

	int priority = NONE;	// 우선순위(리턴 값)

	int emptyCount = 0;		// empty 수
	int addCount = 0;
	int temp;
	
	// 현재 좌표 검사
	cp = B(ix, iy);

	// 좌표가 비어있을 때에만 우선순위 결정. 나머지는 NONE.
	if (cp == EMPTY){
		// 각 방향에 대해 검사
		for (d = 0; d < 4; d++){
			// 좌표에서 + i 방향만큼 이동 후 검사
			cp = EMPTY;
				
			// 그 다음 위치도 비어있으면 다른방향 검사
			// 다음 위치에 돌이 있을 경우 cp는 우선순위 결정에 쓰일 돌 색으로 참조
			if((cp = B(ix + dx[d], iy + dy[d])) == EMPTY)
				continue;

			stateZ[1] = cp;

			emptyCount = 0;
			// 총 4개 검사
			for (n = 2; n < 5; n++){

				np = B(ix + n*dx[d], iy + n*dy[d]);

				// 다른 돌이 있을 경우 
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
