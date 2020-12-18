#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

int64_t calc(char *f, char **r);

int64_t value(char *f, char **r) {
    *r = f;
    if (isdigit(*f)) {
        return strtol(*r, r, 0); 
    } else if (**r == '(') {
        *r += 1;
        return calc(*r, r);
    }
    return 0;
}

int64_t calc(char *f, char **r) {
    int64_t num = value(f, r);
    while (**r != 10 && **r != 0 && **r != ')') {
        switch ((*r)[1]) {
            case '+':
                num += value(&(*r)[3], r);
                break;
            case '*':
                num *= value(&(*r)[3], r);
                break;
        }
    }
    *r += 1;
    return num;
}

int main(void) {
    char *r;
    // printf("71 == %d\n", calc("1 + 2 * 3 + 4 * 5 + 6", &r));
    // printf("51 == %d\n", calc("1 + (2 * 3) + (4 * (5 + 6))", &r));
    // printf("13632 == %d\n", calc("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", &r));
    
    FILE* f = fopen("input18.txt", "r");
    if (f != NULL) {
        int64_t  sum = 0;
        char buf[512];
        while (fgets(buf, sizeof buf, f) != NULL) {
            sum += calc(buf, &r);
        }
        fclose(f);
        printf("part 1: sum of all lines = %lld\n", sum);
    }
    return 0;
}
