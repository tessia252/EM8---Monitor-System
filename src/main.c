#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sensor.h"
#include "buffer.h"
#include "processing.h"
#include "stats.h"

int main() {

    // kh?i t?o random
    srand(time(NULL));

    // khai báo
    Sensor sensors[MAX_SENSORS];
    int numSensors = 0;

    Buffer buffer; 
    Stats stats;

    // kh?i t?o
    initBuffer(&buffer);
    initStats(&stats);

    // ð?c config
    if (readConfig("config.txt", sensors, &numSensors) != 0) {
        printf("Khong doc duoc file config\n");
        return 1;
    }

    printf("Da doc %d sensors\n", numSensors);

    // v?ng l?p chính
    while (1) {

        // 1. sensor g?i d? li?u
        for (int i = 0; i < numSensors; i++) {

            if (isSensorDisconnected(sensors[i])) {

                printf("Sensor %d bi mat ket noi\n",
                       sensors[i].id);

                recordDisconnect(&stats);

                continue;
            }

            if (TimetoSendData(&sensors[i])) {

                float value =
                    receiveData(sensors[i].id);

                // ki?m tra buffer ð?y
                if (isBufferFull(&buffer)) {

                    recordOverflow(&stats);

                }

                pushBuffer(&buffer, value);

                printf("Nhan data tu sensor %d: %.2f\n",
                       sensors[i].id,
                       value);
            }
        }

        // 2. x? l? d? li?u t? buffer
        float data;

        if (popBuffer(&buffer, &data)) {

            if (!isValidData(data)) {

                printf("Data loi: %.2f\n", data);

                recordError(&stats);

            }
            else {

                recordValid(&stats);

                // ki?m tra vý?t ngý?ng
                if (checkThreshold(sensors[0], data)) {

                    printf("Vuot nguong!\n");

                    recordExceed(&stats);
                }

                float avg =
                    filterData(&buffer);

                printf("Gia tri trung binh: %.2f\n",
                       avg);
            }
        }

        // in report m?i 20 v?ng
        static int loop = 0;

        loop++;

        if (loop % 20 == 0) {

            printReport(stats);

        }
		Sleep(3000);
    }

    return 0;
}
