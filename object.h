#pragma once
#include <stdio.h>
#include "face.h"
#include "vertex.h"

typedef struct
{
	// Array of all Vertexes
	int numberOfVertexes;
	Vertex* vertexes;
	// Array of all Faces
	int numberOfFaces;
	Face* faces;
} Object;

Object* createObject(char* filename);
void printFaces(Object* ptr, void* numberOfTriangularFaces);
void printVertexes(Object* ptr, void* numberOfVertexes);
void getTotalArea(Object* ptr, void* totalAreaOfTriangularFac);
void calculateFaceArea(Face* pF, Vertex* pV, double* area);

void transformObject(char* originalObjectFileName, char* deformedObjectFileName);
void saveObjectTextFormat(FILE* fp, Object* pO);
void saveObjectBinaryFormat(FILE* fp, Object* pO);
Object* loadObjectTextFormat(FILE* fp);
Object* loadObjectBinaryFormat(FILE* fp);
void freeObject(Object* pO);
