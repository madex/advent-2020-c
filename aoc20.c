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
    struct tile *right;
    struct tile *down;
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
void printTileRot(tile_t *t);
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

uint16_t getSideMask(tile_t *t, bool right) {
    uint16_t mask = 0;
    for (int j = 0; j < 10; j++) {
        if (right) {
            mask |= get2(t, 9, j) == '#' ? 1 << j : 0; 
        } else {
            mask |= get2(t, j, 9) == '#' ? 1 << j : 0; 
        } 
    }
    return mask;
}

bool fitRight(tile_t *left, tile_t *right) {
    for (int i = 0; i < 10; i++) {
        if (get2(left, 9, i) != get2(right, 0, i)) {
            return false;
        }
    }
    return true;
}

bool fitDown(tile_t *up, tile_t *down) {
    for (int i = 0; i < 10; i++) {
        if (get2(up, i, 9) != get2(down, i, 0)) {
            return false;
        }
    }
    return true;
}

// 


bool setRotation(tile_t *pre, tile_t *cur, bool right) {
    int linkCountCur = 0, linkCountPrev = 0;
    for (int j = 0; j < 4; j++) {
        linkCountCur  += cur->link[j] != 0;
        if (pre != 0) {
            linkCountPrev += pre->link[j] != 0;
        } 
    }
    bool end = linkCountPrev > linkCountCur;
    bool checkRight = (right && !end) || !right;
    bool checkDown  = (!right && !end);
    for (int i = 0; i < 8; i++) {
        cur->rot = i;
        if (pre == 0 || (right ? fitRight(pre, cur) : fitDown(pre, cur))) {
            printTileRot(cur);
            int  countMatch = 0;
            for (int j = 0; j < 4; j++) {
                if (checkRight && cur->link[j] != 0 && pre != cur->link[j]) {
                    if (setRotation(cur, cur->link[j], true)) {
                        printf("%d right %d(rot=%d)\n", cur->id, cur->link[j]->id, cur->link[j]->rot);
                        cur->right = cur->link[j];
                        countMatch++;
                    }
                }
            }
            for (int j = 0; j < 4; j++) {
                if (checkDown && cur->link[j] != 0 && pre != cur->link[j]) {
                    if (setRotation(cur, cur->link[j], false)) {
                        printf("%d down %d(rot=%d)\n", cur->id, cur->link[j]->id, cur->link[j]->rot);
                        cur->down = cur->link[j];
                        countMatch++;
                    }
                }
            }
            if (countMatch == (checkRight + checkDown)) {
                printf("id %d match rot=%d\n", cur->id, cur->rot);
                return true;
            } else {
                cur->right = cur->down = 0;
            }
        }
    }
    
    return false;
}

void printTileRot(tile_t *t) {
    printf("%d rot %d\n", t->id, t->rot);
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            printf("%c", get(t->data, t->rot, x, y));
        }
        puts("");
    }
    puts("");
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

char *copyImageRow(char *result, tile_t *t, int row) {
    // for (int i = 1; i < 9; i++) {
    //     *result++ = get(t->data, t->rot, i, row);
    // }
    // if (t->link[next] == 0) {
    //     return result;
    // } else {
    //     return copyImageRow(result, t->link[next], row);
    // }
    return result;
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
    setRotation(0, &tile[firstCorner], false);
    // char result[8*8*tileCount + 1];
    // memset(result, 0, 8*8*tileCount + 1);
    // // start with the first corner
    // char *r = result;
    // oriantateFirst(&tile[firstCorner]);
    // for (int i = 1; i < 2/*9*/; i++) {
    //     r = copyImageRow(r, &tile[firstCorner], i, 0);
    // }
    // int lineLen = strlen(result);
    // tile_t *t = getDown(&tile[firstCorner]);
    // r = copyImageRow(r, t, 1, 0);
    // tile_t *t2 = getDown(t);
    // r = copyImageRow(r, t2, 1, 0);
}

#define ASSERT_EQ(a, b, r, c) {if (a != b) printf("%s:%d error %d %d "#a" (%d) != "#b" (%d)\n", __FILE__, __LINE__, r, c, a, b); }

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
        // for (int i = 0; i < tileCount; i++) {
        //     printTileRot(&tile[i]);
        // }
    }
    return 0;
}
