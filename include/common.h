#ifndef COMMON_H
#define COMMON_H
#define MAX_SENSORS 30
#define BUFFER_SIZE 100
#include<stdio.h>
#include <time.h>
typedef struct{
	int id;
	char type[20];
	float threshold;
	int period;
	time_t lastTime;
	char location[30];
}Sensor;
typedef struct{
	float data[BUFFER_SIZE];
	int head;
	int tail;
	int count;
}Buffer;
typedef struct{
	int validCount;
    int errorCount;
    int disconnectCount;
    int exceedCount;
    int bufferOverflowCount;
    int errorBySensor[MAX_SENSORS];
    float maxTemp;
    float maxHumidity;
    FILE *logFile;
} Stats;

#endif
