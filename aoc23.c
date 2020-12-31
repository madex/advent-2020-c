#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

struct linkedCircle {
    int val;
    struct linkedCircle *next;
};

typedef struct linkedCircle linkedCircle_t;

int decFind(int find, int len) {
    return --find <= 0 ? len - 1 : find;
}

linkedCircle_t *findDest(linkedCircle_t *all, linkedCircle_t *cur, linkedCircle_t *three, int len) {
    int find = decFind(cur->val, len);
    linkedCircle_t *help;
    for (int j = 0; j < 3; j++) {
        help = three;
        for (int i = 0; i < 3; i++) {
            if (help->val == find) {
                find = decFind(find, len); 
            }
            help = help->next;
        }
    }
    return &all[find]; 
}

uint64_t crabCups(char inOut[10], int rounds, bool part2) {
    int slen = strlen(inOut);
    int len = part2 ? 1000001 : slen + 1;
    
    linkedCircle_t *lc = (linkedCircle_t*) malloc(len * sizeof *lc);
    linkedCircle_t *next, *cur, *three, *dest = 0;
    // init linked list (ring)
    for (int i = 0; i < slen; i++) {
        int val = inOut[i] - '0';
        dest = next;
        next = &lc[val];
        next->val = val;
        if (i  == 0) {
            cur = next;
            cur->next = 0;

        } else {
            dest->next = next;
        }
    }
    if (part2) {
        for (int i = slen + 1; i < len; i++) {
            dest = next;
            next = &lc[i];
            next->val = i;
            dest->next = next;
        }
    }
    next->next = cur; // close ring
    // play the game
    for (int r = 0; r < rounds; r++) {
        // take three elements
        three = cur->next;
        cur->next = three->next->next->next;
        dest = findDest(lc, cur, three, len);
        three->next->next->next = dest->next;
        dest->next = three;    
        cur = cur->next;
    }
    if (part2) {
        int ret = lc[1].next->val * lc[1].next->next->val;
        free(lc);
        return ret;
    } else {
        char buf[10];
        next = lc[1].next;
        for (int i = 0; i < len - 2; i++) {
            buf[i] = '0' + next->val;
            next = next->next;
        }
        buf[len - 1] = 0;
        free(lc);
        return strtol(buf, 0, 0);
    }
}

int main(void) {
    char in[] = "487912365"; // Test "389125467";
    printf("Part 1: %llu\n", crabCups(in, 100, false));
    printf("Part 2: %llu\n", crabCups(in, 10000000, true));
    return 0;
}
