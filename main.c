#include "Header.h"

/**********************************************************
Programmer: Koji Natsuhara
Class: CptS 122, Spring 2020, Lab Section 3
Programming Assignment: Programming Assignment 1: Analyzing Fitbit Data
Date: January 24, 2020
Description: This program will analyze data that was generated from a Fitbit 
devices. The program will to filter any data that is not related to the 
target patient (12cx7). It will also dedupe any entries that appear multiple
times and perform data cleansing any entries that have missing fields. It will
also calculate the totals of the data once the data has been cleansed.
**********************************************************/

int main(void)
{
	FILE* infile = fopen("FitbitData.csv", "r");
	FILE* outfile = fopen("Results.csv", "w");

	if (infile == NULL)
	{
		printf("Infile did not open.\n");
		return 1;
	}


	FitbitData fitbitData[numEntries] = { 0 };
	int numberEntries = 0, totalFloors = 0, totalSteps = 0, maxSteps = 0, indexOfMaxSteps = 0;
	int startSleep = 0, endSleep = 0;
	double totalCalories = 0.0, totalDistance = 0.0, avgHeartRate = 0.0;

	numberEntries = read_file(infile, fitbitData);
	cleansingData(fitbitData);
	calculateTotals(fitbitData, &totalCalories, &totalDistance, &totalFloors, &totalSteps);
	avgHeartRate = calculateHeartRate(fitbitData);
	findMaxSteps(fitbitData, &maxSteps, &indexOfMaxSteps);
	rangePoorSleep(fitbitData, &startSleep, &endSleep);

	printf("Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep\n");
	printf("%.5lf, %.5lf, %d, %d, %.5lf, %d, %s : %s\n", totalCalories, totalDistance, totalFloors, totalSteps, 
		avgHeartRate, maxSteps, fitbitData[startSleep].minute, fitbitData[endSleep].minute);

	fprintf(outfile, "Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep\n");
	fprintf(outfile, "%.5lf, %.5lf, %d, %d, %.5lf, %d, %s : %s\n", totalCalories, totalDistance, totalFloors, totalSteps,
		avgHeartRate, maxSteps, fitbitData[startSleep].minute, fitbitData[endSleep].minute);

	printFitbitData(fitbitData, outfile);

	//Must output these results to "Results.csv" and to the screen

	//Line 1: Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep
	//Line 2: valueCalories, valueDistance, valueFloors, valueSteps, valueHeartrate, valueMax, valueSleepStart:valueSleepEnd

	fclose(infile);
	fclose(outfile);

	return 0;
}