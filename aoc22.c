#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    int data[200];
    int begin, end;
} ringbuf_t;

void ringBufInit(ringbuf_t *r) {
    r->begin = r->end = 0;
}

int takeFromBegin(ringbuf_t *r) {
    if (r->begin != r->end) {
        int result = r->data[r->begin++];
        r->begin %= sizeof r->data / sizeof r->data[0];
        return result;
    } else {
        return 0; // empty
    }
}

void addToEnd(ringbuf_t *r, int add) {
    r->data[r->end++] = add;
    r->end %= sizeof r->data / sizeof r->data[0];
    if (r->end == r->begin) {
        printf("ringbuf to small\n");
        exit(1);
    }
}

void ringBufPrint(ringbuf_t *r, int player) {
    printf("Player %d: ", player + 1);
    ringbuf_t *rp = &r[player];
    for (int i = rp->begin; i != rp->end; i = (i+1) % (sizeof r->data / sizeof r->data[0])) {
        printf("%d, ", rp->data[i]);
    }  
    puts("\b\b  ");
}

void printPart1(ringbuf_t *r) {
    int j = 1;
    int sum = 0;
    for (int i = r->end - 1; i != r->begin - 2; i = (i - 1) % (sizeof r->data / sizeof r->data[0])) {
        sum += j++ * r->data[i];
    }
    printf("Part1: %d\n", sum);
}

void play(ringbuf_t *r) {
    int p1 = takeFromBegin(&r[0]), p2 = takeFromBegin(&r[1]);
    int won = p2 == 0 ? 1 : p1 == 0 ? 2 : 0;
    if (won > 0) {
        printf("Player %d wins\n", won);
        printPart1(&r[won - 1]);
        return;
    } else if (p1 < p2) {
        addToEnd(&r[1], p2);
        addToEnd(&r[1], p1);
    } else {
        addToEnd(&r[0], p1);
        addToEnd(&r[0], p2);
    } 
    // ringBufPrint(r, 0);
    // ringBufPrint(r, 1);
    play(r);
}

int main(void) {
    FILE* f = fopen("input22.txt", "r");
    if (f != NULL) {
        char buf[256];
        int part = 0;
        ringbuf_t player[2];
        ringBufInit(&player[0]);
        ringBufInit(&player[1]);
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (*buf != 0 && *buf != 10) {
                if (isdigit(*buf)) {
                    addToEnd(&player[part], strtol(buf, 0, 0));
                }
            } else {
                part++;
            }
        }
        // ringBufPrint(player, 0);
        // ringBufPrint(player, 1);
        play(player);
        fclose(f);
    }
    return 0;
}
