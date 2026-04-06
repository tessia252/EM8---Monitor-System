#include "processing.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

#define ERROR_VALUE  -9999.0f
#define TEMP_MIN     -20.0f
#define TEMP_MAX      80.0f
#define HUM_MIN        0.0f
#define HUM_MAX      100.0f

/* Kiem tra gia tri co hop le khong, phan biet theo loai cam bien */
int isValidData(float value, const char* type) {
    if (value == ERROR_VALUE) return 0;

    if (strcmp(type, "temperature") == 0) {
        if (value < TEMP_MIN || value > TEMP_MAX) return 0;
    }
    else if (strcmp(type, "humidity") == 0) {
        if (value < HUM_MIN || value > HUM_MAX) return 0;
    }

    return 1;
}

/* Kiem tra co vuot nguong khong */
int checkThreshold(Sensor s, float value) {
    if (value > s.threshold) return 1;
    return 0;
}

/* Tinh trung binh truot tu buffer */
float filterData(Buffer* b) {
    if (b == NULL || b->count == 0) return 0.0f;

    float sum = 0.0f;
    int i;
    for (i = 0; i < b->count; i++) {
        int index = (b->head + i) % BUFFER_SIZE;
        sum += b->data[index];
    }
    return sum / (float)b->count;
}
