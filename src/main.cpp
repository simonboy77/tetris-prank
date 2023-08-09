// https://tronche.com/gui/x/xlib/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring> // memset
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

int main(int argCount, char **arguments)
{
    time_t t;
    srand(time(&t)); // Random seed
    
    XState xState = {};
    if(x_setup(&xState))
    {
        BoardManager bm;
        board_setup(&bm, &xState.screen);
        
        b32 isRunning = true;
        while (isRunning)
        {
            x_clear_image(&xState);
            board_push(&bm, &xState.screen);
            x_draw_image(&xState);
            
            isRunning = board_update(&bm);
        }
        
        printf("It's full\n");
        while(true); // Keep that screen 
        
        free(xState.screen.pixels);
    }
    
    return 0;
}