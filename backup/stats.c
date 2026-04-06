#include "stats.h"
#include "common.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LOG_FILE "log.txt"

/* Ghi 1 dong log vao file */
void writeLog(const char* message) {
    FILE* f = fopen(LOG_FILE, "a");
    if (f == NULL) return;

    time_t now = time(NULL);
    char* t = ctime(&now);
    if (t == NULL) { fclose(f); return; }
    t[24] = '\0';

    fprintf(f, "[%s] %s\n", t, message);
    fclose(f);
}

/* Khoi tao tat ca ve 0 */
void initStats(Stats* s) {
    s->validCount = 0;
    s->errorCount = 0;
    s->exceedCount = 0;
    s->bufferOverflowCount = 0;
    s->disconnectCount = 0;

    s->tempMin = 9999.0f;
    s->tempMax = -9999.0f;
    s->tempSum = 0.0f;
    s->tempCount = 0;

    s->humMin = 9999.0f;
    s->humMax = -9999.0f;
    s->humSum = 0.0f;
    s->humCount = 0;

    s->maxErrorSensorID = -1;
    s->maxErrorSensorCount = 0;

    int i;
    for (i = 0; i < MAX_SENSORS; i++) {
        s->sensorErrorCount[i] = 0;
    }
}

/* Cap nhat min / max / tong
   type = "temperature" : nhiet do
   type = "humidity"    : do am   */
void updateStats(Stats* s, float value, const char* type) {
    if (strcmp(type, "temperature") == 0) {
        if (value < s->tempMin) s->tempMin = value;
        if (value > s->tempMax) s->tempMax = value;
        s->tempSum += value;
        s->tempCount++;
    }
    else if (strcmp(type, "humidity") == 0) {
        if (value < s->humMin) s->humMin = value;
        if (value > s->humMax) s->humMax = value;
        s->humSum += value;
        s->humCount++;
    }
}

void recordValid(Stats* s) {
    s->validCount++;
}

void recordExceed(Stats* s) {
    s->exceedCount++;
    writeLog("CANH BAO: Gia tri vuot nguong!");
}

void recordDisconnect(Stats* s) {
    s->disconnectCount++;
    writeLog("LOI: Cam bien mat ket noi!");
}

/* sensorID: ID cua cam bien bi loi */
void recordError(Stats* s, int sensorID) {
    s->errorCount++;
    writeLog("LOI: Du lieu khong hop le!");

    if (sensorID >= 0 && sensorID < MAX_SENSORS) {
        s->sensorErrorCount[sensorID]++;
        if (s->sensorErrorCount[sensorID] > s->maxErrorSensorCount) {
            s->maxErrorSensorCount = s->sensorErrorCount[sensorID];
            s->maxErrorSensorID = sensorID;
        }
    }
}

void recordOverflow(Stats* s) {
    s->bufferOverflowCount++;
    writeLog("LOI: Tran bo dem - ghi de du lieu cu!");
}

/* In bao cao tong ket */
void printReport(Stats s) {
    printf("\n========== BAO CAO TONG KET ==========\n");
    printf("So ban tin hop le  : %d\n", s.validCount);
    printf("So ban tin loi     : %d\n", s.errorCount);
    printf("So lan vuot nguong : %d\n", s.exceedCount);
    printf("So lan tran bo dem : %d\n", s.bufferOverflowCount);
    printf("So lan mat ket noi : %d\n", s.disconnectCount);

    printf("\n--- Nhiet do ---\n");
    if (s.tempCount > 0) {
        printf("  Lon nhat  : %.2f\n", s.tempMax);
        printf("  Nho nhat  : %.2f\n", s.tempMin);
        printf("  Trung binh: %.2f\n", s.tempSum / s.tempCount);
    }
    else {
        printf("  Chua co du lieu\n");
    }

    printf("\n--- Do am ---\n");
    if (s.humCount > 0) {
        printf("  Lon nhat  : %.2f\n", s.humMax);
        printf("  Nho nhat  : %.2f\n", s.humMin);
        printf("  Trung binh: %.2f\n", s.humSum / s.humCount);
    }
    else {
        printf("  Chua co du lieu\n");
    }

    if (s.maxErrorSensorID >= 0) {
        printf("\nThiet bi loi nhieu nhat: Sensor %d (%d lan)\n",
            s.maxErrorSensorID, s.maxErrorSensorCount);
    }

    printf("======================================\n");
}
