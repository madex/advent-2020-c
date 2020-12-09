#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 25

int main() {
    FILE* f = fopen("input9.txt", "r");
    if (f != NULL) {
        char buf[100];
        int lines = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            lines++;
        }
        fseek(f, 0, SEEK_SET);
        int *nums = (int*) calloc(lines, sizeof(int));
        int i = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            nums[i] = strtol(buf, 0, 0);
            bool valid = false;
            if (i >= BLOCK_SIZE) {
                for (int j = i - BLOCK_SIZE; j < i; j++) {
                    for (int k = j + 1; k < i; k++) {
                        if ((nums[j] + nums[k]) == nums[i]) {
                            valid = true;
                            break;
                        }
                    }
                }
                if (!valid) {
                    printf("%d in not a sum of to number from the 25 previous\n", nums[i]);
                    // now find the block
                    for (int j = 0; j < i; j++) {
                        int sum = nums[j];
                        int min = sum, max = sum;
                        for (int k = j + 1; k < i; k++) {
                            sum += nums[k];
                            if (nums[k] < min) {
                                min = nums[k];
                            }
                            if (nums[k] > max) {
                                max = nums[k];
                            }
                            if (sum == nums[i]) {
                                printf("found group %d to %d  min+max = %d\n", 
                                    nums[j], nums[k], min + max);
                                return 0;
                            }
                        }
                    }
                }
            }
            i++;
        }
        fclose(f);
    }
    return 0;
}
