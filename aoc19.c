#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char match;
    int subRule[3][3]; // subrule 0 as endmark
    int subRuleCount;  // match if subRuleCount == 0
} rule_t;

rule_t rule[200];

typedef struct {
    int strPosCount;
    int strPos[100];
} result_t;

void parseRule(char *b) {
    int num = strtol(b, &b, 0);
    if (num >= sizeof rule / sizeof rule[0]) {
        printf("error rule\n");  
        return;
    }
    rule_t *r = &rule[num];
    b += 2; // skip ": "
    if (*b == '\"') {
        r->match = b[1];
        r->subRuleCount = 0;
    } else {
        int i = 0, subRule = 0;
        while (isdigit(*b)) {
            r->subRule[subRule][i++] = strtol(b, &b, 0);
            b++;
            if (*b == '|') {
                subRule++;
                i = 0;
                b += 2; // skip "| "
            }
        }
        r->subRuleCount = subRule + 1;
    }
}

void checkRule(int ruleId, char *s, int pos, result_t *res, int depth) {
    rule_t *r = &rule[ruleId];
    if (s[pos] == 0 || s[pos] == 10 || depth > 20) {
    } else if (r->subRuleCount == 0) {
        if (s[pos] == r->match) {
            res->strPos[res->strPosCount++] = pos + 1; // add to result list
        }
    } else {
        for (int sr = 0; sr < r->subRuleCount; sr++) {
            result_t work = { .strPosCount = 1, .strPos = {pos} };
            for (int i = 0; r->subRule[sr][i] != 0; i++) {
                result_t rule = { .strPosCount = 0 };
                for (int w = 0; w < work.strPosCount; w++) {
                    checkRule(r->subRule[sr][i], s, work.strPos[w], &rule, depth + 1);
                }
                work = rule;
            }
            memcpy(&res->strPos[res->strPosCount], &work.strPos[0], work.strPosCount * sizeof work.strPos[0]);
            res->strPosCount += work.strPosCount;
        }
    }
}

bool isValid(char *s) {
    //puts(s);
    result_t res = { 0 };
    
    bool result = false;
    checkRule(0, s, 0, &res, 0);
    for (int i = 0; i < res.strPosCount; i++) {
        char c = s[res.strPos[i]];
        if (c == 0 || c == 10) {
            result = true;
        }
    }
    return result;
}

int main(void) {
    FILE* f = fopen("input19.txt", "r");
    if (f != NULL) {
        char buf[256];
        int part = 0;
        int sum = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (*buf != 0 && *buf != 10) {
                switch (part){
                    case 0: // read rule
                        parseRule(buf);
                        break;

                    case 1: // check if valid
                        sum += isValid(buf) ? 1 : 0;
                        break;
                }
            } else {
                part++;
            }
        }
        printf("part 1 sum of all valid  %d\n", sum);

        fseek(f, 0, SEEK_SET);
        memset(rule, 0, sizeof rule);
        sum = 0; part = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (*buf != 0 && *buf != 10) {
                switch (part){
                    case 0: // read rule
                        parseRule(buf);
                        break;

                    case 1: // check if valid
                        sum += isValid(buf) ? 1 : 0;
                        break;
                }
            } else {
                part++;
                // rule update
                rule[8].subRuleCount = 2;
                rule[8].subRule[1][0] = 42;
                rule[8].subRule[1][1] = 8;
                rule[11].subRuleCount = 2;
                rule[11].subRule[1][0] = 42;
                rule[11].subRule[1][1] = 11;
                rule[11].subRule[1][2] = 31;
            }
        }
        printf("part 2 sum of all valid  %d\n", sum);
        fclose(f);
    }
    return 0;
}
