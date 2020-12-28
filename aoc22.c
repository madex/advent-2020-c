#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    int data[60];
    int dataCount;
} buf_t;


typedef struct {
    buf_t player[2];
    uint32_t combination[2000]; // player 1 lower 16 Bit player 2 upper 16 bit
    int combinationCount;
    int won;
    int game, round;
} game_t;

void bufInit(buf_t *r) {
    r->dataCount = 0;
}

int takeFromBegin(buf_t *r) {
    if (r->dataCount == 0) {
        printf("empty buf\n");
        exit(1);
    }
    int result = r->data[0];
    r->dataCount--;
    int *to = &r->data[0], *from = &r->data[1];
    for (int i = r->dataCount; i > 0; i--) {
        *to++ = *from++;
    }
    return result;
}

void addToEnd(buf_t *r, int add) {
    if (r->dataCount >= (sizeof r->data / sizeof r->data[0])) {
        printf("buf to small\n");
        exit(1);
    }
    r->data[r->dataCount++] = add;
}

int bufLen(buf_t *r) {
    return r->dataCount;
}

void bufPrint(buf_t *r, int player) {
    printf("Player %d: ", player + 1);
    buf_t *rp = &r[player];
    for (int i = 0; i < rp->dataCount; i++) {
        printf("%d, ", rp->data[i]);
    }  
    puts("\b\b  ");
}

void printGameResult(buf_t *r) {
    int j = 1;
    int sum = 0;
    for (int i = r->dataCount - 1; i >= 0; i--) {
        sum += j++ * r->data[i];
    }
    printf("GameResult: %d\n", sum);
}

void play1(game_t *g) {
    g->won = g->player[1].dataCount == 0 ? 1 : 
             g->player[0].dataCount == 0 ? 2 : 0;
    if (g->won > 0) {
        //printf("Player %d wins\n", g->won);
        return;
    }
    int p1 = takeFromBegin(&g->player[0]), p2 = takeFromBegin(&g->player[1]);
    if (p1 < p2) {
        addToEnd(&g->player[1], p2);
        addToEnd(&g->player[1], p1);
    } else {
        addToEnd(&g->player[0], p1);
        addToEnd(&g->player[0], p2);
    } 
    // bufPrint(&g->player[0], 0);
    // bufPrint(&g->player[0], 1);
    play1(g);
}

bool addTurnWasBefore(game_t *g, int p1, int p2) {
    uint32_t combined = p1 | ((uint32_t) p2 << 16);
    for (int i = 0; i < g->combinationCount; i++) {
        if (g->combination[i] == combined) {
            return true;
        }
    }
    g->combination[g->combinationCount++] = combined;
    if (g->combinationCount > sizeof g->combination / sizeof g->combination[0]) {
        puts("combination array too small");
        exit(0);
    }
    return false;
}

void play2(game_t *g) {
    int len1 = bufLen(&g->player[0]), len2 = bufLen(&g->player[1]);
    g->won = len2 == 0 ? 1 : 
             len1 == 0 ? 2 : 0;

    if (g->won == 0 && addTurnWasBefore(g, g->player[0].data[0], g->player[1].data[0])) {
        g->won = 1;
    }
    if (g->won > 0) {
        //printf("Player %d wins\n", g->won);
        //printGameResult(&g->player[g->won - 1]);
        return;
    }
    int p1 = takeFromBegin(&g->player[0]), p2 = takeFromBegin(&g->player[1]);
    bool player2Wins = p1 < p2;
    if (len1 > p1 && len2 > p2) {
        game_t subGame = *g;
        subGame.game++;
        subGame.round = 0;
        subGame.combinationCount = 0;
        // printf(">> Game %d <<\n", subGame.game);
        play2(&subGame);
        player2Wins = bufLen(&subGame.player[1]) > 0;
        // printf(">> END Game <<\n");
    }
    if (player2Wins) {
        addToEnd(&g->player[1], p2);
        addToEnd(&g->player[1], p1);
    } else {
        addToEnd(&g->player[0], p1);
        addToEnd(&g->player[0], p2);
    }
    g->round++;
    // printf("round %d game %d combinations %d\n", g->round, g->game, g->combinationCount);
    // bufPrint(&g->player[0], 0);
    // bufPrint(&g->player[0], 1);
    if (g->round == 63 && g->game == 11) {
        g->game = 11;
    }
    play2(g);
}

int main(void) {
    FILE* f = fopen("input22.txt", "r");
    if (f != NULL) {
        char buf[256];
        int part = 0;
        game_t game;
        bufInit(&game.player[0]);
        bufInit(&game.player[1]);
        while (fgets(buf, sizeof buf, f) != NULL) {
            if (*buf != 0 && *buf != 10) {
                if (isdigit(*buf)) {
                    addToEnd(&game.player[part], strtol(buf, 0, 0));
                }
            } else {
                part++;
            }
        }
        // bufPrint(game.player, 0);
        // bufPrint(game.player, 1);
        game_t game2 = game;
        play1(&game);
        printGameResult(&game.player[game.won - 1]);
        play2(&game2);
        printGameResult(&game2.player[game2.won - 1]);
        fclose(f);
    }
    return 0;
}
