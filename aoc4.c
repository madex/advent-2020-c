#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

const char fields[][4] = {"byr", "iyr", "eyr", "hgt" , "hcl", "ecl", "pid"};

bool numberInRage(char *numStr, int min, int max) {
    int num = strtol(numStr, 0, 0);
    return (num >= min) && (num <= max);
}

bool isHexColor(char *str) {
    bool valid = false;
    int count = 0;
    if (*str++ == '#') {
        while (isxdigit(*str++)) {
            count++;
        }
        valid = (count == 6);
    }
    return valid;
}

bool isEyeColor(char *str) {
    const char col[][4] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    for (size_t i = 0; i < sizeof col / sizeof col[0]; i++) {
        if (strncmp(str, col[i], 3) == 0) {
            return true;
        }
    }
    return false;
}

bool isNDigitNum(char *str, int digits) {
    while (isdigit(*str++)) {
        digits--;
    }
    return digits == 0;
}

bool checkPassport(char *line, uint8_t *resultMask, uint8_t *resultMask2) {
    const char s[2] = " ";
    if (strlen(line) < 2) {
        return true;
    }
    char *token = strtok(line, s);
    while (token != NULL) {
        for (size_t i = 0; i < 7; i++) {
            if (strncmp(fields[i], token, 3) == 0) {
                *resultMask |= (1 << i);
                bool valid = false;
                switch (i) {  // 0123
                    case 0:  // byr: four digits; at least 1920 and at most 2002
                        valid = numberInRage(&token[4], 1920, 2002);
                        break;
                    case 1:  // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
                        valid = numberInRage(&token[4], 2010, 2020);
                        break;
                    case 2: // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
                        valid = numberInRage(&token[4], 2020, 2030);
                        break;
                    case 3: { // hgt (Height) - a number followed by either cm or in:
                        size_t len = strlen(token);
                        if (token[len - 1] == 10) { len--; }
                        if (strncmp(&token[len-2], "in", 2) == 0) {
                            valid = numberInRage(&token[4], 59, 76);
                        } else if (strncmp(&token[len-2], "cm", 2) == 0) {
                            valid = numberInRage(&token[4], 150, 193);
                        }
                        break;
                    }
                    case 4: // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
                        valid = isHexColor(&token[4]);
                        break;
                    case 5: // ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
                        valid = isEyeColor(&token[4]);
                        break;
                    case 6: // pid (Passport ID) - a nine-digit number, including leading zeroes.
                        valid = isNDigitNum(&token[4], 9);
                        break;
                }
                if (valid) {
                    *resultMask2 |= (1 << i);
                }
            }
        }
        token = strtok(NULL, s);
    }
    return false;
}

int main() {
    FILE* f = fopen("input4.txt", "r");
    if (f != NULL) {
        char buf[512];
        uint8_t validMask1 = 0; // bit 0 byr .. bit 7 cid
        uint8_t validMask2 = 0;
        uint32_t validCount1 = 0;
        uint32_t validCount2 = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (checkPassport(buf, &validMask1, &validMask2)) {
                if (validMask1 == 0x7f) {
                    validCount1++;
                }
                if (validMask2 == 0x7f) { // no newline at end?
                    validCount2++;
                }
                validMask1 = 0;
                validMask2 = 0;
            }
        }
        if (validMask1 == 0x7f) { // no newline at end?
            validCount1++;
        }
        if (validMask2 == 0x7f) { // no newline at end?
            validCount2++;
        }
        printf("%d valid\n", validCount1);
        printf("%d valid with check\n", validCount2);
        fclose(f);
    }
}
