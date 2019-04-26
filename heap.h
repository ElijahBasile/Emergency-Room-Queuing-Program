/*
 * heap.h
 *
 *  Created on: Mar 15, 2019
 *      Author: Elijah Basile
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <stdbool.h>

typedef struct {
	char id[6];
	char last [20];
	char first [20];
	int status;
	char arrival[20];
} PATIENT;

typedef struct {
	void** heapAry;
	int last;
	int size;
	int (*compare)(PATIENT* argu1, PATIENT* argu2);
	int maxSize;
} HEAP;

HEAP* heapCreate	(int maxSize,int(*compare)(PATIENT* arg1,PATIENT* arg2));
bool heapInsert		(HEAP* heap, void* dataPtr);
bool heapDelete		(HEAP* heap, void** dataOutPtr);
int heapCount		(HEAP* heap);
bool heapFull		(HEAP* heap);
bool heapEmpty		(HEAP* heap);
bool heapDestroy	(HEAP* heap);

void _reheapUp	(HEAP* heap, int childLoc);
void _reheapDown	(HEAP* heap, int root);

#endif /* HEAP_H_ */
