#define _CRT_SECURE_NO_WARNINGS
#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ERROR_VALUE -9999.0f

/* đọc config */
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

        /* giới hạn độ dài chuỗi để tránh tràn */
        int thong_so_doc_duoc = sscanf(line, "%d %49s %f %d %49s",
            &id, type_buf, &threshold, &period, loc_buf);

        if (thong_so_doc_duoc != 5) {
            continue;
        }

        if (strcmp(type_buf, "temperature") != 0 && strcmp(type_buf, "humidity") != 0) {
            continue;
        }

        sensors[count].id = id;

        /* copy an toàn */
        strncpy(sensors[count].type, type_buf, sizeof(sensors[count].type) - 1);
        sensors[count].type[sizeof(sensors[count].type) - 1] = '\0';

        strncpy(sensors[count].location, loc_buf, sizeof(sensors[count].location) - 1);
        sensors[count].location[sizeof(sensors[count].location) - 1] = '\0';

        if (threshold > 0.0f) {
            sensors[count].threshold = threshold;
        }
        else {
            sensors[count].threshold = (strcmp(type_buf, "temperature") == 0) ? 50.0f : 80.0f;
        }

        sensors[count].period = (period >= 1) ? period : 1;
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
        int sign = (rand() % 2 == 0) ? 1 : -1;
        float gia_tri_bat_thuong = (float)(sign * (100 + rand() % 101));
        return gia_tri_bat_thuong;
    }

    /* phân bố đều 0.00..100.00 */
    float gia_tri_ngau_nhien = ((float)rand() / (float)RAND_MAX) * 100.0f;
    return gia_tri_ngau_nhien;
}

/* kiểm tra thời điểm gửi data */
int TimetoSendData(Sensor* s) {
    if (s == NULL) return 0;

    time_t now = time(NULL);

    if (s->lastTime == 0) {
        s->lastTime = now;
        return 1;
    }

    if (difftime(now, s->lastTime) >= (double)s->period) {
        s->lastTime = now;
        return 1;
    }

    return 0;
}

/* kiểm tra mất kết nối */
int isSensorDisconnected(Sensor s) {
    if (s.id < 0) return 1;
    if ((rand() % 10000) == 0) return 1;
    return 0;
}
