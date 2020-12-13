#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int lineCount = 0;

int waitMulLine(int earliesDep, char* buf) { 
    int lineMin = 0, waitMin;
    while (buf != 0 && *buf != 0) {
        if (*buf == 'x' || *buf == 10) { // 10 for skipping newlines
            buf++; // no x at end
        } else {
            int line = strtol(buf, &buf, 0);
            int wait = line - (earliesDep % line);
            if (lineMin == 0 || wait < waitMin) {
                lineMin = line;
                waitMin = wait;
            }
        }
        lineCount++;
        if (*buf == ',') {
            buf++;
        }
    }
    return lineMin * waitMin;
}

uint64_t waitTime(uint64_t dep, uint64_t line) {
    return line - (dep % line);
}

uint64_t earliestTimestamp(int earliesDep, char* buf) {
    uint64_t lines[lineCount]; // double parse because of dynamic length
    int i = 0;
    while (buf != 0 && *buf != 0) {
        if (*buf == 'x') { // 10 for skipping newlines
            buf++; // no x at end
            lines[i++] = 0;
        } else {
            if (i < lineCount) {
                lines[i++] = strtol(buf, &buf, 0);
            }
        }
        if (*buf == ',' || *buf == 10) {
            buf++;
        }
    }
    uint64_t time = 0;
    uint64_t addTime = lines[0];
    //uint64_t last = 0;
    int lcmCalced = 0;
    while (1) {
        bool valid = true;
        for (uint64_t i = 1; i < lineCount; i++) {
            if (lines[i] > 0) {
                if (waitTime(time, lines[i]) != i % lines[i]) { // % is important
                    valid = false;
                    break;
                } else {
                    if (i > lcmCalced) {
                        lcmCalced = i;
                        addTime *= lines[i];
                        //printf("%llu %llu %llu\n", time, lines[i], time-last);
                    }
                }
            }
        }
        if (valid) {
            return time;
        }
        time += addTime;
    } while (1);
    return time;
}

int main(void) {
    FILE* f = fopen("input13.txt", "r");
    if (f != NULL) {
        char buf[256];
        int earliesDeparture;
        if (fgets(buf, sizeof buf, f) != NULL) {
            earliesDeparture = strtol(buf, 0, 0);
        }
        if (fgets(buf, sizeof buf, f) != NULL) {
            printf("waittime * busline = %d\n", 
                waitMulLine(earliesDeparture, buf));
            printf("earliest timestall for all busses = %llu\n", 
                earliestTimestamp(earliesDeparture, buf));
        }
        fclose(f);
    }
    return 0;
}
