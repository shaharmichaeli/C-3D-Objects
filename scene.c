#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "scene.h"
#include "object.h"

Scene* createScene(char* fileName, ...)
{
	Scene* pScene = (Scene*)malloc(sizeof(Scene));
	if (!pScene)
	{
		return NULL;
	}
	S_init(pScene);
	NODE* ptr = &pScene->head;
	va_list objects;
	va_start(objects, fileName);
	char* curr = fileName;
	while (curr != NULL)
	{
		Object* o = createObject(curr);
		ptr = S_insert(ptr, o);
		curr = va_arg(objects, char*);
	}
	va_end(objects);
	return pScene;
}

void saveScene(Scene* scene, char* fileName, FileType ftype) {
	if (scene == NULL) {
		return;
	}
	NODE* ptr = scene->head.next;
	if (ftype == TextFormat) {
		FILE* fp = fopen(fileName, "w");
		if (!fp) {
			return;
		}
		while (ptr != NULL) {
			saveObjectTextFormat(fp, ptr->key);
			fprintf(fp, "NNNNNNNNNNNNNN\n");
			ptr = ptr->next;
		}
		fclose(fp);
	}
	else if (ftype == BinaryFormat) {
		FILE* fp = fopen(fileName, "wb");
		if (!fp) {
			return;
		}
		while (ptr != NULL) {
			saveObjectBinaryFormat(fp, ptr->key);
			ptr = ptr->next;
		}
		fclose(fp);
	}
}

Scene* loadScene(const char* fileName, FileType ftype) {
	Scene* pScene = (Scene*)malloc(sizeof(Scene));
	if (!pScene) {
		return NULL;
	}
	S_init(pScene);
	Object* o;
	NODE* ptr = &pScene->head;
	if (ftype == TextFormat) {
		FILE* fp = fopen(fileName, "r");
		if (!fp) {
			free(pScene);
			return NULL;
		}
		while (ptr != NULL) {
			o = loadObjectTextFormat(fp);
			ptr = S_insert(ptr, o);
		}
		
		fclose(fp);
		return pScene;
	}
	else if (ftype == BinaryFormat) {
		FILE* fp = fopen(fileName, "rb");
		if (!fp) {
			free(pScene);
			return NULL;
		}
		while (ptr != NULL) {
			o = loadObjectBinaryFormat(fp);
			ptr = S_insert(ptr, o);
		}
		
		fclose(fp);
		return pScene;
	}
	return NULL;
	

}

BOOL S_init(Scene* pScene)
{
	if (pScene == NULL)
	{
		return False;
	}
	pScene->head.key = NULL;
	pScene->head.next = NULL;
	return True;
}

NODE* S_insert(NODE* pNode, DATA Value)
{
	NODE* o = (NODE*)malloc(sizeof(NODE));
	if (!o || Value == NULL)
	{
		return NULL;
	}
	if (pNode->next == NULL) {
		o->next = NULL;
	}
	else {
		o->next = pNode->next;
	}
	o->key = Value;
	pNode->next = o;
	return o;
}

NODE* S_delete(NODE* pNode)
{
	NODE* tmp;
	if (pNode == NULL)
	{
		return NULL;
	}
	tmp = pNode->next;
	if (tmp == NULL) {
		free(tmp);
		return NULL;
	}
	if (tmp->next == NULL)
	{
		free(tmp);
		return NULL;
	}
	pNode->next = tmp->next;
	freeObject(tmp->key);
	free(tmp);
	return pNode->next;
}

void perform(Scene* pScene, void (*cal)(Object*, void*), char* type, char* str)
{
	NODE* ptr = pScene->head.next;
	if (strcmp(type, "INT") == 0) {
		int sum = 0, objectSum = 0;
		while (ptr != NULL)
		{
			cal(ptr->key, &objectSum);
			sum += objectSum;
			ptr = ptr->next;
		}
		printf("%s %d\n", str, sum);
	}
	else if (strcmp(type, "DOUBLE") == 0) {
		double sum = 0, objectSum = 0;
		while (ptr != NULL) {
			cal(ptr->key, &objectSum);
			sum += objectSum;
			ptr = ptr->next;
		}
		printf("%s %f\n", str, sum);
	}
}

BOOL freeScene(Scene* pScene)
{
	NODE* tmp;
	if (!pScene)
		return False;

	tmp = &(pScene->head);
	do
	{
		tmp = S_delete(tmp);
	} while (tmp != NULL);
	return True;
}

