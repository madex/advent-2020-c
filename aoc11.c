#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const int dirs[8][2] = {
    {-1, -1}, {-1, 0}, {-1,  1}, 
    { 0, -1},          { 0,  1}, 
    { 1, -1}, { 1, 0}, { 1,  1}
};

int personOnSeatNext(int l, int ll, char *b, size_t lines, size_t lineLen) {
    int neibours = 0;
    for (int i = 0; i < 8; i++) {
        int x = ll + dirs[i][0], y = l + dirs[i][1];
        if (x >= 0 && y >= 0 && x < lineLen && y < lines && 
            b[y*lineLen + x] == '#') {
            neibours++;
        }
    }
    return neibours == 0 ? '#' : neibours > 3 ? 'L' : b[l*lineLen + ll];
} 

int personOnSeatNext2(int l, int ll, char *b, size_t lines, size_t lineLen) {
    int neibours = 0;
    for (int i = 0; i < 8; i++) {
        bool notFound = true;
        int m = 1;
        while (notFound) {
            int x = ll + dirs[i][0]*m, y = l + dirs[i][1]*m;
            if (x < 0 || y < 0 || x >= lineLen || y >= lines || b[y*lineLen + x] == 'L')
                notFound = false; 
            else if (b[y*lineLen + x] == '#') {
                neibours++;
                notFound = false; 
            }
            m++;
        }
    }
    return neibours == 0 ? '#' : neibours > 4 ? 'L' : b[l*lineLen + ll];
} 


void print(char *b, size_t lines, size_t lineLen) {
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < lineLen; j++) {
            putchar(*b++);
        }
        putchar('\n');
    }
    putchar('\n');
}



int gameOfSeats(char *b, size_t lines, size_t lineLen) {
    char cur[lines*lineLen];
    int rounds = 0, occupied = 0;
    do {
        memcpy(cur, b, lines*lineLen);
        occupied = 0;
        char *bp = b;
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < lineLen; j++) {
                if (*bp =='#' || *bp =='L') {
                    *bp = personOnSeatNext(i, j, cur, lines, lineLen);
                    if (*bp == '#') {
                        occupied++;
                    }
                }
                bp++;
            }
        }
        rounds++;
        //printf("occupied %d\n", occupied);
        //print(b, lines, lineLen);
    } while (memcmp(cur, b, lines*lineLen) != 0);
    printf("rounds %d\n", rounds);
    return occupied;
}

int gameOfSeats2(char *b, size_t lines, size_t lineLen) {
    char cur[lines*lineLen];
    int rounds = 0, occupied = 0;
    do {
        memcpy(cur, b, lines*lineLen);
        occupied = 0;
        char *bp = b;
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < lineLen; j++) {
                if (*bp =='#' || *bp =='L') {
                    *bp = personOnSeatNext2(i, j, cur, lines, lineLen);
                    if (*bp == '#') {
                        occupied++;
                    }
                }
                bp++;
            }
        }
        rounds++;
        //printf("occupied %d\n", occupied);
        //print(b, lines, lineLen);
    } while (memcmp(cur, b, lines*lineLen) != 0);
    printf("rounds %d\n", rounds);
    return occupied;
}
int main() {
    FILE* f = fopen("input11.txt", "r");
    if (f != NULL) {
        char buf[300];
        size_t lines = 0;
        size_t lineLen = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            lines++;
        }
        fseek(f, 0, SEEK_SET);
        lineLen = strlen(buf);
        if (buf[lineLen - 1] == 10) {  // remove double newline
            lineLen--;
        }
        char block[lineLen*lines+1];  
        size_t i = 0;
        while (fgets(&block[i*lineLen], sizeof buf, f) != NULL) {
            i++;
        }
        char blockCopy[lineLen*lines];
        memcpy(blockCopy, block, lineLen*lines);
        printf("count 1 = %d\n", gameOfSeats(block, lines, lineLen));
        printf("count 2 = %d\n", gameOfSeats2(blockCopy, lines, lineLen));
      }
    return 0;
}
