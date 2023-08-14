enum Shape
{
    Shape_Square,
    Shape_Long,
    Shape_L,
    Shape_LM,   // Mirrored
    Shape_Zig,
    Shape_ZigM, // Mirrored
    Shape_T,
    
    Shape_Amount
};

struct XY
{
    s32 x;
    s32 y;
};

static XY
operator -(XY a, XY b)
{
    XY outXY;
    outXY.x = a.x - b.x;
    outXY.y = a.y - b.y;
    
    return outXY;
}

struct Tetromino
{
    XY pos;
    Shape shape;
    u8 rotation;
    u32 colour;
};

struct BoardManager
{
    s32 width;
    s32 height;
    u32 size;
    
    b32 full;
    u32 *board;
    Tetromino tetromino;
};