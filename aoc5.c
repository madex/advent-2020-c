#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int readBinarySeatID(char *str) {
    int res = 0;
    while (strchr("FBLR", *str) != NULL) {
        res *= 2;
        if (strchr("BR", *str++) != NULL) {
            res += 1;
        }
    }
    return res;
}

int findMySeat(bool seat[]) {
    for (int i = 2; i < 1024; i++) {
        if (seat[i-2] && !seat[i-1] && seat[i]) {
            return i - 1;
        }
    }
    return -1; // not seat found
}

int main() {
    // test cases 
    // printf("357 = %d\n", readBinarySeatID("FBFBBFFRLR"));
    // printf("567 = %d\n", readBinarySeatID("BFFFBBFRRR\n"));
    FILE* f = fopen("input5.txt", "r");
    if (f != NULL) {
        char buf[100];
        bool seat[1024] = {false};
        int highest = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            int id = readBinarySeatID(buf);
            seat[id] = true;
            if (id > highest) {
                highest = id;
            }
        }
        fclose(f);
        printf("highest seat ID = %d\n", highest);
        printf("my seat ID %d\n", findMySeat(seat));
    }
    return 0;
}
