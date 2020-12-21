#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

struct tile {
    int  id;
    char data[10][10];     // all tiles have 10*10
    uint16_t  sideMask[8]; // top richt bottom left and all mirroed again
    struct tile *link[4];
};

typedef struct tile tile_t;

int tileSides[1024];
tile_t tile[150];
int tileCount;

int64_t calcSides(void) {
    for (int i = 0; i < tileCount; i++) {
        tile_t *t = &tile[i];
        for (int j = 0; j < 10; j++) {
            int k = 9 - j;
            uint16_t bitMask = (1 << j);
            t->sideMask[0] |= (t->data[0][j] == '#' ? bitMask : 0);
            t->sideMask[1] |= (t->data[j][9] == '#' ? bitMask : 0);
            t->sideMask[2] |= (t->data[9][k] == '#' ? bitMask : 0);
            t->sideMask[3] |= (t->data[k][0] == '#' ? bitMask : 0);
            t->sideMask[4] |= (t->data[0][k] == '#' ? bitMask : 0);
            t->sideMask[5] |= (t->data[k][9] == '#' ? bitMask : 0);
            t->sideMask[6] |= (t->data[9][j] == '#' ? bitMask : 0);
            t->sideMask[7] |= (t->data[j][0] == '#' ? bitMask : 0); 
        }
        for (int l = 0; l < 8; l++) {
            tileSides[t->sideMask[l]]++;
        }
    }
    // search corner tiles
    int64_t cornerProduct = 1;
    for (int i = 0; i < tileCount; i++) {
        tile_t *t = &tile[i];
        int sides = 0;
        for (int j = 0; j < 4; j++) {
            if (tileSides[t->sideMask[j]] == 1) {
                sides++;
            }
        }
        if (sides == 2) {
            cornerProduct *= t->id;
            printf("%d  index=%d\n", t->id, i);
        }
    }
    return cornerProduct;
}

int main(void) {
    FILE* f = fopen("input20.txt", "r");
    if (f != NULL) {
        int  sideLen = 0;
        char buf[200];
        int part = 0;
        int i = 0;
        tileCount = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            tile_t *t = &tile[tileCount];
            switch (part) {
                case 0:
                    if (strncmp(buf, "Tile ", 5) == 0) {
                        t->id = strtol(&buf[5], 0, 0);
                        part = 1;
                        i = 0;
                    }
                    break;
                case 1:
                    if (*buf == 0 || *buf == 10) {
                        part = 0;
                        tileCount++;
                    } else {
                        memcpy(&t->data[i++][0], buf, 10);
                    }
                    break;
            }
        }
        if (part == 1) {
            tileCount++;
        }
        fclose(f);
        printf("part 1: corner product %lld\n", calcSides());
        // printf("part 2: count activate 4D %d\n", calculateNext4D(buf, sideLen, 1, 6));
    }
    return 0;
}
