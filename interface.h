/*
 * interface.h
 *
 *  Created on: Mar 15, 2019
 *      Author: Elijah Basile
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "heap.h"

void menu();
void addPatient(HEAP*);
void retrieveNextPatient(HEAP*);
HEAP* printPatientList(HEAP*);
void printPatientInfo(PATIENT*);
int compare(PATIENT*,PATIENT*);
void buildHeapFromFile(HEAP* tree,FILE* fptr);
PATIENT* convertFormatToHeap(HEAP* heap,char* line);
void convertData(HEAP* tree);
int compareArrival(PATIENT*,PATIENT*);
int parseArrival(PATIENT*);
int compareId (PATIENT*,PATIENT*);
int parseId (PATIENT*);
char* getId (void);

#endif /* INTERFACE_H_ */
