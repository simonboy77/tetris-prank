static void
push_rect(Image *screen, u32 x, u32 y, u32 width, u32 height, u32 colour, u32 borderPx = BORDER_PX)
{
    // NOTE(michiel): Colour is 0xAARRGGBB
    if(colour & 0xFF000000)
    {
        if ((x + width) > screen->width) {
            width = screen->width - x;
        }
        if ((y + height) > screen->height) {
            height = screen->height - y;
        }
        
        u32 borderAlpha = colour & 0xFF000000;
        u32 borderRed = (((colour & 0xFF0000) >> 16) / BORDER_DARKEN_FACTOR) << 16;
        u32 borderGreen = (((colour & 0xFF00) >> 8) / BORDER_DARKEN_FACTOR) << 8;
        u32 borderBlue = (colour & 0xFF) / BORDER_DARKEN_FACTOR;
        u32 borderColour = borderAlpha + borderRed + borderGreen + borderBlue;
        
        // TODO: inefficient as heck
        for (u32 yAt = y; yAt < y + height; ++yAt) // draw border as black background
        {
            for (u32 xAt = x; xAt < x + width; ++xAt) {
                screen->pixels[yAt * screen->width + xAt] = borderColour;
            }
        }
        
        x += borderPx;
        y += borderPx;
        
        width -= borderPx * 2;
        height -= borderPx * 2;
        
        for (u32 yAt = y; yAt < y + height; ++yAt)
        {
            for (u32 xAt = x; xAt < x + width; ++xAt) {
                screen->pixels[yAt * screen->width + xAt] = colour;
            }
        }
    }
}

static void
push_block(Image *screen, u32 x, u32 y, u32 colour, int mouseBlockX, int mouseBlockY, u32 borderPx = BORDER_PX)
{
    if((colour & 0xFF000000) &&
       (mouseBlockX >= (x - MOUSE_RADIUS)) && (mouseBlockX <= (x + MOUSE_RADIUS)) &&
       (mouseBlockY >= y - (MOUSE_RADIUS)) && (mouseBlockY <= (y + MOUSE_RADIUS))) {
        //u32 alpha = ((colour & 0xFF000000) >> 24) / 2;
        //colour = (colour & 0x00FFFFF) + (alpha << 24);
        colour = 0x80808080;
    }
    
    x *= BLOCK_SIDE;
    y *= BLOCK_SIDE;
    
    push_rect(screen, x, y, BLOCK_SIDE, BLOCK_SIDE, colour, borderPx);
}

static void
x_clear_image(XState *xState)
{
    Image *screen = &xState->screen;
    memset(screen->pixels, 0, screen->width * screen->height * sizeof(u32));
}

static b32
x_get_mouse_block(XState *xState, int *mouseBlockX, int *mouseBlockY)
{
    Window root, child;
    int winX, winY;
    u32 mask;
    
    int xBlockPrev = *mouseBlockX;
    int yBlockPrev = *mouseBlockY;
    
    XQueryPointer(xState->display, xState->window, &root, &child, mouseBlockX, mouseBlockY, &winX, &winY, &mask);
    
    *mouseBlockX /= BLOCK_SIDE;
    *mouseBlockY /= BLOCK_SIDE;
    
    return (xBlockPrev != *mouseBlockX) || (yBlockPrev != *mouseBlockY);
}

static b32
x_get_arrow_keys(XState *xState, Key *left, Key *right, Key *up, Key *down)
{
    // TODO: Check if there are keycodes
    Key leftPrev  = *left;
    Key rightPrev = *right;
    Key upPrev    = *up;
    Key downPrev  = *down;
    
    char keyStates[32];
    XQueryKeymap(xState->display, keyStates);
    
    left->isDown  = keyStates[14] & 0x02; // byte 15, bit 2
    right->isDown = keyStates[14] & 0x04; // byte 15, bit 3
    up->isDown    = keyStates[13] & 0x80; // byte 14, bit 8
    down->isDown  = keyStates[14] & 0x10; // byte 15, bit 5
    
    left->isPressed = (!leftPrev.isDown && left->isDown);
    right->isPressed = (!rightPrev.isDown && right->isDown);
    up->isPressed = (!upPrev.isDown && up->isDown);
    down->isPressed = (!downPrev.isDown && down->isDown);
    
    left->isReleased  = (leftPrev.isDown  && !left->isDown);
    right->isReleased = (rightPrev.isDown && !right->isDown);
    up->isReleased    = (upPrev.isDown    && !up->isDown);
    down->isReleased  = (downPrev.isDown  && !down->isDown);
    
    return (leftPrev != *left) || (rightPrev != *right) || (upPrev != *up) || (downPrev != *down);
}

static void
x_draw_image(XState *xState)
{
    XPutImage(xState->display, xState->window, xState->gc, xState->image, 0, 0, 0, 0, xState->screen.width, xState->screen.height);
    XSync(xState->display, False);
}

static b32
x_setup(XState *xState)
{
    b32 success = false;
    
    char *displayName = getenv("DISPLAY");
    xState->display = XOpenDisplay(displayName);
    
    if (xState->display)
    {
        u32 bitmapDepth = 32;
        int screenNum = XDefaultScreen(xState->display);
        Window rootWindow = XDefaultRootWindow(xState->display);
        
        XVisualInfo visuals;
        XMatchVisualInfo(xState->display, screenNum, bitmapDepth, TrueColor, &visuals);
        
        // NOTE(michiel): Get the total screen
        XWindowAttributes rootAttribs = {};
        XGetWindowAttributes(xState->display, rootWindow, &rootAttribs);
        
        xState->screen = {};
        xState->screen.width = (u32)rootAttribs.width;
        xState->screen.height = (u32)rootAttribs.height;
        xState->screen.pixels = (u32*)malloc(xState->screen.width * xState->screen.height * sizeof(u32));
        
        XSetWindowAttributes windowAttribs = {};
        windowAttribs.colormap = XCreateColormap(xState->display, rootWindow, visuals.visual, AllocNone);
        windowAttribs.border_pixel = BlackPixel(xState->display, screenNum);
        windowAttribs.background_pixel = BlackPixel(xState->display, screenNum);
        windowAttribs.override_redirect = 1;
        
        xState->window = XCreateWindow(xState->display, rootWindow,
                                       0, 0, xState->screen.width, xState->screen.height, 0, visuals.depth, InputOutput, visuals.visual, CWBorderPixel | CWColormap | CWBackPixel | CWOverrideRedirect, &windowAttribs);
        XMapWindow(xState->display, xState->window);
        XFlush(xState->display);
        
        // NOTE(michiel): Make sure we don't accept input
        XRectangle rect;
        XserverRegion region = XFixesCreateRegion(xState->display, &rect, 1);
        XFixesSetWindowShapeRegion(xState->display, xState->window, ShapeInput, 0, 0, region);
        XFixesDestroyRegion(xState->display, region);
        
        u32 valueMask = 0;
        XGCValues values;
        xState->gc = XCreateGC(xState->display, xState->window, valueMask, &values);
        
        if (xState->gc)
        {
            XSetForeground(xState->display, xState->gc, BlackPixel(xState->display, screenNum));
            XSetBackground(xState->display, xState->gc, BlackPixel(xState->display, screenNum));
            
            int lineWidth = 2;
            int lineStyle = LineSolid;
            int capStyle = CapButt;
            int joinStyle = JoinBevel;
            
            XSetLineAttributes(xState->display, xState->gc, lineWidth, lineStyle, capStyle, joinStyle);
            XSetFillStyle(xState->display, xState->gc, FillSolid);
            XSync(xState->display, False);
            
            for (u32 row = 0; row < xState->screen.height; ++row) {
                for (u32 col = 0; col < xState->screen.width; ++col) {
                    xState->screen.pixels[row * xState->screen.width + col] = 0x00000000;
                }
            }
            
            xState->image = XCreateImage(xState->display, visuals.visual, bitmapDepth, ZPixmap, 0, (char*)xState->screen.pixels, xState->screen.width, xState->screen.height, 32, xState->screen.width * sizeof(u32));
            
            if (xState->image)
            {
                success = true;
            }
            else
            {
                fprintf(stderr, "Couldn't create output image\n");
            }
        }
        else
        {
            fprintf(stderr, "Couldn't create Graphics Context\n");
        }
    }
    else
    {
        fprintf(stderr, "Couldn't open X11 display '%s'!\n", displayName);
    }
    
    return success;
}