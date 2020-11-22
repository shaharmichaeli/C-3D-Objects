#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "vertex.h"
#define SOMESIZE 100
#define VLOADNUMBER 4

void initVertex(Vertex* pV, float* x, float* y, float* z) {
	pV->x = *x;
	pV->y = *y;
	pV->z = *z;
}

void saveVertexTextFormat(FILE* fp, Vertex* pV) {
	if (!pV || !fp) {
		return;
	}
	fprintf(fp, "%c %f %f %f\n", 'v', pV->x, pV->y, pV->z);
}
void saveVertexBinaryFormat(FILE* fp, Vertex* pV) {
	if (!pV || !fp) {
		return;
	}
	fwrite(pV, sizeof(Vertex), 1, fp);
}

void loadVertexTextFormat(const char* temp, Vertex* pV) {
	if (!pV ) {
		return;
	}
	char v;
	if (sscanf(temp, "%c %f %f %f", &v, &pV->x, &pV->y, &pV->z) != VLOADNUMBER) {
		printf("error");
		return;
	}
}

void loadVertexBinaryFormat(FILE* fp, Vertex* pV) {
	if (!pV || !fp) {
		return;
	}
	fread(pV, sizeof(pV), 1, fp);
}