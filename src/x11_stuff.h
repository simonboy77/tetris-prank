struct Key
{
    b32 isDown;
    b32 isPressed;
    b32 isReleased;
};

static b32
operator !=(Key a, Key b)
{
    return (a.isDown != b.isDown) || (a.isReleased != b.isReleased);
}

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