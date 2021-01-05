#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

struct tile {
    int  id;
    char data[10][10];     // all tiles have 10*10
    int rot; // rotation of the tile
    uint16_t  sideMask[8]; // top richt bottom left and all mirroed again
    struct tile *link[4];
};

typedef struct tile tile_t;

int tileSides[1024]; // array for counting side Masks (10 Bit)
tile_t tile[150];
int tileCount = 0;
int firstCorner = -1;
const char seaMonster[] = "                  # " 
                          "#    ##    ##    ###"
                          " #  #  #  #  #  #   ";
int seaMonsterOffset[15][2];
void calcSeamonsterOffsets(void) {
    char *s = (char*) seaMonster;
    int index = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 20; x++) {
            if (*s++ == '#') {
                seaMonsterOffset[index][0] = x;
                seaMonsterOffset[index++][1] = y;
            }
        }
    }
}

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
            if (firstCorner < 0) {
                firstCorner = i;
            }
            cornerProduct *= t->id;
            printf("%d  index=%d\n", t->id, i);
        }
    }
    return cornerProduct;
}

#define IDX2D(y, x, lenx) (y)*(lenx) + (x)
char getAdv(char *d, int rot, int x, int y, int len) {
    int l = len - 1; 
    return rot == 0 ? d[IDX2D(y,     x,     len)] : //   0    
           rot == 1 ? d[IDX2D(x,     l - y, len)] : //  90    
           rot == 2 ? d[IDX2D(l - y, l - x, len)] : // 180    
           rot == 3 ? d[IDX2D(l - x, y,     len)] : // 270    
           rot == 4 ? d[IDX2D(y,     l - x, len)] : // mirr x
           rot == 5 ? d[IDX2D(l - y, x,     len)] : // mirr y
           rot == 6 ? d[IDX2D(x,     y,     len)] : // flipped
                      d[IDX2D(l - x, l - y, len)];  // mirrored flipped
}

char get(char d[10][10], int rot, int x, int y) { // for compatibility
    return getAdv((char*)d, rot, x, y, 10);
}

char get2(tile_t *t, int x, int y) {
    return getAdv((char*)t->data, t->rot, x, y, 10);
}

bool fit(tile_t *left, tile_t *right) {
    for (int i = 0; i < 10; i++) {
        if (get2(left, 9, i) != get2(right, 0, i)) {
            return false;
        }
    }
    return true;
}

void printTileRot(tile_t *t, int rot) {
    printf("%d rot %d\n", t->id, rot);
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            printf("%c", get(t->data, rot, x, y));
        }
        puts("");
    }
    puts("");
}
int getRot(int sideMask) {
    return  sideMask == 3 ? 0 :  
            sideMask == 2 ? 1 :
            sideMask == 1 ? 2 :
            sideMask == 4 ? 3 :
            sideMask == 5 ? 4 :
            sideMask == 7 ? 5 :
            sideMask == 0 ? 6 : 7;
}
/*  rotation mirrorin combinations
sides   ! means reversed
    1     x123
 4     2  y
    3     1
          2             t
 1 2 3 4   0      y  x  0 
!4 1!2 3  90      x !y  1 
!3!4!1!2 180     !y !x  0
 2!3 4!1 270     !x  y  1
!1 4!3 2  mirr x  y !x  0
 3!2 1!4  mirr y !y  x  0
 4 3 2 1  fl      x  y  
!2!1!4!3  mir fl !x !y   
 */
int getSidemaskLeft(int rot) {
    return rot == 0 ? 3 :
           rot == 1 ? 2 :
           rot == 2 ? 5 :
           rot == 3 ? 4 :
           rot == 4 ? 1 :
           rot == 5 ? 7 :
           rot == 6 ? 0 : 6;
}

tile_t *getDown(tile_t *t) {
    int i = getSidemaskLeft(t->rot);
    int down = (i + 1) % 4;
    tile_t *ret = t->link[down];
    int downMask = t->sideMask[i > 3 ? down : down + 4];
    for (i = 0; i < 8; i++) {
        if (downMask == ret->sideMask[i]) {
            int n = (i + 1) % 4;
            ret->rot = getRot(i > 3 ? n + 4 : n);
            break;
        }
    }
    return ret;
}

void oriantateFirst(tile_t *t) {
    for (int rot = 0; rot < 8; rot++) {
        int i = getSidemaskLeft(rot);
        int up = (i + 1) % 4;
        int left = i % 4;
        if (t->link[up] == 0 && t->link[left] == 0) {
            t->rot = i;
        }
    }
}

char *copyImageRow(char *result, tile_t *t, int row, int rightMask) {
    // right 0 1 2 3 4 5 6 7
    //       1 2 3 4!1!2!3!4
    // rot   3 4 1 0   2   5
    int next = 0, nextMask;
    if (rightMask == 0) {
        int i = getSidemaskLeft(t->rot);
        next = (i + 2) % 4;
        nextMask = i > 3 ? next : next + 4;
    } else {
        for (int i = 0; i < 8; i++) {
            if (rightMask == t->sideMask[i]) {
                t->rot = getRot(i);
                next = (i + 2) % 4;
                nextMask = i > 3 ? next : next + 4;
                break;
            }
        }
    }
    printTileRot(t, t->rot);
    for (int i = 1; i < 9; i++) {
        *result++ = get(t->data, t->rot, i, row);
    }
    if (t->link[next] == 0) {
        return result;
    } else {
        return copyImageRow(result, t->link[next], row, t->sideMask[nextMask]);
    }
}

void puzzzle() {
    // link puzzel tiles
    for (int i = 0; i < tileCount; i++) {
        tile_t *t = &tile[i];
        for (int j = 0; j < 4; j++) {
            if (tileSides[t->sideMask[j]] >= 2) {
                for (int k = i + 1; k < tileCount; k++) {
                    tile_t *o = &tile[k]; // o = other
                    for (int l = 0; l < 8; l++) {
                        if (t->sideMask[j] == o->sideMask[l]) {
                            t->link[j] = o; // double link
                            o->link[l % 4] = t;
                        }
                    }
                }
            }
        }
    }
    char result[8*8*tileCount + 1];
    memset(result, 0, 8*8*tileCount + 1);
    // start with the first corner
    char *r = result;
    oriantateFirst(&tile[firstCorner]);
    for (int i = 1; i < 2/*9*/; i++) {
        r = copyImageRow(r, &tile[firstCorner], i, 0);
    }
    int lineLen = strlen(result);
    tile_t *t = getDown(&tile[firstCorner]);
    r = copyImageRow(r, t, 1, 0);
    tile_t *t2 = getDown(t);
    r = copyImageRow(r, t2, 1, 0);
}

#define ASSERT_EQ(a, b, r, c) {if (a != b) printf("%s:%d error %d %d "#a" (%d) != "#b" (%d)\n", __FILE__, __LINE__, r, c, a, b); }
void rotTest(void) {
    tile_t *t = &tile[1];
    for (int rot = 0; rot < 8; rot++) {
        int curMask[4] = {0};
        for (int j = 0; j < 10; j++) {
            int k = 9 - j;
            uint16_t bitMask = (1 << j);
            curMask[0] |= (get(t->data, rot, j, 0) == '#' ? bitMask : 0);
            curMask[1] |= (get(t->data, rot, 9, j) == '#' ? bitMask : 0);
            curMask[2] |= (get(t->data, rot, k, 9) == '#' ? bitMask : 0);
            curMask[3] |= (get(t->data, rot, 0, k) == '#' ? bitMask : 0);
        }
        int left = getSidemaskLeft(rot);
        for (int i = 0; i < 4; i++) {
            int m = left + i;
            int n = m % 4;
            int mask = t->sideMask[m > 0 ? n : n + 4];
            ASSERT_EQ(curMask[i], mask, rot, i);
        }
    }
}


int main(void) {
    FILE* f = fopen("test20.txt", "r");
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
        calcSeamonsterOffsets();
        puzzzle();
        rotTest();
    }
    return 0;
}
