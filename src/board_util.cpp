static u32
board_get_index(BoardManager *bm, s32 x, s32 y)
{
    return x + (y * bm->width);
}

static s32
board_tetromino_width(Shape shape)
{
    switch(shape)
    {
        case Shape_Square:  return 2;
        case Shape_Long:    return 1;
        case Shape_L:       return 2;
        case Shape_LM:      return 2;
        case Shape_Zig:     return 3;
        case Shape_ZigM:    return 3;
        case Shape_T:       return 3;
        
        default:            return 1;
    }
}

static s32
board_tetromino_x_offset(Shape shape)
{
    switch(shape)
    {
        case Shape_Square:  return 0;
        case Shape_Long:    return 0;
        case Shape_L:       return 0;
        case Shape_LM:      return 1;
        case Shape_Zig:     return 1;
        case Shape_ZigM:    return 1;
        case Shape_T:       return 1;
        
        default:            return 0;
    }
}

static XY
board_shape_offset(Shape shape, u32 index)
{
    // NOTE: We only handle blocks that are actually offset, all others can just pass through
    XY offset = { 0, 0 };
    
    if((shape != Shape_Long) && (index < board_tetromino_width(shape)))
    {
        switch(index)
        {
            case 0:
            {
                if((shape == Shape_LM) || (shape == Shape_Zig)) {
                    offset = { -1, 0 };
                }
                else if((shape == Shape_T) || (shape == Shape_ZigM)) {
                    offset = { -1, -1 };
                }
            } break;
            
            case 1:
            {
                if(shape == Shape_L || shape == Shape_Square) {
                    offset = { 1, 0 };
                }
            } break;
            
            case 2:
            {
                if((shape == Shape_Zig) || (shape == Shape_T)) {
                    offset = { 1, -1 };
                }
                else if(shape == Shape_ZigM) {
                    offset = { 1, 0 };
                }
            } break;
        }
    }
    
    return offset;
}