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
linux_get_wall_clock(void)
{
    struct timespec clock;
    clock_gettime(CLOCK_MONOTONIC, &clock);
    return clock;
}

static f32
linux_get_seconds_elapsed(struct timespec start, struct timespec end)
{
    return ((f32)(end.tv_sec - start.tv_sec)
            + ((f32)(end.tv_nsec - start.tv_nsec) * 1e-9f));
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
        
        int mouseBlockX, mouseBlockY;
        Key left, right, up, down;
        
        b32 isRunning = true;
        b32 resetOnFull = false;
        
        f32 lastBoardUpdate = 0.0f;
        f32 lastMouseQuery  = 0.0f;
        f32 lastKeysQuery   = 0.0f;
        f32 lastMovement    = 0.0f;
        
        timespec timeStamp = linux_get_wall_clock();
        timespec curTime = timeStamp;
        
        while (isRunning)
        {
            curTime = linux_get_wall_clock();
            f32 secondsElapsed = linux_get_seconds_elapsed(timeStamp, curTime);
            
            lastBoardUpdate += secondsElapsed;
            lastMouseQuery  += secondsElapsed;
            lastKeysQuery   += secondsElapsed;
            lastMovement    += secondsElapsed;
            
            if(!bm.full && (lastBoardUpdate >= BOARD_REFRESH_TIME)) {
                board_update(&bm, resetOnFull);
                
                x_clear_image(&xState);
                board_push(&bm, &xState.screen, mouseBlockX, mouseBlockY);
                x_draw_image(&xState);
                
                lastBoardUpdate = 0.0f;
            }
            
            if(lastMouseQuery >= MOUSE_REFRESH_TIME) {
                if(x_get_mouse_block(&xState, &mouseBlockX, &mouseBlockY))
                {
                    x_clear_image(&xState);
                    board_push(&bm, &xState.screen, mouseBlockX, mouseBlockY);
                    x_draw_image(&xState);
                }
                
                lastMouseQuery = 0.0f;
            }
            
            if(lastKeysQuery >= KEYS_REFRESH_TIME) {
                if(x_get_arrow_keys(&xState, &left, &right, &up, &down))
                {
                    printf("arrows changed\n");
                }
                
                lastKeysQuery = 0.0f;
            }
            
            if(!bm.full && (lastMovement >= MOVEMENT_SPEED)) {
                if(down.isDown) {
                    board_update(&bm, resetOnFull);
                    
                    x_clear_image(&xState);
                    board_push(&bm, &xState.screen, mouseBlockX, mouseBlockY);
                    x_draw_image(&xState);
                    
                    lastBoardUpdate = 0.0f;
                }
                
                lastMovement = 0.0f;
            }
            
            usleep(10);
            timeStamp = curTime;
        }
        
        printf("It's full\n");
        while(true); // Keep that screen 
        
        free(xState.screen.pixels);
    }
    
    return 0;
}