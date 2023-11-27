#include <stdint.h>
#include "../ctl/defs.h"
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
        case 'c': {
            thTime = millis();
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
            return;
        }
    }
    stopMotors();
}

int8_t cmd_string[10];
int wpos = 0;

int check_string(char ch) {
    return ch == 'e';
}

void parse_ctl() {
    for (;;) {
        int8_t ch = getchar();
        if (-1 == ch) {
            return;
        }
        if (wpos == 0 && ch == 't') {
            wpos = 1;
        } else if (wpos == 1 && ch == 'q') {
            wpos = 2;
        } else if (wpos >= 2 && wpos < 8) {
            cmd_string[wpos] = ch;
            wpos ++;
        } else if (wpos == 8 && check_string(ch)) {
            wpos = 0;
            big_switch(cmd_string[2]);
            int8_t u = cmd_string[3];
            if (u >= 0 && u <= 100) {
                throttle = u * 10;
            }
            if (cmd_string[4] < 0) {
                cmd_string[4] ++;
            }
            if (cmd_string[5] < 0) {
                cmd_string[5] ++;
            }
            desiredX = cmd_string[4];
            desiredY = cmd_string[5];
        } else {
            wpos = 0;
        }
    }
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
            //printf("angle %d %d\n", angle[0], angle[1]);
            //printf("des %d %d\n", desiredX, desiredY);
            printf("up %d\n", millis() / 1000);
            //printf("gyr %5d %5d %5d\n", gyroADC[0], gyroADC[1], gyroADC[2]);
            printf("th %d %d=>%d %d=>%d\n", throttle, desiredX, angle[0], desiredY, angle[1]);
            ptime = ctime;
        }
}

void manual() {
    parse_ctl();
    logic();
    tlmtr();
}
