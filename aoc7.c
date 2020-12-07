#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id, count;
} link_t;

typedef struct {
    char col[40];
    bool linked;
    link_t linkToIndex[5];
    int linkCount;
} color_t;

color_t colors[1000];
int colorsCount = 0;

char *readColor(char **in) {
    char *result = *in;
    char *end = strstr(*in, " bag");
    *end = 0;
    *in = end + 14;
    return result;
}

char *readNColor(char **in, int *n) {
    *n = strtol(*in, in, 0);
    char *result = *in + 1;
    char *end = strstr(*in, " bag");
    if (end == NULL) {
        return NULL;
    }
    *end = 0;
    char *end2 = strstr(end+1, ", ");
    if (end2 == NULL) {
        *in = end;
    } else {
        *in = end2 + 2;
    }
    return result;
}

int getIndexFromString(char *str) {
    for (int i = 0; i < colorsCount; i++) {
        if (strcmp(colors[i].col, str) == 0) {
            return i;
        }
    }
    return -1;
}

void markLinksTo(int id) {
    for (int i = 0; i < colorsCount; i++) {
        color_t *c = &colors[i];
        for (int j = 0; j < c->linkCount; j++) {
            if (c->linkToIndex[j].id == id) {
                colors[i].linked = true;
                markLinksTo(i);
            }
        }
    }
}

int countBagColorsThatContain(char * str) {
    int i = getIndexFromString(str);
    if (i == -1) { return -1; }
    markLinksTo(i);
    int count = 0;
    for (int i = 0; i < colorsCount; i++) {
        count += (colors[i].linked ? 1 : 0); 
        colors[i].linked = false;
    }
    return count;
}

int countInside(int id) {
    color_t *c = &colors[id];
    int count = 1;
    for (int j = 0; j < c->linkCount; j++) {
        link_t *l = &c->linkToIndex[j];
        count += l->count*countInside(l->id);
    }
    return count;
}

int countBagsInside(char * str) {
    int i = getIndexFromString(str);
    if (i == -1) { return -1; }
    return countInside(i) - 1; // not count the shiny gold one
}

int main(void) {
    FILE* f = fopen("input7.txt", "r");
    if (f != NULL) {
        char buf[512];
        char *b = buf;
        int n = 0;
        char *result;
        char *mainColor;
        while (fgets(buf, sizeof buf, f) != NULL) {
            b = buf;
            mainColor = readColor(&b);
            strcpy(colors[colorsCount++].col, mainColor);
        }
        fseek(f, 0, SEEK_SET);
        int i = 0;
        while (fgets(buf, sizeof buf, f) != NULL) {
            b = buf;
            mainColor = readColor(&b);
            result = readNColor(&b, &n);
            color_t *col = &colors[i++];
            while (result != NULL) {
                int id = getIndexFromString(result);
                link_t *l = &col->linkToIndex[col->linkCount++];
                l->id    = id;
                l->count = n;
                result = readNColor(&b, &n);
            }
        }
        printf("%d bag colors that could contain a shiny gold bag\n", 
            countBagColorsThatContain("shiny gold"));
        printf("%d bag could be insige a shiny gold bag\n", 
            countBagsInside("shiny gold"));
        fclose(f);
    }
    return 0;
}
