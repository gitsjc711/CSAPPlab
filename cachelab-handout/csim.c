#include "cachelab.h"
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <string.h>
#include <getopt.h>  
int s, E, b;//
char t[100];//����������
int hits, misses, evicts=0;
struct cacheLine
{
	int valid;//��Ч
	int tag;//���
	int block;//��
	int time;//����ʱ��
};
struct cacheLine** cache=NULL;//��������
void error() {
	printf("There is a mistake.\n"
		"Here is a example:\n"
		"Linux> ./csim -s 4 -E 1 -b 4 -t traces/yi.trace"
		);
}
void updateTime() {//ÿ������ʱ������
	int S = 1<<s ;
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < E; ++j) {
			if (cache[i][j].valid == 1) {
				++cache[i][j].time;
			}
		}
	}
}
void update(unsigned int address){
	int groupIndex= (address >> b) & ((unsigned)(-1) >> (64 - s));//ȡ�м�sλ,����
	int tagIndex = ((unsigned)address) >> (b + s);//��Ǳ��
	for (int i = 0; i < E; ++i) //hit
	{
		if (cache[groupIndex][i].valid==1) {
			if (cache[groupIndex][i].tag == tagIndex)
			{
				cache[groupIndex][i].time = 0;
				hits++;
				return;
			}
		}
	}
	//û��hit
	for (int i = 0; i < E; ++i) 
	{
		if (cache[groupIndex][i].valid == 0)
		{
			cache[groupIndex][i].valid = 1;
			cache[groupIndex][i].tag = tagIndex;
			cache[groupIndex][i].time = 0;
			misses++;
			return;
		}
	}
	// LRU����
	evicts++;
	misses++;
	int maxIndex=-1;
	int timeMax = -1;
	for (int i = 0; i < E; ++i)
	{
		if (cache[groupIndex][i].time >timeMax )
		{
			timeMax = cache[groupIndex][i].time;
			maxIndex = i;
		}
	}
	cache[groupIndex][maxIndex].tag = tagIndex;
	cache[groupIndex][maxIndex].time = 0;
	return;
}

void initCache() {//��ʼ������
	int S = 1<<s;
	cache = (struct cacheLine**)malloc(S*sizeof(struct cacheLine *));
	for (int i = 0; i < S; i++) {
		cache[i] =(struct cacheLine*) malloc(E * sizeof(struct cacheLine));
		for (int j = 0; j < E;j++) {
			cache[i][j].valid = 0;
			cache[i][j].tag = 0;
			cache[i][j].block = 0;
			cache[i][j].time = 0;
		}
	}

}

int process() {//������
	FILE* fp = fopen(t, "r");
	if (fp==NULL) {
		return -1;
	}
	char op;//����
	int address;//��ַ
	int size;//��С
	while (fscanf(fp, "%c %x %d", &op, &address, &size) > 0)
	{
		switch (op)
		{
		case'I':
			continue;
		case 'L':
			update(address);
			break;
		case 'M':
			update(address);
		case 'S':
			update(address);
		}
		updateTime();
	}
	fclose(fp);
	int S = 1<<s;
	for (int i = 0; i < S; ++i) {
		free(cache[i]);
	}
	free(cache);
	return 0;
}
int main(int argc, char** argv)
{
	const char* optstring = "s:E:b:t:";//���ö�ȡ��ʽ
	int opt = 0;//���ܷ��صķ���
	while (-1 !=(opt = getopt(argc, argv, optstring)))//-1Ϊ����
	{
		switch (opt)
		{
		case 's':
			s = atoi(optarg);//optarg���ַ���
			break;
		case 'E':
			E = atoi(optarg);
			break;
		case 'b':
			b = atoi(optarg);
			break;
		case 't':
			strcpy(t, optarg);
			break;
		default:
			error();
			break;
		}
	}
	if (s <= 0 || E <= 0 || b <= 0 || t == NULL) {// ���ѡ��������ϸ���˳�
		return -1;
	}
	FILE* fp = fopen(t, "r");
	if (fp == NULL)
	{
		printf("open error");
		exit(-1);
	}
	initCache();
	process();
	printSummary(hits, misses, evicts);
	return 0;
}
