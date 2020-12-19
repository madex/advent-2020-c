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

bool checkRule(int ruleId, char *s, char **sret) {
    rule_t *r = &rule[ruleId];
    *sret = s;
    bool result = false;
    if (r->subRuleCount == 0) {
        result = **sret == r->match;
        (*sret)++;
    } else if (*s == 0 || *s == 10) {
    } else {
        for (int sr = 0; sr < r->subRuleCount; sr++) {
            int i = 0;
            *sret = s;
            bool rulePartValid = true;
            for (int i = 0; r->subRule[sr][i] != 0; i++) {
                if (!checkRule(r->subRule[sr][i], *sret, sret)) {
                    rulePartValid = false;
                }
            }
            if (rulePartValid) {
                return true;
            }
        }
    }
    return result;
}

bool isValid(char *s) {
    return checkRule(0, s, &s) && (*s == 0 || *s == 10);
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
        // printf("part 2 product of all departures %llu\n", departureProduct());
        fclose(f);
    }
    return 0;
}
