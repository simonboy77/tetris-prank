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