// https://tronche.com/gui/x/xlib/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>      // CLOCK_MONOTONIC, clock_gettime
#include <cstring>     // memset
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <unistd.h>

#include "defines.h"
#include "x11_stuff.h"
#include "board.h"

#include "x11_stuff.cpp"
#include "board.cpp"

static struct timespec
get_wall_clock(void)
{
    struct timespec clock;
    clock_gettime(CLOCK_MONOTONIC, &clock);
    return clock;
}

static f32
get_seconds_elapsed(struct timespec start, struct timespec end)
{
    return ((f32)(end.tv_sec - start.tv_sec)
            + ((f32)(end.tv_nsec - start.tv_nsec) * 1e-9f));
}

static void
refresh_image(XState *xState, BoardManager *bm, int mouseBlockX, int mouseBlockY)
{
    x_clear_image(xState);
    board_push(bm, &xState->screen, mouseBlockX, mouseBlockY);
    x_draw_image(xState);
}

int main(int argCount, char **arguments)
{
    time_t t;
    srand(time(&t)); // Random seed
    
    XState xState = {};
    if(x_setup(&xState))
    {
        BoardManager bm = {};
        board_setup(&bm, &xState.screen);
        
        int mouseBlockX = 0, mouseBlockY = 0;
        x_get_mouse_block(&xState, &mouseBlockX, &mouseBlockY);

        Key left = {}, right = {}, up = {}, down = {}, esc = {}, shift = {};
        x_get_keys(&xState, &left, &right, &up, &down, &esc, &shift);
        
        f32 lastBoardUpdate = 0.0f, lastMouseQuery = 0.0f, lastKeysQuery = 0.0f, lastMovementX = 0.0f, lastMovementY = 0.0f;
        timespec timeStamp = get_wall_clock();
        timespec curTime = timeStamp;

        b32 isRunning = true;
        b32 fatigue = false;
        b32 resetOnFull = false;
        b32 randomTetrominoPosition = false;
        
        while (isRunning)
        {
            curTime = get_wall_clock();
            f32 secondsElapsed = get_seconds_elapsed(timeStamp, curTime);
            
            lastBoardUpdate += secondsElapsed * (1.0f + (bm.score / 10.0f));
            lastMouseQuery  += secondsElapsed;
            lastKeysQuery   += secondsElapsed;
            lastMovementX   += secondsElapsed;
            lastMovementY   += secondsElapsed;

            b32 refresh = false;
            
            if(!bm.full && (lastBoardUpdate >= BOARD_UPDATE_TIME)) {
                board_update(&bm, resetOnFull, randomTetrominoPosition);
                lastBoardUpdate -= BOARD_UPDATE_TIME;
                fatigue = false;
                refresh = true;
            }
            
            if(lastMouseQuery >= MOUSE_QUERY_TIME) {
                lastMouseQuery -= MOUSE_QUERY_TIME;
                refresh |= x_get_mouse_block(&xState, &mouseBlockX, &mouseBlockY);
            }
            
            if(lastKeysQuery >= KEYS_QUERY_TIME) {
                if(x_get_keys(&xState, &left, &right, &up, &down, &esc, &shift))
                {
                    if(left.isPressed && board_move_tetromino_left(&bm)) {
                        lastMovementX = -X_MOVEMENT_SPEED;
                        refresh = true;
                    }

                    if(right.isPressed && board_move_tetromino_right(&bm)) {
                        lastMovementX = -X_MOVEMENT_SPEED;
                        refresh = true;
                    }

                    if(up.isPressed && board_rotate_tetromino(&bm)) {
                        refresh = true;
                    }

                    if(down.isPressed) {
                        lastMovementY = Y_MOVEMENT_SPEED;
                    }

                    if(esc.isPressed) {
                        isRunning = false;
                    }

                    if(shift.isPressed && board_drop_tetromino(&bm)) {
                        lastBoardUpdate = BOARD_UPDATE_TIME;
                        refresh = true;
                    }
                }
                
                lastKeysQuery -= KEYS_QUERY_TIME;
            }
            
            if(lastMovementX >= X_MOVEMENT_SPEED) {
                if(left.isDown && board_move_tetromino_left(&bm)) {
                    refresh = true;
                }

                if(right.isDown && board_move_tetromino_right(&bm)) {
                    refresh = true;
                }
                
                lastMovementX -= X_MOVEMENT_SPEED;
            }

            if(lastMovementY >= Y_MOVEMENT_SPEED) {
                if(down.isDown) {
                    if(board_move_tetromino_down(&bm)) {
                        lastBoardUpdate = 0.0f;
                        refresh = true;
                    }
                    else if (!fatigue) {
                        lastBoardUpdate = BOARD_UPDATE_TIME / 4.0f;
                        fatigue = true;
                    }
                }
                
                lastMovementY -= Y_MOVEMENT_SPEED;
            }

            if(refresh) { refresh_image(&xState, &bm, mouseBlockX, mouseBlockY); }
            timeStamp = curTime;

            usleep(10);
        }
        
        free(xState.screen.pixels);
    }
    
    return 0;
}