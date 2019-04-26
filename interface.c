/*
 * interface.c
 *
 *  Created on: Mar 15, 2019
 *      Author: Elijah Basile
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "interface.h"
#include "heap.h"

#define MAXSIZE 200

// constant value to keep sorting stable
int patientsEntered = 0;

// user interface for using the emergency room queue
void menu() {
	// local definitions
	char option;

	//create heap
	HEAP* heap = heapCreate(MAXSIZE,(compare));

	// if out of memory
	if (!heap)
		exit(100);

	// welcome user
	printf ("================================\n");
	printf ("===== Emergency Room Queue =====\n");
	printf ("================================\n\n");

	//statements
	do {
		printf("============= Menu =============\n");
		printf(" f :	Extract patients from file \n");
		printf(" e :	Enter new patient \n");
		printf(" r : 	Retrieve next patient. \n");
		printf(" p : 	Print all patients. \n");
		printf(" q :	Exit.\n");

		fflush(stdin);
		option = getchar();

		switch (option) {
			case 'f':
				convertData(heap);
				break;
			case 'e':
				addPatient(heap);
				break;
			case 'r':
				retrieveNextPatient(heap);
				break;
			case 'p':
				heap = printPatientList(heap);
				break;
			case 'q':
				break;
		}

		if (option == 'q')
			break;

		fflush (stdin);
		printf ("\nPress ENTER to Continue\n");
		getchar();


	} while (true);

	// deallocate heap and its contents
	heapDestroy(heap);
	printf ("\nExited Emergency Room Queue Successfully.\n");
	return;
}

// converts csv file data into the tree in the form of nodes
void convertData(HEAP* tree) {
	FILE* fptr;
	char filename[256];
	bool isValid = false;
	char choice;

	do {
		fflush(stdin);
		printf("Enter the file you wish to extract the patient data from.\n");
		scanf("%s",filename);
		if (!(fptr = fopen(filename,"r"))) {
			printf("******* File Not Found *******\n");
			printf("e:		Enter a new file name \n");
			printf("q:		Quit this Menu.		  \n");
			while (!isValid) {
				isValid = true;
				fflush(stdin);
				choice = getchar();
				switch (choice) {
					case 'e':
						break;
					case 'q':
						return;
					default:
						printf("Enter valid input.\n");
						isValid = false;
				}
			}
		} else
			break;
	} while (true);
	buildHeapFromFile(tree,fptr);
}

// this builds tree from the file
void buildHeapFromFile(HEAP* tree,FILE* fptr) {
	char* line = (char*)malloc(sizeof(char)*256);

	while(!feof(fptr)) {
		fgets(line,256,fptr);
		heapInsert(tree,convertFormatToHeap(tree,line));
		patientsEntered++;
	}
	free(line);
}

// converts csv file data to a DATA node.
PATIENT* convertFormatToHeap(HEAP* heap,char* line) {\
	PATIENT* patient = (PATIENT*)malloc(sizeof(PATIENT));

	char* curr = line;

	strcpy(patient->first,"UNKNOWN");
	strcpy(patient->last,"UNKNOWN");

	for (int i = 0; i < 5 ; i++, curr++) {
		patient->id[i] = *curr;
	}
	(patient->id)[5] = '\0';

	curr++;curr++;

	patient->status = atoi(curr++);

	curr++;curr++;

	int i;
	for (i = 0; *curr != 'm' ; i++, curr++)
		(patient->arrival)[i] = *curr;
	(patient->arrival)[i] = 'm';
	(patient->arrival)[++i] = '\0';

	return patient;
}

// user enters a new patient into the heap
void addPatient(HEAP* heap) {
	PATIENT* new = (PATIENT*)malloc(sizeof(PATIENT));

	if (!new)
		return;

	struct tm* newArrival;

	time_t curtime;

	time(&curtime);

	newArrival =  localtime(&curtime);

	int hours = newArrival->tm_hour + 1;
	int minutes = newArrival->tm_min;

	//adjust for daylight savings time
	hours--;

	bool pm = false;
	if (hours > 11) {
		pm = true;
		if (hours > 12)
			hours = hours - 12;
	}
	if (hours == 0)
		hours = 12;

	char hours_string[3];
	char minutes_temp_string[3];
	char minutes_string[3] = "0";

	itoa (hours,hours_string,10);
	itoa (minutes,minutes_temp_string,10);
	if (minutes < 10)
		strcat(minutes_string,minutes_temp_string);
	else
		strcpy(minutes_string,minutes_temp_string);


	char arrivalTime[20] = "";

	strcat(arrivalTime,hours_string);
	strcat(arrivalTime,":");
	strcat(arrivalTime,minutes_string);
	strcpy(new->arrival,arrivalTime);
	if (pm == true)
		strcat(new->arrival,"pm");
	else
		strcat(new->arrival,"am");

	printf("Enter the first name of the patient.\n");
	fflush(stdin);
	scanf("%s",new->first);

	printf("Enter the last name of the patient.\n");
	fflush(stdin);
	scanf("%s",new->last);

	char* temp = getId();
	strcpy(new->id,temp);
	free(temp);

	do {
		printf("Enter the critical status of the patient.\n");
		fflush(stdin);
		scanf("%d",&(new->status));
	} while (new->status <= 0 || new->status > 5);

	if (heapInsert(heap,new))
		printf("Patient entered into system.\n");
	else
		printf("Failed to insert patient. No more system memory.\n");

	return;

}

char* getId() {
	patientsEntered++;
	int curr = patientsEntered;
	char* returnString = (char*)malloc(sizeof(char)*6);
	char* writer = returnString;
	int i = 0;
	while (curr >= 26) {
		*writer = 'z';
		writer++;
		i++;
		curr = curr - 26;
	}
	*writer = 'a' + curr;
	writer++;
	for (;i<5;i++) {
		*writer = 'a';
		writer++;
	}
	writer = '\0';

	return returnString;
}

// retrieves and displays next patient to user and removes patient from heap
void retrieveNextPatient(HEAP* heap) {
	if (heapEmpty(heap)) {
		printf("No patients left in system.\n");
		return;
	}

	void* patient;

	heapDelete(heap,&patient);

	printPatientInfo(patient);

	free(patient);

	return;
}

// prints a list of patients on heap in order of critical status
HEAP* printPatientList(HEAP* heap) {
	//create heap
	HEAP* newHeap = heapCreate(MAXSIZE,(*compare));
	void* patient;

	if (heapEmpty(heap)) {
		printf("No patients left in queue.\n");
		return heap;
	}


	printf ("Patient Listing:\n");
	while(!heapEmpty(heap)) {
		heapDelete(heap,&patient);
		heapInsert(newHeap,patient);
		printPatientInfo(patient);
	}
	printf("============================================\n");
	free(heap->heapAry);
	free(heap);
	return newHeap;
}

// prints patient info for patient entered into system
void printPatientInfo(PATIENT* patient) {
	printf("============================================\n");
	printf("Patient ID: %.5s\n",patient->id);
	printf("Arrival time: %s\n",patient->arrival);
	printf("Critical status: %d\n",patient->status);
	if (strcmp(patient->first,"UNKNOWN") != 0 && strcmp(patient->last,"UNKNOWN") != 0)
		printf("Name: %s, %s\n",patient->last,patient->first);
}

// built-in compare function for patients
int compare(PATIENT* p1, PATIENT* p2) {
	if (p2 == NULL)
		return 1;
	if (p1->status < p2->status) {
		return 1;
	} else if (p1->status > p2->status) {
		return -1;
	} else {
		return compareArrival(p1,p2);
	}
}

int compareArrival (PATIENT* p1, PATIENT* p2) {
	int p1Arrival = parseArrival(p1);
	int p2Arrival = parseArrival(p2);
	if (p1Arrival < p2Arrival) {
		return 1;
	} else if (p1Arrival > p2Arrival) {
		return -1;
	} else {
		return compareId(p1,p2);
	}
}

int parseArrival (PATIENT* patient) {
	int hours = (patient->arrival)[0] * 60;
	int minutes = atoi(&((patient->arrival)[2]));

	return hours + minutes;
}

int compareId (PATIENT* p1, PATIENT* p2) {
	int p1Id = parseId(p1);
	int p2Id = parseId(p2);
	if (p1Id < p2Id) {
		return 1;
	} else if (p1Id > p2Id) {
		return -1;
	} else {
		return 0;
	}
}

int parseId (PATIENT* patient) {
	int id_value = 0;
	for (int i = 0; i < 5; i++) {
		id_value += (patient->id)[i]*(pow(10,i));
	}
	return id_value;
}


