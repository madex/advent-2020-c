#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    unsigned int min;
    unsigned int max;
    char c;
} rule_t;

bool parseRule(char **next, rule_t *rule) {
    rule->min = strtol(*next, next, 10);
    if (**next != '-') { return false; }
    (*next)++;
    rule->max = strtol(*next, next, 10);
    (*next)++;
    rule->c = **next;
    (*next)++;
    if (!isalpha(rule->c)) { return false; }
    if (**next != ':') { return false; }
    (*next) += 2;
    return true;
}

bool passwordValid(char *str) {
    char *next = str;
    rule_t r;
    if (!parseRule(&next, &r)) {
        return false;
    }
    int count = 0;
    while (isalpha(*next)) {
        if (*next == r.c) {
            count++;
        }
        next++;
    }
    return (count >= r.min) && (count <= r.max);
}

bool passwordValid2(char *str) {
    rule_t r;
    char *next = str;
    if (!parseRule(&next, &r)) {
        return false;
    }
    unsigned int len = strlen(next);
    r.min--; r.max--;
    if ((len <= r.min) || (len <= r.max)) { 
        return false;
    }
    return (next[r.min] == r.c) != (next[r.max] == r.c);
}

int main() {
    FILE* f = fopen("input2.txt", "r");
    if (f != NULL) {
        char buf[200];
        int validCount = 0;
        int validCount2 = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (passwordValid(buf)) {
                validCount++;
            }
            if (passwordValid2(buf)) {
                validCount2++;
            }
        }
        fclose(f);
        printf("%d valid for rule 1\n", validCount);
        printf("%d valid for rule 2\n", validCount2);
    }
    return 0;
}
