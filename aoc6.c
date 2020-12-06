#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool checkQuestions(char *line, uint32_t *mask, uint32_t *mask2) {
    uint32_t count = 0;
    uint32_t m = 0;
    while (*line >= 'a' && *line <= 'z') {
        uint32_t i = *line++ - 'a';
        m |= (1 << i);
        count++;
    }
    if (*mask == 0) {
        *mask2 = m;
    } 
    *mask |= m;
    if (count > 0) {
        *mask2 &= m;
    }
    return count == 0;
}

uint32_t count(uint32_t mask) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < 27; i++) {
        if (mask & 1) {
            count++;
        }
        mask >>= 1;
    }
    return count;
}

int main(void) {
    FILE* f = fopen("input6.txt", "r");
    if (f != NULL) {
        char buf[512];
        uint32_t trueCount1 = 0;
        uint32_t trueCount2 = 0;
        uint32_t mask1 = 0;
        uint32_t mask2 = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (checkQuestions(buf, &mask1, &mask2)) {
                trueCount1 += count(mask1);
                trueCount2 += count(mask2);
                mask1 = 0;
                mask2 = 0;
            }
        }
        trueCount1 += count(mask1);
        trueCount2 += count(mask2);
        printf("sum of anyone in a groups = %d\n", trueCount1);
        printf("sum of ereryone in a groups = %d\n", trueCount2);
        fclose(f);
    }
    return 0;
}
