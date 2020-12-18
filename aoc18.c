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

int64_t calc2(char *f, char **r, bool brace);

int64_t value2(char *f, char **r) {
    *r = f;
    if (isdigit(*f)) {
        return strtol(*r, r, 0); 
    } else if (**r == '(') {
        *r += 1;
        return calc2(*r, r, true);
    }
    return 0;
}

int64_t calc2(char *f, char **r, bool brace) {
    int64_t num = value2(f, r);
    while (**r != 10 && **r != 0 && **r != ')') {
        switch ((*r)[1]) {
            case '+':
                num += value2(&(*r)[3], r);
                break;
            case '*':
                num *= calc2(&(*r)[3], r, false);
                break;
        }
    }
    if (brace && **r == ')') {
        *r += 1;
    }
    return num;
}

int main(void) {
    char *r;
    // printf("71 == %lld\n", calc("1 + 2 * 3 + 4 * 5 + 6", &r));
    // printf("51 == %lld\n", calc("1 + (2 * 3) + (4 * (5 + 6))", &r));
    // printf("13632 == %lld\n", calc("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", &r));
    
    // printf("231 == %lld\n", calc2("1 + 2 * 3 + 4 * 5 + 6", &r, false));
    // printf("51 == %lld\n", calc2("1 + (2 * 3) + (4 * (5 + 6))", &r, false));
    // printf("669060 == %lld\n", calc2("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))", &r, false));
    // printf("1445 == %lld\n", calc2("5 + (8 * 3 + 9 + 3 * 4 * 3)", &r, false));
    // printf("23340 == %lld\n", calc2("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2", &r, false));

    FILE* f = fopen("input18.txt", "r");
    if (f != NULL) {
        int64_t  sum = 0;
        int64_t  sum2 = 0;
        char buf[512];
        while (fgets(buf, sizeof buf, f) != NULL) {
            sum += calc(buf, &r);
            sum2 += calc2(buf, &r, false);
        }
        fclose(f);
        printf("part 1: sum of all lines = %lld\n", sum);
        printf("part 2: sum of all lines = %lld\n", sum2);
    }
    return 0;
}
