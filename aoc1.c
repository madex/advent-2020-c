#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* f = fopen("input1.txt", "r");
    if (f != NULL) {
        char buf[200];
        int lines = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            lines++;
        }
        fseek(f, 0, SEEK_SET);
        int *years = (int*) calloc(lines, sizeof(int));
        int *y = years;
        while (fgets(buf, sizeof buf, f) != NULL) {
            *y++ = strtol(buf, 0, 0);
        }
        for (int i = 0; i < lines; i++) {
            for (int j = i; j < lines; j++) {
                if (years[i] + years[j] == 2020) {
                    printf("%d * %d = %d\n", 
                            years[i], years[j], 
                            years[i] * years[j]
                    );
                }
            }
        }
        for (int i = 0; i < lines; i++) {
            for (int j = i; j < lines; j++) {
                for (int k = j; k < lines; k++) {
                    if (years[i] + years[j] + years[k] == 2020) {
                        printf("%d * %d * %d = %d\n", 
                                years[i], years[j], years[k], 
                                years[i] * years[j] * years[k]
                        );
                    }
                }
            }
        }
    }
    return 0;
}
