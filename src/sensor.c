#define _CRT_SECURE_NO_WARNINGS
#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ERROR_VALUE -9999.0f

/* Đọc config từ file */
int readConfig(const char* filename, Sensor sensors[], int* numSensors) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Khong mo duoc file config!\n");
        return -1;
    }

    char line[256];
    int count = 0;

    while (count < MAX_SENSORS && fgets(line, sizeof(line), f) != NULL) {

        if (line[0] == '#' || line[0] == '\n') continue;

        int id, period;
        float threshold;
        char type_buf[20], loc_buf[30];

        int so_truong = sscanf(line, "%d %99s %f %d %9s",
            &id, type_buf, &threshold, &period, loc_buf);

        if (so_truong != 5) continue;

        if (strcmp(type_buf, "temperature") != 0 &&
            strcmp(type_buf, "humidity") != 0) continue;

        sensors[count].id = id;
        sensors[count].threshold = threshold;
        sensors[count].period = period;
        sensors[count].lastTime = 0;

        strcpy(sensors[count].type, type_buf);
        strcpy(sensors[count].location, loc_buf);

        count++;
    }

    fclose(f);
    *numSensors = count;
    return 0;
}

/* Sinh dữ liệu ngẫu nhiên giả lập cảm biến */
float receiveData(int sensorID, const char* type) {
    (void)sensorID;
    int r = rand() % 10;

    /* 1/10 trả về lỗi */
    if (r == 0) {
        return ERROR_VALUE;
    }

    if (strcmp(type, "temperature") == 0) {
        if (r == 1) return (float)(60 + rand() % 41);  /* 60..100 °C bất thường */
        return (float)(15 + rand() % 26);              /* 15..40 °C  bình thường */
    }

    if (strcmp(type, "humidity") == 0) {
        if (r == 1) return (float)(95 + rand() % 6);   /* 95..100 %  bất thường */
        return (float)(30 + rand() % 51);              /* 30..80 %   bình thường */
    }

    return ERROR_VALUE;
}

/* Kiểm tra đã đến lúc gửi dữ liệu chưa */
int TimetoSendData(Sensor* s) {
    if (s == NULL) return 0;

    time_t now = time(NULL);

    if ((now - s->lastTime) >= s->period) {
        s->lastTime = now;
        return 1;
    }

    return 0;
}

/* Kiểm tra cảm biến có bị mất kết nối không */
int isSensorDisconnected(Sensor s) {
    if (s.id < 0) return 1;
    return 0;
}
