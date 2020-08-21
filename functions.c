#include "Header.h"

int read_file(FILE* infile, FitbitData fitbitData[])
{
	//tok = strtok(NULL, ",")
	char delim[2] = ",";
	char line[100];
	char* token;
	int i = 0;

	//while (!feof(infile))
	for(i = 0; i < numEntries; i++)
	{
		
		//Reads entire line
		fgets(line, 100, infile);

		//1. Reads the patient's name and adds it to fitbitData
		token = strtok(line, delim);
		if (strcmp(token, "12cx7") == 0)
		{
			strcpy(fitbitData[i].patient, token);
		} 
		else 
		{
			strcpy(fitbitData[i].patient, "ERROR");
		}

		//2. Reads the minute and adds it to fitbitData
		token = strtok(NULL, delim);
		strcpy(fitbitData[i].minute, token);

		//3. Reads the calories and adds it to fitbitData
		token = strtok(NULL, delim);
		fitbitData[i].calories = atof(token);

		//4. Reads the distance and adds it to fitbitData
		token = strtok(NULL, delim);
		fitbitData[i].distance = atof(token);

		//5. Reads the floors and adds it to fitbitData
		token = strtok(NULL, delim);
		fitbitData[i].floors = atoi(token);

		//6. Reads the heartRate and adds it to fitbitData
		token = strtok(NULL, delim);
		if (token != NULL)
		{
			fitbitData[i].heartRate = atoi(token);
		}
		else
		{
			fitbitData[i].heartRate = -1;
		}

		//7. Reads the steps and adds it to fitbitData
		token = strtok(NULL, delim);
		fitbitData[i].steps = atoi(token);

		//8. Reads the sleep level and adds it to fitbitData
		token = strtok(NULL, delim);
		if (token != NULL)
		{
			fitbitData[i].sleepLevel = atoi(token);
		}
		else
		{
			fitbitData[i].sleepLevel = -1;
		}
		//i++;                                                                                                                                  
	}

	return i;
}

void printFitbitData(FitbitData fitbitData[], FILE* outfile)
{
	for (int i = 0; i < numEntries; i++)
	{
		if (strcmp(fitbitData[i].patient, "ERROR") != 0)
		{
			fprintf(outfile, "%s, ", fitbitData[i].patient);
			fprintf(outfile, "%s, ", fitbitData[i].minute);
			fprintf(outfile, "%lf, ", fitbitData[i].calories);
			fprintf(outfile, "%lf, ", fitbitData[i].distance);
			fprintf(outfile, "%d, ", fitbitData[i].floors);
			fprintf(outfile, "%d, ", fitbitData[i].heartRate);
			fprintf(outfile, "%d, ", fitbitData[i].steps);
			fprintf(outfile, "%d\n", fitbitData[i].sleepLevel);
		}
	}
}

//Must compute the total calories burned, distance walked in miles, floors walked, and steps taken
void calculateTotals(FitbitData fitbitData[], double *ptrCal, double *ptrDistance, int *ptrFloors, int *ptrSteps)
{
	double calories = 0.0, distance = 0.0;
	int floors = 0, steps = 0;
	for (int i = 0; i < numEntries; i++)
	{
		if (strcmp(fitbitData[i].patient, "ERROR") != 0)
		{
			calories += fitbitData[i].calories;
			distance += fitbitData[i].distance;
			floors += fitbitData[i].floors;
			steps += fitbitData[i].steps;
		}
	}
	*ptrCal = calories;
	*ptrDistance = distance;
	*ptrFloors = floors;
	*ptrSteps = steps;
}

//Compute average heartrate over the 24-hour period
double calculateHeartRate(FitbitData fitbitData[])
{
	double heartBeat = 0.0, avgHeartBeat = 0.0;
	int count = 0;
	for (int i = 0; i < numEntries; i++)
	{
		if (fitbitData[i].heartRate > 0 && strcmp(fitbitData[i].patient, "ERROR") != 0)
		{
			heartBeat += fitbitData[i].heartRate;
			count++;
		}
	}

	avgHeartBeat = heartBeat / count;

	return avgHeartBeat;
}

//Record the max steps taken in one minute over the 24-hour period; if there are multiple minutes throughout the day where the max
//is discovered, then you must report the one that is the latest in the 24-hour period
void findMaxSteps(FitbitData fitbitData[], int *ptrMaxSteps, int *ptrIndexOfMaxSteps)
{
	int maxSteps = fitbitData[0].steps, index = 0;
	for (int i = 0; i < numEntries; i++)
	{
		if (maxSteps <= fitbitData[i].steps && strcmp(fitbitData[i].patient, "ERROR") != 0)
		{
			maxSteps = fitbitData[i].steps;
			index = i;
		}
	}
	*ptrMaxSteps = maxSteps;
	*ptrIndexOfMaxSteps = index;
}

void cleansingData(FitbitData fitbitData[])
{
	for (int i = 0; i < numEntries; i++)
	{
		for (int j = 0; j < numEntries; j++)
		{
			if (strcmp(fitbitData[i].patient, "ERROR") != 0 && strcmp(fitbitData[j].patient, "ERROR") != 0
				&& strcmp(fitbitData[i].minute, fitbitData[j].minute) == 0 && i != j)
			{
				strcpy(fitbitData[i].patient, "ERROR");
				strcpy(fitbitData[i].minute, "ERROR");
				fitbitData[i].calories = -1;
				fitbitData[i].distance = -1;
				fitbitData[i].floors = -1;
				fitbitData[i].heartRate = -1;
				fitbitData[i].steps = -1;
				fitbitData[i].sleepLevel = -1;
			}
		}
	}
}

//You must report the longest consecutive range of poor sleep; a range is defined as one or more consecutive minutes where the sleepLevel > 1; 
//the poorest sleep is not based on teh length of the range, but the sum of the sleep levels in the range; the max sum of the ranges
//is considered the poorest sleep (report the starting and ending minutes of range)

void rangePoorSleep(FitbitData fitbitData[], int* ptrStartSleep, int* ptrEndSleep)
{
	int startSleep = 0, endSleep = 0, tempIndex = 0;
	int rangeSleep = 0, countSleep = 0;

	for (int i = 0; i < numEntries; i++)
	{
		if (fitbitData[i].sleepLevel > 1)
		{
			tempIndex++;
			countSleep += fitbitData[i].sleepLevel;
		}
		else
		{
			if (countSleep > rangeSleep)
			{
				startSleep = i - tempIndex;
				endSleep = i - 1;
				rangeSleep = countSleep;
			}
			tempIndex = 0;
			countSleep = 0;
		}
	}
	*ptrStartSleep = startSleep;
	*ptrEndSleep = endSleep;
}