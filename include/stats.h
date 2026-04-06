#ifndef STATS_H
#define STATS_H

#include "common.h"
#include "sensor.h"
#include <stdio.h>

void logMessage(FILE *logFile,const char *msg);
void initStats(Stats *s);

void recordValid(Stats *s);

/* sua: thêm sensorID */

void recordError(
    Stats *s
);

void recordDisconnect(
    Stats *s
);

void recordExceed(Stats *s);

void recordOverflow(Stats *s);

/* s?a: thêm Stats* */

void updateMaxValue(
    Stats *s,
    Sensor sensor,
    float value
);

void printReport(Stats s, FILE *logFile);

/* thêm hàm ðóng file */

void closeStats(Stats *s);

#endif
