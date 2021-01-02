#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

int countLoops(int subject) {
    return 0;
}

uint64_t loopFromPk(uint64_t pk, uint64_t sub) {
    uint64_t erg = 1;
    uint64_t loops = 0;
    while (erg != pk) {
        erg *= sub;
        erg = erg % 20201227;
        loops++;
    }
    return loops;
}

uint64_t encyptionKey(uint64_t sub, uint64_t loopSize) {
    uint64_t erg = 1;
    for (int i = 0; i < loopSize; i++) {
        erg *= sub;
        erg = erg % 20201227;
    }
    return erg;
}

int main(void) {
    // Test
    // printf("loopsize=8 == %llu? \n",  loopFromPk(5764801, 7));
    // printf("loopsize=11 == %llu? \n", loopFromPk(17807724, 7));
    // printf("14897079 == %llu? \n", encyptionKey(17807724, 8));
    // printf("14897079 == %llu? \n", encyptionKey(5764801, 11));
    FILE* f = fopen("input25.txt", "r");
    if (f != NULL) {
        char buf[100];
        fgets(buf, sizeof buf, f);
        uint64_t pkCard = strtol(buf, 0, 0);
        fgets(buf, sizeof buf, f);
        uint64_t pkDoor = strtol(buf, 0, 0);
        fclose(f);
        printf("Part 1: %llu\n",  encyptionKey(pkDoor, loopFromPk(pkCard, 7)));
    }
    return 0;
}
