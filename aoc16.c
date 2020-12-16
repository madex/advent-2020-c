#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char name[40];
    int min, max;
} range_t;

typedef struct {
    int fields[50];
    int fieldsCnt;
} ticket_t;

range_t ranges[100];
int rangeCnt = 0;
ticket_t ticket[300]; // 0 ist my
int ticketCnt = 0;

int sumInvalid(void) {
    int sum = 0;
    for (int i = 1; i < ticketCnt; i++) {
        ticket_t *t = &ticket[i];
        for (int j = 0; j < t->fieldsCnt; j++) {
            int value = t->fields[j];
            bool valid = false;
            for (int k = 0; k < rangeCnt; k++) {
                range_t *r = &ranges[k];
                if (value >= r->min && value <= r->max) {
                    valid = true;
                    break;
                }
            }
            if (!valid) {
                sum += value;
            }
        }
    }
    return sum;
}

int main(void) {
    FILE* f = fopen("input16.txt", "r");
    if (f != NULL) {
        char buf[256];
        int part = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (*buf != 0 && *buf != 10) {
                char *b = buf;
                switch (part){
                    case 0: // read rule
                        b = strstr(b, ": ");
                        if (b == NULL) break;
                        strncpy(ranges[rangeCnt].name, buf, b - buf);
                        strncpy(ranges[rangeCnt + 1].name, buf, b - buf);
                        b += 2; // ": "
                        ranges[rangeCnt].min = strtol(b, &b, 0);
                        b++; // "-"
                        ranges[rangeCnt].max = strtol(b, &b, 0);
                        b += 4; // " or "
                        ranges[rangeCnt + 1].min = strtol(b, &b, 0);
                        b++; // "-"
                        ranges[rangeCnt + 1].max = strtol(b, &b, 0);
                        rangeCnt += 2;
                        break;

                    case 1: 
                    case 2: 
                        if (isdigit(*b)) {
                            ticket_t *t = &ticket[ticketCnt++];
                            while (*b != 0) {
                                t->fields[t->fieldsCnt++] = strtol(b, &b, 0);
                                if (*b == ',' || *b == 10) {
                                    b++;
                                }
                            }
                        }
                        break;

                    default:
                        break;
                }
            } else {
                part++;
            }
        }
        printf("part 1 sum of all invalid  %d\n ", sumInvalid());
        fclose(f);
    }
    return 0;
}
