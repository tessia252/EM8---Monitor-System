#include <stdio.h>
#include "processing.h"
#include <string.h>
static void writeLog(const char* level, const char* message) {
    FILE *f = fopen("system_log.txt", "a");
    if (f != NULL) {
        time_t now = time(NULL);
        char *t = ctime(&now);
        if (t != NULL) {
            t[strlen(t)-1] = '\0'; 
            fprintf(f, "[%s] [%s] %s\n", t, level, message);
        }
        fclose(f);
    }
}
void logMessage(FILE *logFile, const char *msg) {
    if (logFile) {
        fprintf(logFile, "%s\n", msg);
    }
}
void initStats(Stats *s){
	s->validCount = 0;
	s->errorCount = 0;
	s->exceedCount = 0;
	s->bufferOverflowCount = 0;
	s->disconnectCount = 0;
	FILE *f = fopen("system_log.txt", "w");
    if(f) {
        fprintf(f, "=== HE THONG BAT DAU GIAM SAT ===\n");
        fclose(f);
    }
}
void updateStats(Stats *s, float value){}
void recordValid(Stats *s){
	s->validCount++;
}
void recordExceed(Stats *s){
	s->exceedCount++;
	writeLog("WARNING", "Gia tri vuot nguong canh bao!");
}
void recordDisconnect(Stats *s){
	s->disconnectCount++;
	writeLog("ERROR", "Cam bien bi mat ket noi!");
}
void recordError(Stats *s){
	s->errorCount++;
	writeLog("ERROR", "Ban tin loi hoac du lieu khong hop le!");
}
void recordOverflow(Stats *s){
	s->bufferOverflowCount++;
	writeLog("CRITICAL", "Bo dem day! Du lieu cu da bi ghi de.");
}
void printReport(Stats s,FILE *logFile){
	printf("\n==========REPORT==========\n");
	if (logFile)
		fprintf(logFile,"\n==========REPORT==========\n");
	printf("Valid: %d\n", s.validCount);
	if (logFile)
		fprintf(logFile,"Valid: %d\n", s.validCount);
	printf("Error: %d\n", s.errorCount);
	if (logFile)
		fprintf(logFile,"Error: %d\n", s.errorCount);
	printf("Exceed: %d\n", s.exceedCount);
	if (logFile)
		fprintf(logFile,"Exceed: %d\n", s.exceedCount);
	printf("Overflow: %d\n", s.bufferOverflowCount);
	if (logFile)
		fprintf(logFile,"Overflow: %d\n", s.bufferOverflowCount);
	printf("Disconnect: %d\n", s.disconnectCount);
	if(logFile)
		fprintf(logFile,"Disconnect: %d\n", s.disconnectCount);
}
