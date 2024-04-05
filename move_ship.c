#include "asteroids.h"
#include <math.h>

#define MAX_DISTANCE 10

struct ship_state {
    int row;
};

struct ship_action move_ship(int field[][FIELD_WIDTH], void *ship_state) {
    struct ship_action sa;
    struct ship_state *ss = (struct ship_state *) ship_state;

    // If this is the first call to the function, initialize ship state
    if (!ss) {
        ss = malloc(sizeof(struct ship_state));
        ss->row = FIELD_HEIGHT >> 1;
        sa.move = MOVE_NO;
        sa.state = ss;
        return sa;
    }

    // Check if there is an asteroid close to the ship
    int closest_row = -1;
    int closest_col = -1;
    int closest_distance = MAX_DISTANCE + 1;
    for (int row = ss->row - MAX_DISTANCE; row <= ss->row + MAX_DISTANCE; row++) {
        if (row < 0 || row >= FIELD_HEIGHT) continue;
        for (int col = 0; col < FIELD_WIDTH; col++) {
            if (field[row][col] == ASTEROID_VAL) {
                int distance = abs(ss->row - row) + abs(col - SHIP_WIDTH / 2);
                if (distance < closest_distance) {
                    closest_row = row;
                    closest_col = col;
                    closest_distance = distance;
                }
            }
        }
    }

   // If there is an asteroid close to the ship, move to a safe row
    if (closest_distance <= MAX_DISTANCE) {
        int safe_row = -1;
        int safe_distance = MAX_DISTANCE + 1;
        for (int row = ss->row - MAX_DISTANCE; row <= ss->row + MAX_DISTANCE; row++) {
            if (row < 0 || row >= FIELD_HEIGHT) continue;
            if (row == closest_row) continue; // Avoid the row with the closest asteroid
            int distance = abs(ss->row - row);
            if (distance >= closest_distance) continue; // Avoid rows that are too close to the closest asteroid
            if (distance >= safe_distance) continue; // Avoid rows that are too close to previous safe rows
            
            // Check if there are asteroids in the row in front of the ship
            int has_asteroid_in_front = 0;
            for (int col = SHIP_WIDTH / 2; col < FIELD_WIDTH; col++) {
                if (field[row][col] == ASTEROID_VAL) {
                    has_asteroid_in_front = 1;
                    break;
                }
            }
            if (has_asteroid_in_front) continue; // Avoid rows with asteroids in front
            
            int is_safe = 1;
            for (int col = 0; col < FIELD_WIDTH; col++) {
                if (field[row][col] == ASTEROID_VAL) {
                    is_safe = 0;
                    break;
                }
            }
            if (is_safe) {
                safe_row = row;
                safe_distance = distance;
            }
        }
        if (safe_row >= 0) {
            if (safe_row < ss->row) {
                ss->row--;
                sa.move = MOVE_UP;
            } else if (safe_row > ss->row) {
                ss->row++;
                sa.move = MOVE_DOWN;
            } else {
                sa.move = MOVE_NO;
            }
            sa.state = ss;
            return sa;
        }
    }
    
    // If there is no asteroid close to the ship or no safe row, move up or down
    int next_row = ss->row + (rand() % 3) - 1;
    if (next_row < 0) next_row = 0;
    if (next_row >= FIELD_HEIGHT) next_row = FIELD_HEIGHT - 1;
    
    // Check if there are asteroids in the row in front of the ship
    int has_asteroid_in_front = 0;
    for (int col = SHIP_WIDTH / 2; col < FIELD_WIDTH; col++) {
        if (field[next_row][col] == ASTEROID_VAL) {
            has_asteroid_in_front = 1;
            break;
        }
    }
    
    if (has_asteroid_in_front) {
        // If there are asteroids in front, don't move
        sa.move = MOVE_NO;
    } else {
        // If there are no asteroids in front, move up or down
        if (next_row < ss->row) {
            sa.move = MOVE_UP;
        } else if (next_row > ss->row) {
            sa.move = MOVE_DOWN;
        } else {
            sa.move = MOVE_NO;
        }
        ss->row = next_row;
    }
    
    sa.state = ss;
    return sa;
}










