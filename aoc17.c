#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    int x, y, z;
} dir_t;

typedef struct {
    int x ,y, z, w;
} dir4_t;

dir4_t dir4d[80];

dir_t dir[] = {
    {-1, -1, -1}, { 0, -1, -1}, { 1, -1, -1},
    {-1, -1,  0}, { 0, -1,  0}, { 1, -1,  0},
    {-1, -1,  1}, { 0, -1,  1}, { 1, -1,  1},
    {-1,  0, -1}, { 0,  0, -1}, { 1,  0, -1},
    {-1,  0,  0},               { 1,  0,  0}, 
    {-1,  0,  1}, { 0,  0,  1}, { 1,  0,  1},
    {-1,  1, -1}, { 0,  1, -1}, { 1,  1, -1},
    {-1,  1,  0}, { 0,  1,  0}, { 1,  1,  0},
    {-1,  1,  1}, { 0,  1,  1}, { 1,  1,  1}
};

int calculateNext(char *buf, int sideLen, int z, int level) {
    int len  = sideLen + 2;
    int off  =  1;
    int offZ = (z == 1 ? sideLen / 2 : 1);
    char cube[len*len*len];
    char *c = cube;
    int countHash = 0;
    for (int nz = 0; nz < len; nz++) {
        for (int ny = 0; ny < len; ny++) {
            for (int nx = 0; nx < len; nx++) {
                int n = 0;
                for (int i = 0; i < sizeof dir / sizeof dir[0]; i++) {
                   dir_t *d = &dir[i];
                   uint16_t px = nx - off + d->x;
                   uint16_t py = ny - off + d->y;
                   uint16_t pz = nz - offZ + d->z;
                   if (px < sideLen && py < sideLen && pz < z && 
                       buf[pz*sideLen*sideLen + py*sideLen + px] == '#')
                    {
                        n++;
                    } 
                }
                uint16_t px = nx - off, py = ny - off, pz = nz - offZ;
                bool wasSet = (px < sideLen && py < sideLen && pz < z && 
                               buf[pz*sideLen*sideLen + py*sideLen + px] == '#');
                *c = n == 3 ? '#' : wasSet && n == 2 ? '#' : '.';
                if (*c++ == '#') {
                    countHash++;
                }
            }
        }
    }
    return level > 1 ? calculateNext(cube, len, len, level - 1) : countHash;
}

int calculateNext4D(char *buf, int sideLen, int z, int level) {
    int len  = sideLen + 2;
    int off  =  1;
    int offZ = (z == 1 ? sideLen / 2 : 1);
    char cube[len*len*len*len];
    char *c = cube;
    int countHash = 0;
    for (int nw = 0; nw < len; nw++) {
        for (int nz = 0; nz < len; nz++) {
            for (int ny = 0; ny < len; ny++) {
                for (int nx = 0; nx < len; nx++) {
                    int n = 0;
                    for (int i = 0; i < sizeof dir4d / sizeof dir4d[0]; i++) {
                        dir4_t *d = &dir4d[i];
                        uint16_t px = nx - off + d->x;
                        uint16_t py = ny - off + d->y;
                        uint16_t pz = nz - offZ + d->z;
                        uint16_t pw = nw - offZ + d->w;
                        if (px < sideLen && py < sideLen && pz < z && pw < z &&
                            buf[pw*sideLen*sideLen*sideLen + pz*sideLen*sideLen + py*sideLen + px] == '#')
                        {
                            n++;
                        } 
                    }
                    uint16_t px = nx - off, py = ny - off, pz = nz - offZ, pw = nw - offZ;
                    bool wasSet = (px < sideLen && py < sideLen && pz < z && pw < z &&
                                buf[pw*sideLen*sideLen*sideLen + pz*sideLen*sideLen + py*sideLen + px] == '#');
                    *c = n == 3 ? '#' : wasSet && n == 2 ? '#' : '.';
                    if (*c++ == '#') {
                        countHash++;
                    }
                }
            }
        }
    }
    return level > 1 ? calculateNext4D(cube, len, len, level - 1) : countHash;
}

void calcDir() {
    dir4_t *d = dir4d;
    for (int w = -1; w < 2; w++) {
        for (int z = -1; z < 2; z++) {
            for (int y = -1; y < 2; y++) {
                for (int x = -1; x < 2; x++) {
                    if (x == 0 && y == 0 && z ==0 && w == 0) {
                        // own
                    } else {
                        d->w = w;
                        d->z = z;
                        d->y = y;
                        d->x = x;
                        d++;
                    }
                }
            }
        }
    }
}

int main(void) {
    FILE* f = fopen("input17.txt", "r");
    if (f != NULL) {
        int  sideLen = 0;
        char buf[200];  
        char *b = buf;
        while (fgets(b, sizeof buf - (b - buf), f) != NULL) {
            if (sideLen == 0) {
                sideLen = strlen(b);
                if (b[sideLen - 1] == 10) {
                    sideLen--;
                }
            }
            b += sideLen;
        }
        fclose(f);
        calcDir();
        printf("part 1: count activate %d\n", calculateNext(buf, sideLen, 1, 6));
        printf("part 2: count activate 4D %d\n", calculateNext4D(buf, sideLen, 1, 6));
    }
    return 0;
}
