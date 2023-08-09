#define BLOCK_SIDE 120

enum Shape
{
    Shape_Square,
    Shape_Long,
    Shape_L,
    Shape_L_Flipped,
    Shape_Zig,
    Shape_Zig_Flipped,
    Shape_T,
    
    Shape_Amount
};

struct Tetromino
{
    u32 colour;
    
    u8 xPos;
    u8 yPos;
    u8 rotation;
    Shape shape;
};

struct BoardManager
{
    u32 width;
    u32 height;
    u32 size;
    
    u32 *board;
    Tetromino tetromino;
};