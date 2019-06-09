#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;
#pragma region Introduction
//
//@서울 / 수도권 지하철 노선도를 가져온다.
//@양이 너무 많으니 개인적으로 정리해서 일부만 가져온다.
//@지하철 역은 30개가 넘지 않도록 한다.주로 환승역을 이용한다.
//@예) 2호선과 그 내부의 연결된 다른 호선 및 환승역 30개를 이용한다.
//@역 사이의 거리를 넣는다.현실에 기반하지 않아도 된다.

//출발점과 도착점을 입력하면 경로와 최단거리를 표시한다.
//강의용 슬라이드의 코드는 최단거리만 표시하며, 경로는 응용하는 방법을 찾아야 합니다.

//실행 예
//모든 역을 표시한다. (BFS, DFS를 이용.배열에서 바로 출력하지 말것) : 역 이름이 표시되어야 출발역, 도착역 이름을 입력할 수 있음
//출발역 : 사당
//도착역 : 선정릉
//사당 – 교대 – 선릉 – 선정릉(5.5km)

/*
과제 해설
1. 내가 사용하고자 하는 노선도 그리기, 이것도 제출(그래프 형태) -> 배열 or 인접리스트,
가중치를 매번 랜덤하게 하면 안됩니다.(디버깅이 어려움)
float형태로 1.1, 1.2 이렇게 해도 디버깅이 편해질 수 있다.
역이 30개면 30x30이 나오겠죠. 30개 정도 역이면 엣지가 50~60개 될테니, 900개의 칸 중 50~60개만 차있겠지.
코드는 강의 슬라이드를 참고하세요. 경로를 어떻게 표시할 건지 코드를 수정해야함(u,w,v 나와있는 슬라이드)

2. DFS/BFS 둘 중하나 이용해서 모든역을 표시 하자. 이게 잘 되었다면 그래프를 잘 구성했다는 뜻
환승역? 데이터를 집어 넣을 때 이 역은 환승역이다라고 체크 해놓아야 함(*을 붙이 던지 하는 자기만의 방식으로)
*/
#pragma endregion
#pragma region GlobalVariables
const int dx[4] = { 0, 0, 1, -1 };
const int dy[4] = { 1, -1, 0, 0 };
const int MAX_STATION_NAME_LEN = 16;
const float INF = 99999.f;
typedef enum {
	//중복 역은 표기하지 않음, _은 환승역
	_사당, 방배, 서초, _교대, _강남, 역삼, _선릉, //2호선
	_고속터미널, 남부터미널, _양재, 매봉, _도곡, //3호선
	_동작, _이수, //4호선
	상도, 숭실대입구, 남성, 내방, 반포, 논현, 학동, _강남구청, //7호선
	흑석, 구반포, 신반포, //9호선
	선정릉, 한티, //분당선
	양재시민의숲, 청계산입구, 판교, //신분당선
	NUM_OF_STATIONS,
	ERROR
} STATION;
float weight[NUM_OF_STATIONS][NUM_OF_STATIONS]; //역 사이 거리
bool visit[NUM_OF_STATIONS][NUM_OF_STATIONS]; //visit check array for bfs
char st_name[16];
char departSt[MAX_STATION_NAME_LEN];
char arriveSt[MAX_STATION_NAME_LEN];
float fDistance[NUM_OF_STATIONS]; //Dijkstra로 생성된 최단 거리 배열
bool found[NUM_OF_STATIONS]; //최단거리 갱신 확인용 배열
STATION path[NUM_OF_STATIONS]; //최단경로 출력용
#pragma endregion
#pragma region Functions
void enumToStr(STATION input) {
	switch (input) {
	case _사당: strcpy_s(st_name, sizeof(st_name), "사당"); break;
	case 방배: strcpy_s(st_name, sizeof(st_name), "방배"); break;
	case 서초: strcpy_s(st_name, sizeof(st_name), "서초"); break;
	case _교대: strcpy_s(st_name, sizeof(st_name), "교대"); break;
	case _강남: strcpy_s(st_name, sizeof(st_name), "강남"); break;
	case 역삼: strcpy_s(st_name, sizeof(st_name), "역삼"); break;
	case _선릉: strcpy_s(st_name, sizeof(st_name), "선릉"); break;
	case _고속터미널: strcpy_s(st_name, sizeof(st_name), "고속터미널"); break;
	case 남부터미널: strcpy_s(st_name, sizeof(st_name), "남부터미널"); break;
	case _양재: strcpy_s(st_name, sizeof(st_name), "양재"); break;
	case 매봉: strcpy_s(st_name, sizeof(st_name), "매봉"); break;
	case _도곡: strcpy_s(st_name, sizeof(st_name), "도곡"); break;
	case _동작: strcpy_s(st_name, sizeof(st_name), "동작"); break;
	case _이수: strcpy_s(st_name, sizeof(st_name), "이수"); break;
	case 상도: strcpy_s(st_name, sizeof(st_name), "상도"); break;
	case 숭실대입구: strcpy_s(st_name, sizeof(st_name), "숭실대입구"); break;
	case 남성: strcpy_s(st_name, sizeof(st_name), "남성"); break;
	case 내방: strcpy_s(st_name, sizeof(st_name), "내방"); break;
	case 반포: strcpy_s(st_name, sizeof(st_name), "반포"); break;
	case 논현: strcpy_s(st_name, sizeof(st_name), "논현"); break;
	case 학동: strcpy_s(st_name, sizeof(st_name), "학동"); break;
	case _강남구청: strcpy_s(st_name, sizeof(st_name), "강남구청"); break;
	case 흑석: strcpy_s(st_name, sizeof(st_name), "흑석"); break;
	case 구반포: strcpy_s(st_name, sizeof(st_name), "구반포"); break;
	case 신반포: strcpy_s(st_name, sizeof(st_name), "신반포"); break;
	case 선정릉: strcpy_s(st_name, sizeof(st_name), "선정릉"); break;
	case 한티: strcpy_s(st_name, sizeof(st_name), "한티"); break;
	case 양재시민의숲: strcpy_s(st_name, sizeof(st_name), "양재시민의숲"); break;
	case 청계산입구: strcpy_s(st_name, sizeof(st_name), "청계산입구"); break;
	case 판교: strcpy_s(st_name, sizeof(st_name), "판교"); break;
	default: break;
	}
}
STATION strToEnum(const string& str) {
	STATION rtSt;
	if (str == "사당") { rtSt = _사당; }
	else if (str == "방배") { rtSt = 방배; }
	else if (str == "서초") { rtSt = 서초; }
	else if (str == "교대") { rtSt = _교대; }
	else if (str == "강남") { rtSt = _강남; }
	else if (str == "역삼") { rtSt = 역삼; }
	else if (str == "선릉") { rtSt = _선릉; }
	else if (str == "고속터미널") { rtSt = _고속터미널; }
	else if (str == "남부터미널") { rtSt = 남부터미널; }
	else if (str == "양재") { rtSt = _양재; }
	else if (str == "매봉") { rtSt = 매봉; }
	else if (str == "도곡") { rtSt = _도곡; }
	else if (str == "동작") { rtSt = _동작; }
	else if (str == "이수") { rtSt = _이수; }
	else if (str == "상도") { rtSt = 상도; }
	else if (str == "숭실대입구") { rtSt = 숭실대입구; }
	else if (str == "남성") { rtSt = 남성; }
	else if (str == "내방") { rtSt = 내방; }
	else if (str == "반포") { rtSt = 반포; }
	else if (str == "논현") { rtSt = 논현; }
	else if (str == "학동") { rtSt = 학동; }
	else if (str == "강남구청") { rtSt = _강남구청; }
	else if (str == "흑석") { rtSt = 흑석; }
	else if (str == "구반포") { rtSt = 구반포; }
	else if (str == "신반포") { rtSt = 신반포; }
	else if (str == "선정릉") { rtSt = 선정릉; }
	else if (str == "한티") { rtSt = 한티; }
	else if (str == "양재시민의숲") { rtSt = 양재시민의숲; }
	else if (str == "청계산입구") { rtSt = 청계산입구; }
	else if (str == "판교") { rtSt = 판교; }
	else {
		rtSt = ERROR; //잘못 입력한 경우
	}
	return rtSt;
}
void bfs(int startX, int startY) {
	queue<pair<int, int>> Q;
	enumToStr(static_cast<STATION>(startX));
	strcpy_s(departSt, sizeof(departSt), st_name);
	enumToStr(static_cast<STATION>(startY));
	strcpy_s(arriveSt, sizeof(arriveSt), st_name);
	Q.push(make_pair(startX, startY));
	visit[startX][startY] = true;
	std::cout << departSt << "->" << arriveSt << ':' << weight[startX][startY] << "km" << '\n';
	while (!Q.empty()) {
		startX = Q.front().first;
		startY = Q.front().second;
		Q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = startX + dx[dir];
			enumToStr(static_cast<STATION>(nx));
			strcpy_s(departSt, sizeof(departSt), st_name);
			int ny = startY + dy[dir];
			enumToStr(static_cast<STATION>(ny));
			strcpy_s(arriveSt, sizeof(arriveSt), st_name);
			if (nx >= 0 && nx < NUM_OF_STATIONS && ny >= 0 && ny < NUM_OF_STATIONS) {
				if (!visit[nx][ny] && !(weight[nx][ny] == 0 || weight[nx][ny] == INF)) {
					std::cout << departSt << "->" << arriveSt << ':' << weight[nx][ny] << "km" << '\n';
					Q.push(make_pair(nx, ny));
					visit[nx][ny] = true;
				}
			}
		}
	}
}
int choose() {
	int i, minpos;
	float min = INF;
	minpos = -1;
	for (i = 0; i < NUM_OF_STATIONS; i++)
		if (fDistance[i] < min && !found[i]) { //전체 가중치가 가장 작은 놈으로 min 갱신
			min = fDistance[i];
			minpos = i;
		}
	return minpos;
}
void Dijkstra(STATION departure) {
	int i, from, to;
	for (i = 0; i < NUM_OF_STATIONS; i++) 	// 초기화
	{
		path[i] = ERROR;
		fDistance[i] = weight[departure][i];
		found[i] = false;
	}
	found[departure] = true; // 시작 정점 방문 표시
	fDistance[departure] = 0.f;
	for (i = 0; i < NUM_OF_STATIONS - 2; i++) {
		from = choose();
		found[from] = true;
		for (to = 0; to < NUM_OF_STATIONS; to++) {
			if (!found[to])
				if (fDistance[to] > fDistance[from] + weight[from][to]) {
					fDistance[to] = fDistance[from] + weight[from][to]; //최단거리 갱신
					path[to] = static_cast<STATION>(from); //이전 역 저장
				}
		}
	}
}
#pragma endregion
int main() {
	int row, col, diag;
	for (row = _사당; row <= 판교; row++) { //먼저 모두 INF로 초기화
		for (col = _사당; col <= 판교; col++) {
			weight[row][col] = INF;
		}
	}

	for (diag = _사당; diag <= 판교; diag++) { //대각성분만 0으로 초기화
		weight[diag][diag] = 0.f;
	}

	//file IO로 노선 정보 읽어오기
	FILE * fp = NULL;
	fp = fopen("weight.txt", "r");
	STATION enumDepart, enumArrive;
	float fInitDistance;
	while (NULL != fp) {
		fscanf(fp,"%*s %d %*s %*s %d %*c %f", &enumDepart, &enumArrive, &fInitDistance);
		weight[enumDepart][enumArrive] = fInitDistance;
		if (enumDepart == 판교) break;
	}

	//BFS로 초기 역 정보 출력
	std::cout << "30개 역 정보 출력(인접한 역의 거리만 출력)" << std::endl;
	std::cout << std::setprecision(3);
	for (row = _사당; row <= 판교; row ++) {
		for (col = _사당; col <= 판교; col++) {
			if(!visit[row][col] && weight[row][col] > 0.f && weight[row][col] < INF)
				bfs(row, col); //출발 역
		}
	}
	std::cout << '\n';

	char inputStr[MAX_STATION_NAME_LEN];
	string str;
	while (true) {
		std::cout << "출발역(종료하려면 '종료'입력) : "; std::cin >> inputStr;
		str = inputStr;
		if (str == "종료") break;
		enumDepart = strToEnum(str);
		std::cout << "도착역 : "; std::cin >> inputStr;
		str = inputStr;
		enumArrive = strToEnum(str);
		if (enumDepart == ERROR || enumArrive == ERROR) {
			std::cout << "존재하지 않는 역 입니다. 다시 입력해주세요." << '\n';
			continue;
		}
		Dijkstra(enumDepart);
		stack<string> pathStack;
		STATION p = enumArrive;
		while (p != ERROR) {
			enumToStr(p);
			pathStack.push(st_name);
			p = path[p];
		}
		enumToStr(enumDepart);
		pathStack.push(st_name);
		while (!pathStack.empty()) {
			std::cout << pathStack.top();
			pathStack.pop();
			if (pathStack.size() != 0) {
				std::cout << " - ";
			}
		}
		std::cout <<'('<< fDistance[enumArrive]<<"km"<<')' <<std::endl;
	}
	return 0;
}