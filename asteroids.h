#ifndef __ASTEROIDS__H_
#define __ASTEROIDS__H_
#include <stdio.h>
#include <stdlib.h>

#define FIELD_HEIGHT 20
#define FIELD_WIDTH  40
#define P_NEW_ASTEROID    5
#define P_LARGER_ASTEROID   2
#define ASTEROID_VAL    1
#define SHIP_VAL        2
#define SHIP_WIDTH  3

#define MOVE_UP -1
#define MOVE_DOWN 1
#define MOVE_NO 0

struct ship_action{
    int move;
    void *state;
};

struct ship_action move_ship(int field[][FIELD_WIDTH], void *ship_state);

#endif