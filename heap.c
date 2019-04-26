/*
 * heap.c
 *
 *  Created on: Mar 15, 2019
 *      Author: Elijah Basile
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "heap.h"
#include "interface.h"

// allocates memory for heap and returns address of heap head structures
HEAP* heapCreate	(int maxSize,int(*compare)(PATIENT* arg1,PATIENT* arg2)) {
	// local definitions
	HEAP* heap;

	// statements
	heap = (HEAP*)malloc(sizeof(HEAP));
	if (!heap)
		return NULL;

	heap->last = -1;
	heap->compare = compare;
	heap->size = 0;

	// force heap size to power of 2 -1
	heap->maxSize = (int) pow(2,ceil(log2(maxSize))) -1;
	heap->heapAry = (void*)calloc(heap->maxSize,sizeof(void*));

	return heap;

} // heapCreate

bool heapInsert		(HEAP* heap, void* dataPtr) {
	//statements
	if (heap->size == 0) { // heap empty
		heap->size = 1;
		heap->last = 0;
		heap->heapAry[heap->last] = dataPtr;
		return true;
	}

	if (heap->last == heap->maxSize - 1)
		return false;
	++(heap->last);
	++(heap->size);
	heap->heapAry[heap->last] = dataPtr;
	_reheapUp(heap,heap->last);

	return true;
} // heapInsert

// delete the root of the heap and passes data back to caller
bool heapDelete		(HEAP* heap, void** dataOutPtr) {
	if (heap->size == 0)
		// heap empty
		return false;

	*dataOutPtr = heap->heapAry[0];
	heap->heapAry[0] = heap->heapAry[heap->last];
	(heap->last)--;
	(heap->size)--;
	_reheapDown(heap,0);
	return true;
} // heapDelete

int heapCount		(HEAP* heap) {
	if (heap == NULL)
		return 0;
	return heap->size;
}

bool heapFull		(HEAP* heap) {
	if (heap->maxSize == heapCount(heap))
		return true;
	return false;
}

bool heapEmpty		(HEAP* heap) {
	if (heap == NULL)
		return true;
	if (heapCount(heap) == 0)
		return true;
	return false;
}

//deallocates memory associated with heap
bool heapDestroy	(HEAP* heap) {
	void* dataOut;
	if (heap == NULL)
		return false;
	while(!heapEmpty(heap)) {
		heapDelete(heap,&dataOut);
		PATIENT* patient = dataOut;
		free(patient->arrival);
		free(patient);
	}
	free(heap->heapAry);
	free(heap);
	return true;
}

// reestablishes heap by moving data in child up to correct location heap array
void _reheapUp	(HEAP* heap, int childLoc) {
	// local definitions
	int parent;
	void** heapAry;
	void* hold;

	//statements
	//if not at root of heap -- index 0
	if (childLoc) {
		heapAry = heap->heapAry;
		parent = (childLoc - 1)/2;
		if (heap->compare(heapAry[childLoc],heapAry[parent]) > 0) {
			hold = heapAry[parent];
			heapAry[parent] = heapAry[childLoc];
			heapAry[childLoc] = hold;
			if (childLoc%2 == 1 && childLoc != 0) {
				// check left
				if (heap->compare(heapAry[childLoc],heapAry[childLoc-1]) > 0) {
					// left child larger -- swap
					hold = heapAry[childLoc-1];
					heapAry[childLoc-1] = heapAry[childLoc];
					heapAry[childLoc] = hold;
				}
			} else {
				// check right
				if (heap->compare(heapAry[childLoc],heapAry[childLoc+1]) < 0 && heapAry[childLoc+1] != NULL) {
					// right child larger -- swap
					hold = heapAry[childLoc+1];
					heapAry[childLoc+1] = heapAry[childLoc];
					heapAry[childLoc] = hold;
				}
			}
			_reheapUp(heap,parent);
		} // if heap[]
	} // if newNode
	return;
} // reheapUp

void _reheapDown	(HEAP* heap, int root) {
	// local definitions
	void* hold;
	void* leftData;
	void* rightData;
	int largeLoc;
	int last;

	//statements
	last = heap->last;
	if ((root * 2 + 1) <= last) //left subtree
		//there is at least one child
	{
		leftData = heap->heapAry[root * 2 + 1];
		if ((root * 2 + 2) <= last) // right substree
			rightData = heap->heapAry[root * 2 + 2];
		else
			rightData = NULL;

		// determine which tree is larger
		if ((!rightData) || heap->compare(leftData,rightData) > 0) {
			largeLoc = root * 2 + 1;
		} // if no right key or leftKey greater
		else {
			largeLoc = root * 2 + 2;
		} // else
		//test if root > larger subtree
		if (heap->compare(heap->heapAry[root],heap->heapAry[largeLoc]) < 0) {
			// parent < children
			hold = heap->heapAry[root];
			heap->heapAry[root] = heap->heapAry[largeLoc];
			heap->heapAry[largeLoc] = hold;
			_reheapDown (heap,largeLoc);
		} // if root <
	} // if root
} // _reheapDown









