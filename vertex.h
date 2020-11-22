#pragma once
#include <stdio.h>
typedef struct
{
	float x; // Coordinates of Vertex
	float y;
	float z;
} Vertex;

void initVertex(Vertex* pV, float* x, float* y, float* z);
void saveVertexTextFormat(FILE* fp, Vertex* pV);
void saveVertexBinaryFormat(FILE* fp, Vertex* pV);
void loadVertexTextFormat(const char* temp, Vertex* pV);
void loadVertexBinaryFormat(FILE* fp, Vertex* pV);
