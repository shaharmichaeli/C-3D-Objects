#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vertex.h"
#include "face.h"
#include "object.h"
#define STARTSIZE 1000
#define TRIANGLE3 3
#define VSTARTINDEX 1

Object* createObject(char* filename)
{
	Face* t;
	Vertex* v;
	Object* o = (Object*)malloc(sizeof(Object));
	if (!o) {
		return NULL;
	}
	if (filename == NULL) {
		return NULL;
	}
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		return NULL;
	}

	int numberOfFaces = 0, numberOfVertexes = VSTARTINDEX, logicNumberofFaces = STARTSIZE, logicNumberofVertex = STARTSIZE;
	o->vertexes = (Vertex*)malloc(sizeof(Vertex) * logicNumberofVertex);
	o->faces = (Face*)malloc(sizeof(Face) * logicNumberofFaces);
	if (!o->vertexes || !o->faces) {
		return NULL;
	}

	float x = 0, y = 0, z = 0;
	int FLAG = 0;
	char c;
	char str[STARTSIZE];

	while (fgets(str, STARTSIZE, fp)) {
		if (str[0] == 'v') {
			if (numberOfVertexes == logicNumberofVertex) {
				logicNumberofVertex *= 2;
				v = (Vertex*)realloc(o->vertexes, logicNumberofVertex * sizeof(Vertex));
				if (v != NULL) {
					o->vertexes = v;
				}

			}
			if (sscanf(str, "%c %f %f %f", &c, &x, &y, &z) == 4) {
				initVertex(&o->vertexes[numberOfVertexes], &x, &y, &z);
				numberOfVertexes++;
			}
		}
		else if (str[0] == 'f') {
			if (sscanf(str, "%c %d", &c, &FLAG) != 2) {
				continue;
			}
			if (numberOfFaces == logicNumberofFaces) {
				logicNumberofFaces *= 2;
				t = (Face*)realloc(o->faces, logicNumberofFaces * sizeof(Face));
				if (t != NULL) {
					o->faces = t;
				}
			}
			initFace(&o->faces[numberOfFaces], str, (int)strlen(str));
			numberOfFaces++;

		}
	}
	o->numberOfFaces = numberOfFaces;
	o->numberOfVertexes = numberOfVertexes;
	return o;

}


void printFaces(Object* ptr, void* numberOfTriangularFaces) {
	int count = 0;

	for (int i = 0; i < ptr->numberOfFaces; i++) {
		if (ptr->faces[i].size == TRIANGLE3) {
			count++;
		}
	}
	int* s = (int*)numberOfTriangularFaces;
	*s = count;
	return;
}
void printVertexes(Object* ptr, void* numberOfVertexes) {
	int* num = (int*)numberOfVertexes;
	*num = ptr->numberOfVertexes-1;
}

void getTotalArea(Object* ptr, void* totalAreaOfTriangularFaces) {
	double* sumArea = (double*)totalAreaOfTriangularFaces;
	for (int i = 0; i < ptr->numberOfFaces; i++)
	{
		if (ptr->faces[i].size == TRIANGLE3) {
			calculateFaceArea(&ptr->faces[i], ptr->vertexes, sumArea);
		}
	}

}
void calculateFaceArea(Face* pF, Vertex* vertexArr, double* sumArea) {
	Vertex A = vertexArr[pF->vertex[0]];
	Vertex B = vertexArr[pF->vertex[1]];
	Vertex C = vertexArr[pF->vertex[2]];

	Vertex AB = { A.x - B.x ,A.y - B.y ,A.z - B.z };
	Vertex AC = { A.x - C.x , A.y - C.y, A.z - C.z };

	float ABACx = AB.x * AC.x;
	float ABACy = AB.y * AC.y;
	float ABACz = AB.z * AC.z;

	double ABSIZE = sqrt(pow(AB.x, 2) + pow(AB.y, 2) + pow(AB.z, 2));
	double ACSIZE = sqrt(pow(AC.x, 2) + pow(AC.y, 2) + pow(AC.z, 2));
	double dABAC = ((double)ABACx + (double)ABACy + (double)ABACz);
	double angle = acos(dABAC / (ABSIZE * ACSIZE));

	*sumArea += 0.5 * sin(angle) * ABSIZE * ACSIZE;

}


void transformObject(char* originalObjectFileName, char* deformedObjectFileName) {
	FILE* fSOURCE = fopen(originalObjectFileName, "r");
	FILE* fDEST = fopen(deformedObjectFileName, "w");
	if (!fSOURCE || !fDEST) {
		return;
	}
	char c;
	char temp[STARTSIZE];
	Face f;
	float x = 0, y = 0, z = 0;
	while (fgets(temp, STARTSIZE, fSOURCE)) {
		sscanf(temp, "%c", &c);
		if (c == 'v') {
			if (sscanf(temp, "%c %f %f %f", &c, &x, &y, &z) == 4) {
				x = x * 3;
				fprintf(fDEST, "%c %f %f %f", c, x, y, z);
				fprintf(fDEST, "\n");
			}
		}else if (c == 'f') {	
				initFace(&f, temp,(int)strlen(temp));
				saveFaceTextFormat(fDEST, &f);
			}
		}
	fclose(fSOURCE);
	fclose(fDEST);
}


void saveObjectTextFormat(FILE* fp, Object* pO) {
	// FILE FORMAT : NUM OF VERTEX , ALL VERTEX -->> NUM OF FACES , ALL FACES.

	fprintf(fp, "%d %d\n", pO->numberOfVertexes, pO->numberOfFaces);
	for (int i = 1; i < pO->numberOfVertexes; i++)
	{

		saveVertexTextFormat(fp, &pO->vertexes[i]);
	}
	for (int i = 0; i < pO->numberOfFaces; i++)
	{
		saveFaceTextFormat(fp, &pO->faces[i]); // everyline print the vertexes id
	}
	
}



void saveObjectBinaryFormat(FILE* fp, Object* pO) {
	if (!fp || !pO) {
		return;
	}
	fwrite(&pO->numberOfVertexes, sizeof(int), 1, fp);
	fwrite(pO->vertexes, sizeof(Vertex), pO->numberOfVertexes, fp);

	fwrite(&pO->numberOfFaces, sizeof(int), 1, fp);
	for (int i = 0; i < pO->numberOfFaces ; i++)
	{
		Face* pF = &pO->faces[i];
		fwrite(&pF->size,sizeof(int),1,fp);
		fwrite(pF->vertex, sizeof(int), pF->size, fp);
	}
}


Object* loadObjectTextFormat(FILE* fp) {
	// FILE FORMAT : NUM OF VERTEX , ALL VERTEX -->> NUM OF FACES , ALL FACES.
	char FVcheck;
	char str[STARTSIZE];
	int countVertex = VSTARTINDEX;
	int countFaces = 0;
	Object* pO = (Object*)malloc(sizeof(Object));
	if (!pO) {
		return NULL;
	}
	if (fscanf(fp, "%d %d", &pO->numberOfVertexes, &pO->numberOfFaces) == -1) {
		return NULL;
	}
	pO->vertexes = (Vertex*)malloc(pO->numberOfVertexes * sizeof(Vertex));
	pO->faces = (Face*)malloc(sizeof(Face) * pO->numberOfFaces);
	if (!pO->faces || !pO->vertexes) {
		return NULL;
	}
	for (int i = 0; i < pO->numberOfVertexes; i++)
	{
		fgets(str, STARTSIZE, fp);
		sscanf(str, "%c", &FVcheck);
		if (FVcheck == 'v') {
			loadVertexTextFormat(str, &pO->vertexes[countVertex]);
			countVertex++;
		}
	}
	for (int i = 0; i < pO->numberOfFaces; i++)
	{
		fgets(str, STARTSIZE, fp);
		sscanf(str, "%c", &FVcheck);
		if (FVcheck == 'f') {
			loadFaceTextFormat(str, &pO->faces[countFaces]);
			countFaces++;
		}
	}

	fgets(str, STARTSIZE, fp); // clean buffer
	return pO;
}

Object* loadObjectBinaryFormat(FILE* fp) {
	Object* pO = (Object*)malloc(sizeof(Object));
	if (!pO) {
		return NULL;
	}

	fread(&pO->numberOfVertexes, sizeof(int), 1, fp);
	pO->vertexes = (Vertex*)malloc(sizeof(Vertex) * pO->numberOfVertexes);
	if (!pO->vertexes) {
		return NULL;
	}
	fread(pO->vertexes, sizeof(Vertex), pO->numberOfVertexes, fp);
	fread(&pO->numberOfFaces, sizeof(int), 1, fp);
	pO->faces = (Face*)malloc(sizeof(Face) * pO->numberOfFaces);
	if (!pO->faces) {
		return NULL;
	}
	for (int i = 0; i < pO->numberOfFaces; i++)
	{
		Face* pF = &pO->faces[i];
		loadFaceBinaryFormat(fp, pF);
	}
	return pO;

}

void freeObject(Object* pO) {
	for (int i = 0; i < pO->numberOfFaces; i++) {
		free(pO->faces[i].vertex);
	}
	free(pO->vertexes);
	free(pO->faces);
}



