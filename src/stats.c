#include <stdio.h>
#include "processing.h"

void initStats(Stats *s){
	s->validCount = 0;
	s->errorCount = 0;
	s->exceedCount = 0;
	s->bufferOverflowCount = 0;
	s->disconnectCount = 0;
}
void updateStats(Stats *s, float value){
    if(value <= -9999.0f){
        recordError(s);
    }
    else{
        recordValid(s);
    }

}
void recordValid(Stats *s){
	s->validCount++;
}
void recordExceed(Stats *s){
	s->exceedCount++;
}
void recordDisconnect(Stats *s){
	s->disconnectCount++;
}
void recordError(Stats *s){
	s->errorCount++;
}
void recordOverflow(Stats *s){
	s->bufferOverflowCount++;
}
void printReport(Stats s){
	printf("\n==========REPORT==========\n");
	printf("Valid: %d\n", s.validCount);
	printf("Error: %d\n", s.errorCount);
	printf("Exceed: %d\n", s.exceedCount);
	printf("Overflow: %d\n", s.bufferOverflowCount);
	printf("Disconnect: %d\n", s.disconnectCount);
}
