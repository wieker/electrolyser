#include <stdint.h>
#include "defs.h"
#include <stdio.h>

int32_t startTime;
int32_t chState = 0;
int32_t thTime = 0;

void big_switch(char ch) {
    switch (ch) {
        case '=': {
            throttle += 50;
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
            throttle = -10;
            chState = 1;
            startTime = millis();
            break;
        }
    }
}

void read_throttle() {
    int ctime = millis();
    int nt = 0;
    while (millis() - ctime < 1000) {
        char ch = getchar();
        if (ch >= 0 && ch <= 100) {
            throttle = ch * 10;
            thTime = millis();
            return;
        }
    }
    stopMotors();
}

void parse_ctl() {

        char ch = getchar();
        if (0xFF != ch) {
            printf("%c", ch);
        }
        if ('t' == ch) {
            read_throttle();
        }
        big_switch(ch);
}

void logic() {
    if (millis() - startTime > 30000) {
        stopMotors();
        chState = 0;
    }
    if (millis() - thTime > 1000) {
        stopMotors();
        chState = 0;
    }
    if (chState == 0) {
        stopMotors();
    }
}

void tlmtr() {
        uint32_t ctime = millis();
        static uint32_t ptime = 0;
        if (ctime - ptime > 1000) {
            //printf("[%3d]\n", ctime / 1000);
            printf("angle %d %d\n", angle[0], angle[1]);
            //printf("gyr %5d %5d %5d\n", gyroADC[0], gyroADC[1], gyroADC[2]);
            printf("throttle %d\n", throttle);
            ptime = ctime;
        }
}

void manual() {
    parse_ctl();
    logic();
    tlmtr();
}
