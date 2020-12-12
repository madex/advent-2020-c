#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int dir; // 0 = east
    int n, e;
    // for 2nd part
    int way_n, way_e;
    int n2, e2;
} ship_t;

void rotateWaypoint(int deg, ship_t *ship) {
    int help;
    switch (deg / 90) {
        case 1:
            help        =  ship->way_n;
            ship->way_n = -ship->way_e;
            ship->way_e =  help;
            break;
        case 2:
            ship->way_n = -ship->way_n;
            ship->way_e = -ship->way_e;
            break;
        case 3:
            help        = -ship->way_n;
            ship->way_n =  ship->way_e;
            ship->way_e =  help;
            break;
    }
}

void parseCommand(char *in, ship_t *ship) {
    int value = strtol(&in[1], 0, 10);
    switch (in[0]) {
        case 'N':
            ship->n     += value;
            ship->way_n += value;
            break;
        case 'S':
            ship->n     -= value; 
            ship->way_n -= value;
            break;
        case 'E':
            ship->e     += value;
            ship->way_e += value;
            break;
        case 'W':
            ship->e     -= value;
            ship->way_e -= value;
            break; 
        case 'L':
            ship->dir -= value; 
            if (ship->dir < 0) {
                ship->dir += 360;
            }
            rotateWaypoint(360 - value, ship);
            break; 
        case 'R': ship->dir += value; 
            if (ship->dir >= 360) {
                ship->dir -= 360;
            }
            rotateWaypoint(value, ship);
            break; 
        case 'F':
            switch (ship->dir / 90) {
                case 0: ship->e += value; break; // East
                case 1: ship->n -= value; break; // South
                case 2: ship->e -= value; break; // West
                case 3: ship->n += value; break; // North
                default:
                    break;
            }
            ship->n2 += value * ship->way_n;
            ship->e2 += value * ship->way_e;
            break;
        default:
            break;
    }
}

int main(void) {
    FILE* f = fopen("input12.txt", "r");
    if (f != NULL) {
        char buf[200];
        ship_t ship = { 
            .dir = 0, .e = 0, .n = 0, 
            .way_e = 10, .way_n = 1, .n2 = 0, .e2 = 0
        };
        while (fgets(buf, sizeof buf, f) != NULL) {
            parseCommand(buf, &ship);
        }
        printf("manhatten distance %d\n", 
            abs(ship.e) + abs(ship.n));
        printf("waypoint manhatten distance %d\n", 
            abs(ship.e2) + abs(ship.n2));
        fclose(f);
    }
    return 0;
}
