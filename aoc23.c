#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

int take3(char *inOut, char *three, int position, int len) {
    int result = len - 3;
    for (int i = 0; i < 3; i++) { // copy to three
        int x = (position + i + 1) % len;
        three[i] = inOut[x];
    }
    int next = position + 4;
    if (next > len) { // overlab. if == len no nothing to do.
        next -= len;
        for (int i = 0; i < next; i++) { // copy to three
            inOut[i] = ' ';
            result++;
        }
    } else {
        for (int i = next; i < len; i++) { // copy to three
            inOut[i - 3] = inOut[i];
        }
    }
    inOut[result] = 0;  // for debugging
    return result;
}

int getPosition(char inOut[10], char find, int len) {
    for (int i = 0; i < len; i++) { // copy to three
        if (inOut[i] == find) {
            return i;
        }
    }
    return 0;
}

int getDestination(char inOut[10], int position, int len) {
    char find = inOut[position] - 1;
    while (1) {
        if (find <= '0') {
            find = '9';
        }
        for (int i = 0; i < len; i++) { // copy to three
            if (inOut[i] == find) {
                return i;
            }
        }
        find--;
    }
}

void insert3(char inOut[10], char three[4], int position,  int cur, int len) {
    if (position <= cur) {  // don´t move current
        // fillup end
        int x = cur - len + 4; // fix spaces at begin
        x = x > 0 ? x : 0;
        int to = (len - 3 + x) % len;
        for (int i = x; i < position; i++) {
            inOut[to] = inOut[i];
            to = (to + 1) % len;
        }
        //insert
        for (int i = 0; i < 3; i++) {
            inOut[to] = three[i];
            to = (to + 1) % len;
        }
    } else {
        for (int i = 0; i < 3; i++) {
            int to = position + i;
            if (to + 3 < len) {
                inOut[to + 3] = inOut[to];
            }
            inOut[to] = three[i];
        }
    }
}

char *crabCups(char inOut[10], int rounds) {
    int cur = 0;
    char three[4] = {0};
    int len = strlen(inOut);
    for (int r = 0; r < rounds; r++) {
        // puts(inOut);
        int l = take3(inOut, three, cur, len);
        int dest = getDestination(inOut, cur, l);
        insert3(inOut, three, dest + 1, cur, len);
        cur = (cur + 1) % len;
    }
    // puts(inOut);
    int pos = getPosition(inOut, '1', len);
    char help[10] = {0};
    memcpy(help, inOut, len);
    int from = (pos + 1) % len;
    for (int i = 0; i < len - 1; i++) {
        inOut[i] = help[from];
        from = (from + 1) % len;
    }
    inOut[len - 1] = 0;
    return inOut;
}

int main(void) {
    char in[] = "487912365";
    printf("Part 1: %s\n", crabCups(in, 100));
    return 0;
}
