#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    unsigned int right;
    unsigned int down;
    unsigned int x;
    unsigned int y;
    unsigned int trees;
} state_t;

void checkLine(state_t *s, const char *line, size_t maxlen) {
    if ((s->y % s->down) == 0) {
        int len = strnlen(line, maxlen);
        if (line[s->x % (len-1)] == '#') { // -1 to skip \n at end
            s->trees++;
        }
        s->x += s->right;
    }
    s->y++;
}

int main() {
    FILE* f = fopen("input3.txt", "r");
    if (f != NULL) {
        char buf[200];
        state_t r[] = {
            { .right = 1, .down = 1, .x = 0, .y = 0, .trees = 0},
            { .right = 3, .down = 1, .x = 0, .y = 0, .trees = 0},
            { .right = 5, .down = 1, .x = 0, .y = 0, .trees = 0},
            { .right = 7, .down = 1, .x = 0, .y = 0, .trees = 0},
            { .right = 1, .down = 2, .x = 0, .y = 0, .trees = 0}
        };
        const int rules = sizeof r / sizeof r[0];
        while (fgets(buf, sizeof buf, f) != NULL) {
            for (int i = 0; i < rules; i++) {
                checkLine(&r[i], buf, sizeof buf);
            }
        }
        unsigned long long mul = 1;
        for (int i = 0; i < rules; i++) {
            mul *= r[i].trees;
        }
        printf("%d trees\n", r[1].trees);
        printf("%llu mul trees\n", mul);
        fclose(f);
    }
    return 0;
}
