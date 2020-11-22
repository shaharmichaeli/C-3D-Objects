#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "face.h"
#include "vertex.h"

typedef enum { False, True } BOOL;
typedef enum { TextFormat, BinaryFormat } FileType;
typedef	Object* DATA;

typedef struct NODE
{
	DATA key;
	struct NODE* next;
}NODE;

typedef struct
{
	NODE head;
}Scene;

Scene* createScene(char* fileName, ...);
void	saveScene(Scene* scene, char* fileName, FileType ftype);
Scene* loadScene(const char* name, FileType ftype);
BOOL	S_init(Scene* pScene);
NODE* S_insert(NODE* pNode, DATA Value);
NODE* S_delete(NODE* pNode);
void	perform(Scene* pScene, void (*cal)(Object*, void*), char* type, char* str);
BOOL	freeScene(Scene* pScene);
