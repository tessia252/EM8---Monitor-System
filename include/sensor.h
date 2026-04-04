#ifndef SENSOR_H
#define SENSOR_H
#include "common.h"
#include <time.h>
int readConfig(const char *filename, Sensor sensors[], int *numSensors);
float receiveData(int sensorID);
int TimetoSendData(Sensor *s);
int isSensorDisconnected(Sensor s);
#endif
