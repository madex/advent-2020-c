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
            int dep = (((earliesDep - 1) / line) + 1) * line; // -1 bus is comming at earliesDep
            int wait = dep - earliesDep;
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
    uint64_t depLine = (((dep - 1) / line) + 1) * line; // -1 bus is comming at earliesDep
    return depLine - dep;
}

uint64_t earliestTimestamp(int earliesDep, char* buf) {
    uint64_t lines[lineCount];
    int i = 0;
    while (buf != 0 && *buf != 0) {
        if (*buf == 'x' || *buf == 10) { // 10 for skipping newlines
            buf++; // no x at end
            lines[i++] = 0;
        } else {
            if (i < lineCount) {
                lines[i++] = strtol(buf, &buf, 0);
            }
        }
        if (*buf == ',') {
            buf++;
        }
    }
    uint64_t time = 0;
    while (1) {
        bool valid = true;
        if (time == 7903) {
            valid = true;
        }
        for (uint64_t i = 1; i < lineCount; i++) {
            if (lines[i] > 0) {
                if (waitTime(time, lines[i]) != i) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            return time;
        }
        time += lines[0];
    } while (1);
    return time;
}

int main(void) {
    FILE* f = fopen("input13.txt", "r");
    if (f != NULL) {
        char buf[512];
        int earliesDeparture;
        if (fgets(buf, sizeof buf, f) != NULL) {
            earliesDeparture = strtol(buf, 0, 0);
        }
        if (fgets(buf, sizeof buf, f) != NULL) {
            lineCount = 4;
            printf("1789,37,47,1889 = 1202161486= %llu\n", 
                earliestTimestamp(earliesDeparture, "1789,37,47,1889"));
            lineCount = 5;
            printf("67,7,x,59,61 = 1261476= %llu\n", 
                earliestTimestamp(earliesDeparture, "67,7,x,59,61\n"));
            printf("waittime * busline = %d\n", 
                waitMulLine(earliesDeparture, buf));
            printf("earliest timestall all busses = %llu\n", 
                earliestTimestamp(earliesDeparture, buf));
        }
        fclose(f);
    }
    return 0;
}
