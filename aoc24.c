#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

/* hexagonal coordinate calculating
            +---x--->
            |   | 0 | 2 | 4 | 6          y=-1
-4-2 0 2 4  y  /'\./'\./'\./'\./     "\
 -3-1 1 3 5 |  -1 | 1 | 3 | 5 | 7     |
-4-2 0 2 4  v  \./'\./'\./'\./'\./     > y=0 x -1 0 1 2 3 4 5 6 7 8
 -3-1 1 3 5     | 0 | 2 | 4 | 6 | 8   |
-4-2 0 2 4     /'\./'\./'\./'\./'\   _/
                                         y=1
    x even   x odd
     x  y     x  y
e    2  0     2  0 
w   -2  0    -2  0
se   1  1     1  0
sw  -1  1    -1  0
nw  -1  0    -1 -1
ne   1  0     1 -1
*/

typedef struct {
    int x;
    int y;
    bool black;
} tile_t;

typedef struct {
    tile_t tile[4000];
    int tileCount;
} floor_t;

tile_t getCoord(char *str) {
    tile_t t = { 0, 0, false };
    char *s = str;
    while (*s != 0 && *s != 10) {
        bool xEven = (t.x % 2) == 0; 
        switch (*s) {
            case 'e': t.x += 2; break;
            case 'w': t.x -= 2; break;
            case 's':
                t.y += xEven ? 1 : 0;
                t.x += s[1] == 'e' ? 1 : -1;
                s++;
                break;
            case 'n':
                t.y += xEven ? 0 : -1;
                t.x += s[1] == 'e' ? 1 : -1;
                s++;
                break;
            default:
                printf("parse error\n");
                exit(1);
        }
        s++;
    }
    return t;
}

void insert(tile_t c, floor_t *floor) {
    for (int i = 0; i < floor->tileCount; i++) {
        tile_t *a = &floor->tile[i];
        if (a->x == c.x && a->y == c.y) {
            a->black = !a->black; // flip
            return;
        }
    }
    if (floor->tileCount >= sizeof floor->tile / sizeof floor->tile[0]) {
        puts("tileCount too low");
        exit(1);
    }
    floor->tile[floor->tileCount] = c;
    floor->tile[floor->tileCount++].black = true;
}

int countBlack(floor_t *floor) {
    int count = 0;
    for (int i = 0; i < floor->tileCount; i++) {
        count += floor->tile[i].black ? 1 : 0;
    }
    return count;
}

int mutate(int times, floor_t *floor) {
    const int neighbours[6][3] = { // 0=x, 1=yEven 2=yOdd   e w se sw nw ne
        { 2, 0, 0 }, {-2, 0, 0 }, { 1, 1, 0 }, {-1, 1, 0 }, {-1, 0, -1 }, { 1, 0, -1 }
    };
    floor_t new;
    for (int l = 0; l < times; l++) {
        new.tileCount = 0;
        // For all in the list and for all neighbours that anr not in the list
        for (int i = 0; i < floor->tileCount; i++) {
            tile_t *cur = &floor->tile[i];
            int blackCount = 0;
            for (int n = 0; n < 6; n++) {
                int nx = cur->x + neighbours[n][0];
                int ny = cur->y + neighbours[n][(cur->x % 2) == 0 ? 1 : 2];
                bool neighInList = false;
                for (int j = 0; j < floor->tileCount; j++) {
                    tile_t *t = &floor->tile[j];
                    if (t->x == nx && t->y == ny) {
                        neighInList = true;
                        blackCount += t->black ? 1 : 0;
                    }
                }
                // check also neighbours not in the list
                if (!neighInList) {
                    bool insideReslt = false;
                    // check if there are already in the result
                    for (int j = 0; j < new.tileCount; j++) {
                        if (new.tile[j].x == nx && new.tile[j].y == ny) {
                            insideReslt = true;
                            break;
                        }
                    }
                    if (!insideReslt) {
                        int blackCount2 = 0;
                        tile_t non = { nx, ny, false };
                        for (int m = 0; m < 6; m++) {
                            int mx = non.x + neighbours[m][0];
                            int my = non.y + neighbours[m][(nx % 2) == 0 ? 1 : 2];
                            for (int j = 0; j < floor->tileCount; j++) {
                                tile_t *t = &floor->tile[j];
                                if (t->x == mx && t->y == my) {
                                    blackCount2 += t->black ? 1 : 0;
                                }
                            }
                        }
                        if (blackCount2 == 2) {
                            non.black = true;
                            new.tile[new.tileCount++] = non;
                        }
                    }
                }
            }
            if (cur->black ? !(blackCount == 0 || blackCount > 2) : blackCount == 2) {
                tile_t c = *cur;
                c.black = true;
                new.tile[new.tileCount++] = c;
            }
        }
        *floor = new;
    }
    return countBlack(floor);
}

int main(void) {
    FILE* f = fopen("input24.txt", "r");
    if (f != NULL) {
        char buf[256];
        floor_t floor;
        while (fgets(buf, sizeof buf, f) != NULL) {
            insert(getCoord(buf), &floor);
        }
        fclose(f);
        printf("Part 1: %d\n", countBlack(&floor));
        printf("Part 2: %d\n", mutate(100, &floor));
    }
    return 0;
}
