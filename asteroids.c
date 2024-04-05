#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include "asteroids.h"

const char chars[] = {0, '1', '2'};

int update_field(int field[][FIELD_WIDTH], int ship_row){
    int i,j;
    int dice;

    // Start by shifting the columns to the left
    for (j=1;j<FIELD_WIDTH;j++)
        for (i=0;i<FIELD_HEIGHT;i++)
            field[i][j-1] = field[i][j];

    // Update the last column
    j = FIELD_WIDTH - 1;
    for (i=0;i<FIELD_HEIGHT;i++){
        // Roll the dice
        dice = random()%100;
        // If there was an asteroid in the previous column we may extend it
        if (field[i][j-1] == 1){
            if (dice < P_LARGER_ASTEROID) field[i][j] = ASTEROID_VAL;
            else field[i][j] = 0;
        }else{
            // If there was no asteroid we may create a new one
            if (dice < P_NEW_ASTEROID) field[i][j] = ASTEROID_VAL;
            else field[i][j] = 0;
        }
    }

    // Check collision
    for (int i=0;i<SHIP_WIDTH;i++){
        if (field[ship_row][i] != 0){
            return 1;
        }
    }


    // Add the ship
    for (int i=0;i<SHIP_WIDTH;i++) field[ship_row][i] = SHIP_VAL;
    return 0;
}

void update_screen(int field[][FIELD_WIDTH]){
    int i,j;
    clear();
    wmove(stdscr, 0,0);

    for (i=0;i<FIELD_HEIGHT;i++)
        for (j=0;j<FIELD_WIDTH;j++) 
            if (field[i][j])
                mvwaddch(stdscr, i, j, chars[field[i][j]]);
}

void main(){
    int ship_row = FIELD_HEIGHT>>1;
    void *ship_state = NULL;
    struct ship_action nxt_action;
    char position[10] = "";
    int step = 0;
    int collision = 0;

    int asteroid_field[FIELD_HEIGHT][FIELD_WIDTH];

    // Initially the field is empty
    memset(asteroid_field, 0, sizeof(int)*FIELD_WIDTH*FIELD_HEIGHT);


    initscr();
    cbreak();
    noecho();

    while (1){
        if (update_field(asteroid_field, ship_row)) break;
        nxt_action = move_ship(asteroid_field, ship_state);
        update_screen(asteroid_field);
        // Clear current ship position
        for (int i=0;i<SHIP_WIDTH;i++) asteroid_field[ship_row][i] = 0;

        ship_state = nxt_action.state;
        if (nxt_action.move > 0){
            ship_row ++;
            mvwaddch(stdscr, 0, 0, '+');
        }else if (nxt_action.move < 0){
            ship_row --;
            mvwaddch(stdscr, 0, 0, '-');
        }else mvwaddch(stdscr, 0, 0, '0');
        if (ship_row < 0) ship_row = 0;
        if (ship_row >= FIELD_HEIGHT) ship_row = FIELD_HEIGHT-1;

        sprintf(position, "%d: %d", step, ship_row);
        mvwaddstr(stdscr, FIELD_HEIGHT, 0, position);
        getch();
        step++;
    }

    clear();
    mvwaddstr(stdscr, FIELD_HEIGHT>>1, FIELD_WIDTH>>2, "Game over!");
    getch();

    endwin();
}