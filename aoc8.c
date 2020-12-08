#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char inst;  // only use the first char to get rid of strcmp and strcpy
    bool exe; // this line is already executet
    int  value;
} instr_t;

int highestPc = 0;

bool readInstruction(char *line, instr_t *instr) {
    if (line == 0 || instr == 0) {
        return true;
    }
    if (strlen(line) < 6) {
        return true;
    }
    instr->inst = line[0];
    instr->value = strtol(&line[4], 0, 0);
    return false;
}

int execToUntilFirstRepeat(instr_t *code, int codeLines) {
    int acc = 0;
    int pc = 0;
    while (pc < codeLines && !code[pc].exe) {
        instr_t *c = &code[pc];
        c->exe = true;
        switch (c->inst) {
            case 'a':
                acc += c->value;
                pc++;
                break;
            case 'j':
                pc += c->value;
                break;
            default:
                pc++;
                break;
        }
        if (pc > highestPc) {
            highestPc = pc;
        }
    }
    return acc;
}

void reset(instr_t *code, int codeLines) {
    for (int i = 0; i < codeLines; i++) {
        code[i].exe = false; // reset execution mark
    }
}

int fixAndExecToUntilFirstRepeat(instr_t *code, int codeLines) {
    // bruteforce chang one instruction
    for (int i = 0; i < codeLines; i++) {
        reset(code, codeLines);
        instr_t *c = &code[i];
        if (c->inst != 'a') {
            c->inst = c->inst == 'j' ? 'n' : 'j'; 
            int acc = execToUntilFirstRepeat(code, codeLines);
            if (highestPc >= codeLines - 1) {
                printf("fix line %d\n", i);
                return acc;
            }
            c->inst = c->inst == 'j' ? 'n' : 'j'; 
        } 
    }
    return 0;
}

int main() {
    FILE* f = fopen("input8.txt", "r");
    if (f != NULL) {
        char buf[200];
        int lines = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            lines++;
        }
        fseek(f, 0, SEEK_SET);
        instr_t *code = (instr_t*) calloc(lines, sizeof(instr_t));
        int i = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            readInstruction(buf, &code[i++]);
        }
        printf("last acc = %d\n", execToUntilFirstRepeat(code, lines));
        printf("last acc after fix = %d\n", 
            fixAndExecToUntilFirstRepeat(code, lines));
    }
    return 0;
}
