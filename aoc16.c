#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char name[40];
    int min1, max1;
    int min2, max2;
    int map;
} range_t;

typedef struct {
    int fields[50];
    bool valid;
    int fieldsCnt;
} ticket_t;

range_t ranges[100];
int rangeCnt = 0;
ticket_t ticket[300]; // 0 ist my
int ticketCnt = 0;

int getSetBit(int in) {
    int cnt = 0;
    int i = in;
    while ((i & 1) == 0) {
        cnt++;
        i >>= 1;
    }
    return cnt;
}

int sumInvalid(void) {
    int sum = 0;
    for (int i = 1; i < ticketCnt; i++) {
        ticket_t *t = &ticket[i];
        t->valid = true;
        for (int j = 0; j < t->fieldsCnt; j++) {
            int value = t->fields[j];
            bool valid = false;
            for (int k = 0; k < rangeCnt; k++) {
                range_t *r = &ranges[k];
                if ((value >= r->min1 && value <= r->max1) ||
                    (value >= r->min2 && value <= r->max2)) {
                    valid = true;
                    break;
                }
            }
            if (!valid) {
                sum += value;
                t->valid = false;
            }
        }
    }
    return sum;
}

uint64_t departureProduct(void) {
    int fieldCnd = ticket[0].fieldsCnt;
    ticket[0].valid = true;
    for (int k = 0; k < rangeCnt; k++) {
        range_t *r = &ranges[k];
        r->map = 0;
        for (int j = 0; j < ticket[0].fieldsCnt; j++) {
            bool valid = true;
            for (int i = 0; i < ticketCnt; i++) {
                ticket_t *t = &ticket[i];
                if (t->valid) {
                    int value = t->fields[j];
                    if ((value >= r->min1 && value <= r->max1) ||
                        (value >= r->min2 && value <= r->max2)) {
                    } else {
                        valid = false;
                    }
                }
            }
            if (valid) {
                r->map |= (1 << j);
            }
        }
    }
    // try if bit is only set to one rule
    for (int i = 0; i < ticket[0].fieldsCnt; i++) {
        for (int j = 0; j < ticket[0].fieldsCnt; j++) {
            int bitCount = 0, id = 0;
            for (int k = 0; k < rangeCnt; k++) {
                range_t *r = &ranges[k];
                if (r->map & (1 << j)) {
                    bitCount++;
                    id = k;
                }
            }
            if (bitCount == 1) {
                ranges[id].map = (1 << j);
            }
        }
    }
    uint64_t product = 1;
    for (int k = 0; k < rangeCnt; k++) {
        range_t *r = &ranges[k];
        r->map = getSetBit(r->map);
        if (strncmp(r->name, "departure", 9) == 0) {
            product *= (uint64_t) ticket[0].fields[r->map];
        }
    }
    return product;
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
                        b += 2; // ": "
                        ranges[rangeCnt].min1 = strtol(b, &b, 0);
                        b++; // "-"
                        ranges[rangeCnt].max1 = strtol(b, &b, 0);
                        b += 4; // " or "
                        ranges[rangeCnt].min2 = strtol(b, &b, 0);
                        b++; // "-"
                        ranges[rangeCnt].max2 = strtol(b, &b, 0);
                        rangeCnt++;
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
                }
            } else {
                part++;
            }
        }
        printf("part 1 sum of all invalid  %d\n", sumInvalid());
        printf("part 2 product of all departures %llu\n", departureProduct());
        fclose(f);
    }
    return 0;
}
