#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vertex.h"
#include "face.h"

void initFace(Face* pF, const char* str, int length) {
	int logicSize = 1, physSize = 0;
	int* pArr;
	pF->vertex = (int*)malloc(sizeof(int) * logicSize);
	if (!pF->vertex) {
		return;
	}
	char c[2] = { 0, '\0' };
	char temp[100] = { 0 };

	// Running on all chars in the string and strcat when find space or '/' and jump to space
	for (const char* p = str; p < str + length; p++)
	{
		if (isdigit(*p)) {
			c[0] = *p;
			strcat(temp, c);
		}
		if (*p == '/' || isspace(*p)) {
			if (logicSize == physSize) {
				logicSize++;
				pArr = (int*)realloc(pF->vertex, logicSize * sizeof(int));
				if (pArr != NULL) {
					pF->vertex = pArr;
				}
			}
			if (strcmp(temp, "") != 0) {
				pF->vertex[physSize] = atoi(temp);
				physSize++;
			}
			p = strchr(p, ' ');
			if (!p) {
				pF->size = physSize;
				return;
			}
			memset(temp, 0, 100 * sizeof(char));
		}
	}
	pF->size = physSize;

}

void saveFaceTextFormat(FILE* fp, Face* pF) {
	if (!pF || !fp) {
		return;
	}
	fprintf(fp, "%c ", 'f');
	for (int i = 0; i < pF->size; i++)
	{
		fprintf(fp, "%d ", pF->vertex[i]);
	}
	fprintf(fp, "\n");
}


void saveFaceBinaryFormat(FILE* fp, Face* pF) {
	if (pF == NULL || fp == NULL) {
		return;
	}
	// save size , and save size * Faces in binary.
	fwrite(&pF->size, sizeof(int), 1, fp);
	fwrite(pF->vertex, sizeof(int), pF->size, fp);
}

void loadFaceTextFormat(const char* temp, Face* pF) {
	if (pF == NULL){
		return;
	}
	initFace(pF, temp,(int)strlen(temp));
}

void loadFaceBinaryFormat(FILE* fp, Face* pF) {
	if (pF == NULL || fp == NULL) {
		return;
	}
	fread(&pF->size, sizeof(int), 1, fp);
	pF->vertex = (int*)malloc(sizeof(int) * pF->size);
	if (!pF->vertex) {
		return;
	}
	fread(pF->vertex, sizeof(int), pF->size, fp);
}