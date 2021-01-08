#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

struct tile {
    int  id;
    char data[10][10];     // all tiles have 10*10
    int rot; // rotation or better mutation of the tile
    uint16_t  sideMask[8]; // top richt bottom left and all mirroed again
    struct tile *link[4];
    struct tile *right;
    struct tile *down;
};

typedef struct tile tile_t;

int tileSides[1024]; // array for counting side Masks (10 Bit)
tile_t tile[150];
int tileCount = 0;
int corners[4];
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
    int c = 0;
    for (int i = 0; i < tileCount; i++) {
        tile_t *t = &tile[i];
        int sides = 0;
        for (int j = 0; j < 4; j++) {
            if (tileSides[t->sideMask[j]] == 1) {
                sides++;
            }
        }
        if (sides == 2) {
            corners[c++] = i;
            cornerProduct *= t->id;
            printf("%d  index=%d\n", t->id, i);
        }
    }
    return cornerProduct;
}

#define IDX2D(y, x, lenx) (y)*(lenx) + (x)
char getAdv(char *d, int rot, int x, int y, int len) {
    if (x >= len || y >= len) return 0;
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

bool setRotation(tile_t *pre, tile_t *cur, tile_t *up, bool right) {
    int linkCountCur = 0, linkCountPrev = 0;
    for (int j = 0; j < 4; j++) {
        linkCountCur  += cur->link[j] != 0;
        if (pre != 0) {
            linkCountPrev += pre->link[j] != 0;
        } 
    }
    bool end = linkCountPrev > linkCountCur;
    tile_t *nextUp = !right ? pre : (up != 0 ? up->right : 0);
    bool checkRight = (right && !end) || !right;
    bool checkDown  = (!right && !end);
    bool checkUp    = nextUp != 0 && right;
    for (int i = 0; i < 8; i++) {
        cur->rot = i;
        if (pre == 0 || (right ? fitRight(pre, cur) : fitDown(pre, cur))) {
            bool found[4] = {false, false, false, false};
            // printTileRot(cur);
            int  countMatch = 0;
            if (checkUp && nextUp != 0) {
                if (fitDown(nextUp, cur)) {
                    // printf("%d up matches\n", cur->id);
                    countMatch++;
                    for (int j = 0; j < 4; j++) {
                        if (cur->link[j] == nextUp) {
                            found[j] = true;
                        } 
                    }
                }
            }
            for (int j = 0; j < 4; j++) {
                if (checkRight && !found[j] && cur->link[j] != 0 && pre != cur->link[j]) {
                    if (setRotation(cur, cur->link[j], nextUp, true)) {
                        // printf("%d right %d(rot=%d)\n", cur->id, cur->link[j]->id, cur->link[j]->rot);
                        cur->right = cur->link[j];
                        found[j] = true;
                        countMatch++;
                        break;
                    }
                }
            }
            for (int j = 0; j < 4; j++) {
                if (checkDown && !found[j] && cur->link[j] != 0 && pre != cur->link[j]) {
                    if (setRotation(cur, cur->link[j], 0, false)) {
                        // printf("%d down %d(rot=%d)\n", cur->id, cur->link[j]->id, cur->link[j]->rot);
                        cur->down = cur->link[j];
                        countMatch++;
                        break;
                    }
                }
            }
            if (countMatch == (checkRight + checkDown + checkUp)) {
                // printf("id %d match rot=%d\n", cur->id, cur->rot);
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

void printRot(char *r, int rot, int len) {
    printf("rot %d\n", rot);
    for (int y = 0; y < len; y++) {
        for (int x = 0; x < len; x++) {
            printf("%c", getAdv(r, rot, x, y, len));
        }
        puts("");
    }
    puts("");
}

char *copyImageRow(char *result, tile_t *t, int row) {
    for (int i = 1; i < 9; i++) {
        *result++ = get2(t, i, row);
    }
    if (t->right == 0) {
        return result;
    } else {
        return copyImageRow(result, t->right, row);
    }
    return result;
}

int puzzzle() {
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
    int c = 0;
    //while (c < 4 && !setRotation(0, &tile[corners[c]], 0, false)) c++;
    setRotation(0, &tile[corners[c]], 0, false);
    if (c == 4) return 0;
    char result[8*8*tileCount +2];
    memset(result, 0, 8*8*tileCount + 1);
    // start with the first corner
    char *r = result;
    tile_t *t = &tile[corners[c]];
    // get line len
    r = copyImageRow(r, t, 1);
    int len = strlen(result);
    int rows = 0;
    r = result;
    // copy mached puzzel to array
    while (t != 0) {
        for (int i = 1; i < 9; i++) {
            r = copyImageRow(r, t, i);
            rows++;
        }
        t = t->down;
    }
    int countHash = 0;
    
    // search for seamonster in all rotations
    for (int rot = 0; rot < 8; rot++) {
        // printRot(result, rot, len);
        countHash = 0;
        int countSeamonster = 0;
        for (int x = 0; x < len; x++) {
            for (int y = 0; y < rows;  y++) {
                char b = getAdv(result, rot, x, y, len);
                countHash += b == '#';
                bool match = true;
                for (int o = 0; o < 15; o++) {
                    int *off = seaMonsterOffset[o];
                    int ox = x + off[0], oy = y + off[1];
                    if (getAdv(result, rot, ox, oy, len) != '#') {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    countSeamonster++;
                }
            }
        }
        if (countSeamonster > 0) {
            // printf("%d Seamonsters found\n", countSeamonster);
            countHash -=  countSeamonster * 15;
            break;
        }
    }
    return countHash;
}

#define ASSERT_EQ(a, b, r, c) {if (a != b) printf("%s:%d error %d %d "#a" (%d) != "#b" (%d)\n", __FILE__, __LINE__, r, c, a, b); }

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
        calcSeamonsterOffsets();
        printf("Part 2: %d\n",  puzzzle());
    }
    return 0;
}
