#ifndef STATS_H
#define STATS_H
#include "common.h"
void initStats(Stats *s);
void updateStats(Stats *s, float value);
void recordValid(Stats *s);
void recordExceed(Stats *s);
void recordDisconnect(Stats *s);
void recordError(Stats *s);
void recordOverflow(Stats *s);
void printReport(Stats s);
#endif
