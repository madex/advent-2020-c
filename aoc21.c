#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char name[40];
    int cnt;
    int linkAl;
    int al[32]; // count of allergian
} ingredient_t;

typedef struct {
    char name[40];
    int ingr[512];
} allergen_t;

ingredient_t ingredient[512];
int ingredientCount = 0;
allergen_t allergen[32];
int allergenCount = 0;

int findIngredient(char *ingr) {
    for (int i = 0; i < ingredientCount; i++) {
        if (strcmp(ingredient[i].name, ingr) == 0) {
            ingredient[i].cnt++;
            return i;
        }
    }
    ingredient[ingredientCount].cnt = 1;
    return ingredientCount++;
}

int findAllergen(char *allerg) {
    for (int i = 0; i < allergenCount; i++) {
        if (strcmp(allergen[i].name, allerg) == 0) {
            return i;
        }
    }
    return allergenCount++;
}

void read(char *buf) {
    if (*buf != 0 && *buf != 10) {
        char *b = buf;
        b = strstr(b, "(contains ");
        *b = 0;
        b += 10;
        int al[32];
        int alCount = 0;
        uint32_t mask = 0;
        char *token = strtok(b, " ,)");
        while (token != 0 && *token != 10) {
            int id = findAllergen(token);
            if (id >= 32) printf("32  are not enough"), exit(1);
            al[alCount++] = id;
            strncpy(allergen[id].name, token, sizeof allergen[0].name);
            token = strtok(NULL, " ,)");
        }
        b = buf;
        token = strtok(b, " ");
        while (token != 0) {
            int ingrIdx = findIngredient(token);
            ingredient_t *ingr = &ingredient[ingrIdx];
            strcpy(ingr->name, token);
            for (int i = 0; i < alCount; i++) {
                ingr->al[al[i]]++;
                allergen[al[i]].ingr[ingrIdx]++;
            }
            token = strtok(NULL, " ");
        }
    }
}

int countAllerg(allergen_t *alerg, int comp) {
    int result = 0;
    for (int i = 0; i < ingredientCount; i++) {
        if (alerg->ingr[i] >= comp) {
            result++;
        }
    }
    return result;
}

int countIngr(ingredient_t *ingr, int comp) {
    int result = 0;
    for (int i = 0; i < allergenCount; i++) {
        if (ingr->al[i] >= comp) {
            result++;
        }
    }
    return result;
}

int compAl(const void *p1, const void *p2) {
    int i = *(int*) p1, j = *(int*) p2;
    return strcmp(allergen[ingredient[i].linkAl].name, allergen[ingredient[j].linkAl].name);
}

int main(void) {
    FILE* f = fopen("input21.txt", "r");
    if (f != NULL) {
        char buf[1024];
        int part = 0;
        //memset(ingredient, 0xff, sizeof ingredient);
        while (fgets(buf, sizeof buf, f) != NULL) {
            read(buf);
        }
        fclose(f);
        for (int times = 0; times < allergenCount; times++) { 
            for (int a = 0; a < allergenCount; a++) {
                int max = 0;
                int maxId = 0;
                allergen_t *alerg = &allergen[a];
                for (int i = 0; i < ingredientCount; i++) {
                    if (alerg->ingr[i] > max) {
                        max = alerg->ingr[i];
                        maxId = i;
                    }
                }
                if (max > 0 && countAllerg(alerg, alerg->ingr[maxId]) == 1) {
                    for (int i = 0; i < ingredientCount; i++) {
                        ingredient_t *ingr = &ingredient[i];
                        if (i != maxId) {
                            ingr->al[a] = 0;
                            alerg->ingr[i] = 0;
                        }
                    }
                    ingredient[maxId].linkAl = a;
                    for (int i = 0; i < allergenCount; i++) {
                        if (i != a) {
                            ingredient[maxId].al[i] = 0;
                            allergen[i].ingr[maxId] = 0;
                        }
                    }
                }
            }
        }
        int sum = 0;
        int result[allergenCount];
        int resultCount = 0;
    
        for (int i = 0; i < ingredientCount; i++) {
            ingredient_t *ingr = &ingredient[i];
            int cnt = countIngr(ingr, 1);
            if (cnt == 0) {
                sum += ingr->cnt;
            } else {
                result[resultCount++] = i;
            }
        }
        qsort(result, resultCount, sizeof result[0], compAl);
        printf("part 1: sum of ingreediant counts without allergens %d\n", sum);
        printf("part 2: ");
        for (int i = 0; i < resultCount; i++) {
            printf("%s,", ingredient[result[i]].name);
        }
        printf("\b \n");
    }
    return 0;
}
