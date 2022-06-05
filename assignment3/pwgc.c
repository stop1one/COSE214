#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF	0x04
#define GOAT	0x02
#define CABBAGE	0x01

// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename( FILE *fp, int state){
	if (fp==NULL) fp = stdout;
	int bin[4];
	for (int i = 0; i < 4; i++){
		bin[3-i] = state%2;
		state /= 2;
	}
	fprintf(fp, "<%d%d%d%d>", bin[0], bin[1], bin[2], bin[3]);
}

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc( int state, int *p, int *w, int *g, int *c){
	int bin[4];
	for (int i = 0; i < 4; i++){
		bin[3-i] = state%2;
		state /= 2;
	}
	*p = bin[0]; *w = bin[1]; *g = bin[2]; *c = bin[3];
}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end( int state){
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	if (w == g && p != w) return 1;
	else if (g == c && p != g) return 1;
	else return 0;
}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition( int state1,	int state2);

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP( int state){
	return state ^ 8;
}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW( int state){
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	if (p == w) return state ^ 12;
	else return -1;
}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG( int state){
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	if (p == g) return state ^ 10;
	return -1;
}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC( int state){
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	if (p == c) return state ^ 9;
	return -1;
}

// Define "is_possible_transition"
static int is_possible_transition( int state1,	int state2){
	if (is_dead_end(state1) || is_dead_end(state2)) return 0;
	int p, w, g, c;
	get_pwgc(state1, &p, &w, &g, &c);
	int next_states[4];
	next_states[0] = changeP(state1); next_states[1] = changePW(state1);
	next_states[2] = changePG(state1); next_states[3] = changePC(state1);
	for (int i = 0; i < 4; i++) if (state2 == next_states[i]) return 1;
	return 0;
}

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited( int visited[], int depth, int state){
	for (int i = 0; i <= depth; i++){
		if (visited[i] == state) return 1;
	}
	return 0;
}

// 방문한 경로(상태들)을 차례로 화면에 출력
static void print_path( int visited[], int depth){
	for (int i = 0; i <= depth; i++) printf("%d\n", visited[i]);
}

// recursive function
static void dfs_main( int initial_state, int goal_state, int depth, int visited[]){
	printf("current state is "); print_statename(NULL, initial_state); printf(" (depth %d)\n", depth);
	visited[depth] = initial_state;
	if (goal_state == initial_state){
		printf("Goal-state found!\n");
		for (int j = 0; j <= depth; j++) {
			print_statename(NULL, visited[j]);
			printf("\n");
		}
		printf("\n");
		return;
	}
	int next_states[4];
	next_states[0] = changeP(initial_state); next_states[1] = changePW(initial_state);
	next_states[2] = changePG(initial_state); next_states[3] = changePC(initial_state);
	for (int i = 0; i < 4; i++){
		if (next_states[i] == -1) continue;
		if(is_dead_end(next_states[i])){
			printf("\tnext state "); print_statename(NULL, next_states[i]); printf(" is dead-end\n");
		}
		else if (is_visited(visited, depth, next_states[i])){
			printf("\tnext state "); print_statename(NULL, next_states[i]); printf(" has been visited\n");
		}
		else{
			dfs_main(next_states[i], goal_state, depth+1, visited);
			printf("back to "); print_statename(NULL, initial_state); printf(" (depth %d)\n", depth);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix( int graph[][16]){
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			if (is_possible_transition(i, j)) graph[i][j] = 1;
		}
	}
}

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph( int graph[][16], int num){
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			printf("%d\t", graph[i][j]);
		}
		printf("\n");
	}
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph( char *filename, int graph[][16], int num){
	FILE *fp = fopen(filename, "w");
	fprintf(fp, "*Vertices 16\n");
	for (int i = 0; i < num; i++){
		fprintf(fp, "%d \"", i+1);
		print_statename(fp, i);
		fprintf(fp, "\"\n");
	}
	fprintf(fp, "*Edges\n");
	for (int i = 0; i < num/2; i++){
		for (int j = 0; j < num; j++){
			if (graph[i][j]){
				fprintf(fp, "  %d  %d\n", i+1, j+1);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int initial_state, int goal_state)
{
	int depth = 0;
	int visited[16] = {0,}; // 방문한 정점을 저장
	
	dfs_main( initial_state, goal_state, depth, visited); 
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int graph[16][16] = {0,};

	// 인접 행렬 만들기
	make_adjacency_matrix( graph);

	// 인접 행렬 출력 (only for debugging)
	//print_graph( graph, 16);
	
	// .net 파일 만들기
	save_graph( "pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search( 0, 15); // initial state, goal state
	
	return 0;
}