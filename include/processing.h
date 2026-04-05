#ifndef PROCESSING_H
#define PROCESSING_H
#include "common.h"
#include "sensor.h"
#include "buffer.h"
int isValidData(float value);
int checkThreshold(Sensor s, float value);
float filterData(Buffer *b);
#endif
