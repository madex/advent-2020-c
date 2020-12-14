#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint64_t executeMask(char *mask, uint64_t value) { 
    int len = strlen(mask);
    if (len < 36) 
        return 0;
    for (int bit = 35; bit >= 0; bit--) {
        switch (*mask++) {
            case '0': value &= ~(1ULL << bit); break;
            case '1': value |=  (1ULL << bit); break;
        } 
    }
    return value;
}

uint64_t sum(uint64_t *mem, size_t size) {
    uint64_t sum = 0;
    for (size_t i; i < size; i++) {
        sum += mem[i];
    }
    return sum;
}

uint64_t mem[66000];

int main(void) {
    FILE* f = fopen("input14.txt", "r");
    if (f != NULL) {
        char buf[256];
        char mask[37] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        int earliesDeparture;
        size_t highestAdr = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (strncmp(buf, "mask = ", 7) == 0) {
                strncpy(mask, &buf[7], 36);
            } else if (strncmp(buf, "mem[", 4) == 0) {
                char *b = &buf[4];
                size_t adr = strtol(b, &b, 0);
                b += 4; // skip "] = "
                uint64_t value = strtol(b, 0, 0);
                mem[adr] = executeMask(mask, value);
                if (adr > highestAdr) {
                    highestAdr = adr;
                }
            }
        }
        printf("sum of all values = %llu\n", sum(mem, highestAdr + 1));
        fclose(f);
    }
    return 0;
}
