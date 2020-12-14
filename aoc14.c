#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t adr;
    uint64_t value;
} mem_list_t;

typedef struct {
    size_t memPos;
    mem_list_t mem[100000];
} mem_t;

mem_t mem1;
mem_t mem2;

uint64_t sum(mem_t *mem) {
    uint64_t sum = 0;
    for (size_t i = 0; i < mem->memPos; i++) {
        sum += mem->mem[i].value;
    }
    return sum;
}

int findAdr(mem_t *mem, uint64_t adr) {
    for (int i = 0; i < mem->memPos; i++) { // could be faster with sorted list
        if (mem->mem[i].adr == adr) {
            return i;
        }
    }
    if (mem->memPos >= sizeof mem->mem / sizeof mem->mem[0]) {
        printf("memmory full");
        exit(1);
    }
    mem->mem[mem->memPos].adr = adr;
    return mem->memPos++;
}

uint64_t executeValueMask(char *mask, uint64_t value) { 
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

void writeFloating(uint64_t xmask, uint64_t adr, uint64_t value) {
    for (int bit = 35; bit >= 0; bit--) {
        uint64_t bitMask = (1ULL << bit);
        if ((xmask & bitMask) != 0) {
            xmask &= ~bitMask;
            uint64_t adr0 = adr & ~bitMask; // adr with mask bit 0
            uint64_t adr1 = adr | bitMask;  // adr with mask bit 1
            if (xmask == 0) { // end
                mem2.mem[findAdr(&mem2, adr0)].value = value;
                mem2.mem[findAdr(&mem2, adr1)].value = value;
            } else {
                writeFloating(xmask, adr0, value);
                writeFloating(xmask, adr1, value);
            }
            return;
        }
    }
}

void executeAdressMask(char *mask, uint64_t adr, uint64_t value) {
    int len = strlen(mask);
    if (len < 36) 
        return;
    uint64_t xmask = 0;
    for (int bit = 35; bit >= 0; bit--) {
        switch (*mask++) {
            case '1': adr   |=  (1ULL << bit); break;
            case 'X': xmask |=  (1ULL << bit); break;
        } 
    }
    writeFloating(xmask, adr, value);
}

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
                // part1
                mem1.mem[findAdr(&mem1, adr)].value = executeValueMask(mask, value);
                // part2
                executeAdressMask(mask, adr, value);
            }
        }
        fclose(f);
        printf("sum of all values V1 = %llu (%zu values)\n", sum(&mem1), mem1.memPos);
        printf("sum of all values V2 = %llu (%zu values)\n", sum(&mem2), mem2.memPos);
    }
    return 0;
}
