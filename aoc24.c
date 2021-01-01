#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

/* hexagonal memory mapping
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
    bool on;
} tile_t;

tile_t tile[500];
int tileCount = 0;

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

void insert(tile_t c) { 
    //printf("%d, %d\n", c.x, c.y);
    for (int i = 0; i < tileCount; i++) {
        tile_t *a = &tile[i];
        if (a->x == c.x && a->y == c.y) {
            a->on = !a->on; // flip
            //puts(a->on ? "on" : "off");
            return;
        }
    }
    if (tileCount >= sizeof tile / sizeof tile[0]) {
        puts("tileCount too low");
        exit(1);
    }
    //puts("new");
    tile[tileCount] = c;
    tile[tileCount++].on = true;
}

int main(void) {
    FILE* f = fopen("input24.txt", "r");
    if (f != NULL) {
        char buf[256];
        while (fgets(buf, sizeof buf, f) != NULL) {
            insert(getCoord(buf));
        }
        fclose(f);
        int count = 0;
        for (int i = 0; i < tileCount; i++) {
            count += tile[i].on ? 1 : 0;
        }
        printf("Part 1: %d\n", count);
    }
    return 0;
}
