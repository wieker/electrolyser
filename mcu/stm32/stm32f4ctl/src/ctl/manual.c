#include <stdint.h>
#include "defs.h"
#include <stdio.h>

int32_t startTime;
int32_t chState = 0;


void parse_ctl() {

        char ch = getchar();
        if (0xff != ch) {
            printf("%c", ch);
        }
        switch (ch) {
            case '=': {
                throttle += 50;
                //chState = 1;
                break;
            }
            case '-': {
                throttle -= 10;
                break;
            }
            case '0': {
                stopMotors();
                chState = 0;
                break;
            }
            case 'a': {
                desiredX --;
                break;
            }
            case 's': {
                desiredY --;
                break;
            }
            case 'd': {
                desiredX ++;
                break;
            }
            case 'w': {
                desiredY ++;
                break;
            }
            case '1': {
                cMode = 0;
                break;
            }
            case '2': {
                cMode = 1;
                break;
            }
            case '3': {
                cMode = 2;
                break;
            }
            case '4': {
                cMode = 3;
                break;
            }
            case '5': {
                cMode = 4;
                break;
            }
            case '6': {
                cMode = 5;
                break;
            }
            case 'g': {
                calibratingG = CALIBRATING_GYRO_CYCLES;
                break;
            }
            case 'v': {
                throttle = 0;
                break;
            }
            case 'm': {
                throttle = 300;
                chState = 1;
                startTime = millis();
                break;
            }
        }
}

void logic() {
    if ((chState == 1) && (millis() - startTime > 300)) {
        throttle = 350;
        chState ++;
    }
    if ((chState == 2) && (millis() - startTime > 600)) {
        throttle = 400;
        chState ++;
    }
    if ((chState == 3) && (millis() - startTime > 2600)) {
        throttle = 350;
        chState ++;
    }
    if ((chState == 4) && (millis() - startTime > 3600)) {
        throttle = 300;
        chState ++;
    }
    if (millis() - startTime > 5000) {
        stopMotors();
        chState = 0;
    }
}

void tlmtr() {
        uint32_t ctime = millis();
        static uint32_t ptime = 0;
        if (ctime - ptime > 1000) {
            printf("[%7d]\n", ctime);
            printf("acADC %5d %5d %5d\n", accADC[0], accADC[1], accADC[2]);
            printf("grADC %5d %5d %5d\n", gyroADC[0], gyroADC[1], gyroADC[2]);
            printf("throttle %d\n", throttle);
            ptime = ctime;
        }
}

void manual() {
    parse_ctl();
    logic();
    tlmtr();
}
