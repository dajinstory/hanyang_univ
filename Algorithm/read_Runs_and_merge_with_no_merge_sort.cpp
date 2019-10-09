/*

	적은 메모리 환경에서, 큰 디스크 파일 읽어오는 프로그램.
	각각의 정렬된 M개의 데이터가 저장된 file을 N개 읽어와서
	전체 정렬시켰을 때, 처음부터 k번째인 애들을 각각 output.txt에 출력.
	N<=1000000, M<=256, K<=N*M
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <string.h>
#include <queue>
#include <algorithm>
#define ARRSIZE 100000
#define QSIZE 1000
#define POPSIZE 500
using namespace std;

uint64_t MAX_N = 18446744073709551615;
priority_queue<pair<uint64_t, int> >myQ;
FILE* fin[257];
FILE* fout;

uint64_t arr[257][ARRSIZE];
int rest[257], arr_size[257], q_size[257];
int idx[257], end_[257];
int done[257];

uint64_t getMin(uint64_t a, uint64_t b){
	if (a < b)return a;
	return b;
}

void loadFilePointer(int num);
void loadData(int num);
void pushQueue(int num);

int main(){
	int N, M, K, bp = 0, step = 1;
	scanf("%d%d%d", &N, &M, &K);

	fout = fopen("./output.txt", "w");

	for (int i = 1; i <= N; i++){
		rest[i] = M;
		loadFilePointer(i);
		pushQueue(i);
	}
	while (!myQ.empty()){
		for (int i = 0; i < POPSIZE; i++, step++){
			if (myQ.empty()){
				bp = 1;
				break;
			}
			if (step == K){
				fprintf(fout, "%llu ", MAX_N - myQ.top().first);
				step = 0;
			}
			q_size[myQ.top().second]--;
			myQ.pop();
		}
		if (bp){
			break;
		}
		for (int i = 1; i <= N; i++){
			if (q_size[i] < POPSIZE && done[i] != 2){
				pushQueue(i);
			}
		}
	}
	for (int i = 1; i <= N; i++){
		fclose(fin[i]);
	}
	fclose(fout);

	return 0;
}

void loadFilePointer(int num){
	string filename_txt = "./input";
	string filetype_txt = ".txt";
	string filename_txt_s;
	char* filename_txt_c;
	char tmp[65] = { 0 };
	string number_s;


	itoa(num, tmp, 10);
	number_s = string(tmp);
	filename_txt_s = filename_txt + number_s + filetype_txt;
	filename_txt_c = &filename_txt_s[0u];
	fin[num] = fopen(filename_txt_c, "r");
}

void loadData(int num){
	if (rest[num] < ARRSIZE){
		for (end_[num] = 0; end_[num] < rest[num]; end_[num]++){
			fscanf(fin[num], "%llu", &arr[num][end_[num]]);
		}
	}
	else{
		for (end_[num] = 0; end_[num] < ARRSIZE; end_[num]++){
			fscanf(fin[num], "%llu", &arr[num][end_[num]]);
		}
	}
	rest[num] -= end_[num];
	arr_size[num] += end_[num];
	if (rest[num] == 0)done[num] = 1;
}

void pushQueue(int num){
	int i = 0;
	if (arr_size[num] < QSIZE && done[num] == 0){
		loadData(num);
	}
	for (i = 0; i < getMin(QSIZE, arr_size[num]); i++){
		myQ.push(make_pair(MAX_N - arr[num][idx[num] + i], num));
	}
	idx[num] = (idx[num] + i) % ARRSIZE;
	q_size[num] += i;
	arr_size[num] -= i;
	if (i == 0){
		done[num] = 2;
	}
}