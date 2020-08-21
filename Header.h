#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define numEntries 1444

typedef enum sleep
{
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

typedef struct fitbit
{
	char patient[10];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;

int read_file(FILE* infile, FitbitData fitbitData[]);

void printFitbitData(FitbitData fitbitData[], FILE* outfile);

void calculateTotals(FitbitData fitbitData[], double* ptrCal, double* ptrDistance, int* ptrFloors, int* ptrSteps);

double calculateHeartRate(FitbitData fitbitData[]);

void findMaxSteps(FitbitData fitbitData[], int* ptrMaxSteps, int* ptrIndexOfMaxSteps);

void cleansingData(FitbitData fitbitData[]);

void rangePoorSleep(FitbitData fitbitData[], int* ptrStartSleep, int* ptrEndSleep);
