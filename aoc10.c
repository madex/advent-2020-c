#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int compInt(const void *p1, const void *p2) {
    return *(int*)p1 - *(int*)p2;
}

int calcJolts(int *nums, size_t len) {
    qsort(nums, len, sizeof nums[0], compInt);
    int jolt1 = 0, jolt3 = 1, last = 0;
    for (int i = 0; i < len; i++) {
        int n = nums[i]; 
        switch (n - last) {
            case 1: jolt1++; break;
            case 2: break;
            case 3: jolt3++; break;
            default: printf("error %d\n", n - last);
        }
        last = n;
    }
    return jolt1 * jolt3;
}

long countArrangments(int *nums, size_t len) {
    long paths[len];
    for (int i = 0; i < len; i++) {
        paths[i] = nums[i] <= 3;
        for (int j = i - 1; (j >= 0) && (nums[i] - nums[j]) <= 3; j--) {
            paths[i] += paths[j];
        }
    }
    return paths[len - 1];
}

int main() {
    FILE* f = fopen("input10.txt", "r");
    if (f != NULL) {
        char buf[100];
        int lines = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            lines++;
        }
        fseek(f, 0, SEEK_SET);
        int *nums  = (int*) calloc(lines, sizeof(int));
        int i = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            nums[i] = strtol(buf, 0, 0);
            i++;
        }
        printf("product of jolt 1 count and jolt 3 count %d\n", calcJolts(nums,  lines));
        printf("%li different arrangements possible\n", countArrangments(nums,  lines));
        fclose(f);
    }
    return 0;
}
