#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    int x, y, z;
} dir_t;

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
                // (wasSet ? (n >= 2 && n <= 3 ? '#':'.') : (n == 3 ? '#' : '.'));
                if (*c++ == '#') {
                    countHash++;
                }
            }
        }
    }
    return level > 1 ? calculateNext(cube, len, len, level - 1) : countHash;
}

int main(void) {
    FILE* f = fopen("test17.txt", "r");
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
        printf("part 1: count activate %d\n", calculateNext(buf, sideLen, 1, 6));
    }
    return 0;
}
