#pragma once
#include <stdio.h>

typedef struct
{
	int size;    // Number of vertexes of the face
	int* vertex; // Array of Ids of the vertexes
} Face;

void initFace(Face* pF, const char* str, int length);
void saveFaceTextFormat(FILE* fp, Face* pF);
void saveFaceBinaryFormat(FILE* fp, Face* pF);
void loadFaceTextFormat(const char* temp, Face* pF);
void loadFaceBinaryFormat(FILE* fp, Face* pF);
