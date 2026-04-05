#define _CRT_SECURE_NO_WARNINGS
#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ERROR_VALUE -9999.0f

/* đọc config*/
int readConfig(const char* filename, Sensor sensors[], int* numSensors) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        return -1; 
    }

    char line[256];
    int count = 0;

    while (count < MAX_SENSORS && fgets(line, sizeof(line), f) != NULL) {

        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        int id, period;
        float threshold;
        char type_buf[50] = { 0 }, loc_buf[50] = { 0 };

        int thong_so_doc_duoc = sscanf(line, "%d %s %f %d %s", &id, type_buf, &threshold, &period, loc_buf);

        if (thong_so_doc_duoc != 5) {
            continue;
        }

        if (strcmp(type_buf, "temperature") != 0 && strcmp(type_buf, "humidity") != 0) {
            continue;
        }

        sensors[count].id = id;
        strcpy(sensors[count].type, type_buf);
        strcpy(sensors[count].location, loc_buf);

        if (threshold > 0.0) {
            sensors[count].threshold = threshold;
        }
        else {
            if (strcmp(type_buf, "temperature") == 0) {
                sensors[count].threshold = 50.0f;
            }
            else {
                sensors[count].threshold = 80.0f;
            }
        }

        if (period >= 1) {
            sensors[count].period = period;
        }
        else {
            sensors[count].period = 1;
        }

        sensors[count].lastTime = 0;

        count++;
    }

    fclose(f);
    *numSensors = count; 
    return 0;
}

/* dữ liệu ngẫu nhiên */
float receiveData(int sensorID) {
    (void)sensorID;
    int r = rand() % 1000;

    if (r < 5) {
        return ERROR_VALUE;
    }

    if (r < 20) {
        int sign;
        if (rand() % 2 == 0) {
            sign = 1;   
        }
        else {
            sign = -1; 
        }

        float gia_tri_bat_thuong = (float)(sign * (100 + rand() % 101));
        return gia_tri_bat_thuong;
    }

    // Chuyển rand() về float trong dải [0.00, 100.00]
    float gia_tri_ngau_nhien = ((float)rand() / (float)RAND_MAX) * 100.0f;
    return gia_tri_ngau_nhien;
}

/* kiểm tra thời điểm gửi data*/
int TimetoSendData(Sensor* s) {
    if (s == NULL) return 0;

    time_t now = time(NULL);

    // Lần đầu khởi động máy
    if (s->lastTime == 0) {
        s->lastTime = now;
        return 1;
    }

    // Kiểm tra đã đủ period
    if (difftime(now, s->lastTime) >= (double)s->period) {
        s->lastTime = now;
        return 1;
    }

    return 0;
}

/* kiểm tra mất kết nối*/
int isSensorDisconnected(Sensor s) {
    if (s.id < 0) return 1;
    if ((rand() % 10000) == 0) return 1; 
    return 0;
}