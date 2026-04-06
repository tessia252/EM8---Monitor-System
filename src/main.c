#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sensor.h"
#include "buffer.h"
#include "processing.h"
#include "stats.h"

int main() {
    srand(time(NULL));
    Sensor sensors[MAX_SENSORS];
    int numSensors = 0;
    char msg[200];
    Buffer buffer; 
    Stats stats;
    initBuffer(&buffer);
    initStats(&stats);

    if (readConfig("config.txt", sensors, &numSensors) != 0) {
        printf("Loi: Khong tim thay file config.txt\n");
        return 1;
    }
    FILE *logFile = fopen("activity_log.txt", "a");
if (logFile == NULL) {
    printf("Khong mo duoc file log\n");
    return 1;
}

    printf("He thong san sang voi %d cam bien\n", numSensors);
    while (1) {
		int i;
		time_t now = time(NULL);
        struct tm *t = localtime(&now);
        for (i = 0; i < numSensors; i++) {
            if (isSensorDisconnected(sensors[i])) {
                sprintf(msg,"[%02d:%02d:%02d] Sensor ID %d: Mat ket noi", t->tm_hour, t->tm_min, t->tm_sec, sensors[i].id);
                printf("%s\n",msg);
                logMessage(logFile,msg);
                recordDisconnect(&stats);
                continue;
            }

            if (TimetoSendData(&sensors[i])) {
                float value = receiveData(sensors[i].id,sensors[i].type);
                
                if (isBufferFull(&buffer)) {
                    recordOverflow(&stats);
                }
                pushBuffer(&buffer, value);        
	            float data;
		        if (popBuffer(&buffer, &data)) {
		        	sprintf(msg,"[%02d:%02d:%02d] Sensor ID %d (%s): %.2f ", t->tm_hour, t->tm_min, t->tm_sec, sensors[i].id, sensors[i].type, data);
		        	printf("%s\n",msg);
                	logMessage(logFile,msg);
		            if (!isValidData(data)) {
		                sprintf(msg,"Du lieu loi!");
		                printf("%s\n",msg);
		                recordError(&stats);
		            }
		            else {
		                recordValid(&stats);
		                if (checkThreshold(sensors[i], data)) {
		                    sprintf(msg,"[CANH BAO: Vuot nguong!]");
		                    printf("%s\n",msg);
                			logMessage(logFile,msg);
		                    recordExceed(&stats);
		                }
		                else {
                            sprintf(msg,"-> [OK]");
                            printf("%s\n",msg);
               				logMessage(logFile,msg);
                        }
		                float avg = filterData(&buffer);
		                if (avg != 0){
		                	sprintf(msg,"Trung binh truot: %.2f", avg);
		                	printf("%s\n",msg);
                			logMessage(logFile,msg);
						}
		       		}
		    	}
        	}
    	}
		static int loop = 0;
        loop++;
        if (loop % 20 == 0) {
            printReport(stats,logFile);
        }
		Sleep(1000);
	}
	fclose(logFile);
    return 0;
}
