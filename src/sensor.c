#define _CRT_SECURE_NO_WARNINGS
#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ERROR_VALUE      -9999.0f

/* Doc config tu file */
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
        char type_buf[20] = { 0 };
        char loc_buf[30] = { 0 };

        int thong_so = sscanf(line, "%d %19s %f %d %29s",
            &id, type_buf, &threshold, &period, loc_buf);

        if (thong_so != 5) continue;
        if (threshold < 0)  continue;
        if (period <= 0)    continue;

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

/* Sinh du lieu ngau nhien gia lap cam bien */
float receiveData(int sensorID, const char* type) {
    (void)sensorID;
    int r = rand() % 100;

    if (r == 0) return ERROR_VALUE;       
    
    if (strcmp(type, "temperature") == 0) {
        if (r == 1) return (float)(60 + rand() % 41);  /* 60..100 °C bat thuong */
        return (float)(15 + rand() % 26);              /* 15..40  °C binh thuong */
    }

    if (strcmp(type, "humidity") == 0) {
        if (r == 1) return (float)(95 + rand() % 6);   /* 95..100 % bat thuong  */
        return (float)(30 + rand() % 51);              /* 30..80  % binh thuong */
    }

    return ERROR_VALUE;
}

/* Kiem tra da den luc gui du lieu chua */
int TimetoSendData(Sensor* s) {
    if (s == NULL) return 0;

    time_t now = time(NULL);

    if ((now - s->lastTime) >= s->period) {
        s->lastTime = now;
        return 1;
    }

    return 0;
}

/* Kiem tra cam bien co bi mat ket noi khong */
int isSensorDisconnected(Sensor s) {
    if (s.id < 0) return 1;
    if ((rand() % 100) == 0) return 1;
    return 0;
}
