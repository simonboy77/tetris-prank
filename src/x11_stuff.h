struct Image
{
    u32 width;
    u32 height;
    u32 *pixels;
};

struct XState
{
    Display *display;
    Window window;
    XImage *image;
    GC gc;
    
    Image screen;
};