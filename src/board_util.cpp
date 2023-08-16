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
    XY offset = { 0, 0 };
    
    switch(shape)
        {
            case Shape_Square:
            {
                switch(index)
                {
                    case 0: { offset = {  0, -1 }; } break; //
                    case 1: { offset = {  1, -1 }; } break; //  **
                    case 2: { offset = {  0,  0 }; } break; //  **
                    case 3: { offset = {  1,  0 }; } break; //
                }
            }; break;

            case Shape_Long:
            {
                switch(index)
                {
                    case 0: { offset = {  0, -3 }; } break; //  *
                    case 1: { offset = {  0, -2 }; } break; //  *
                    case 2: { offset = {  0, -1 }; } break; //  *
                    case 3: { offset = {  0,  0 }; } break; //  *
                }
            }; break;

            case Shape_L:
            {
                switch(index)
                {
                    case 0: { offset = {  0, -2 }; } break; //
                    case 1: { offset = {  0, -1 }; } break; //  *
                    case 2: { offset = {  0,  0 }; } break; //  *
                    case 3: { offset = {  1,  0 }; } break; //  **
                }
            }; break;

            case Shape_LM:
            {
                switch(index)
                {
                    case 0: { offset = {  0, -2 }; } break; //
                    case 1: { offset = {  0, -1 }; } break; //  *
                    case 2: { offset = {  0,  0 }; } break; //  *
                    case 3: { offset = { -1,  0 }; } break; // **
                }
            }; break;

            case Shape_Zig:
            {
                switch(index)
                {
                    case 0: { offset = { -1,  0 }; } break; //
                    case 1: { offset = {  0, -1 }; } break; //  **
                    case 2: { offset = {  0,  0 }; } break; // **
                    case 3: { offset = {  1, -1 }; } break; //
                }
            }; break;

            case Shape_ZigM:
            {
                switch(index)
                {
                    case 0: { offset = { -1, -1 }; } break; //
                    case 1: { offset = {  0, -1 }; } break; // **
                    case 2: { offset = {  0,  0 }; } break; //  **
                    case 3: { offset = {  1,  0 }; } break; //
                }
            }; break;

            case Shape_T:
            {
                switch(index)
                {
                    case 0: { offset = { -1, -1 }; } break; //
                    case 1: { offset = {  0, -1 }; } break; // ***
                    case 2: { offset = {  0,  0 }; } break; //  *
                    case 3: { offset = {  1, -1 }; } break; //
                }
            }; break;
        }
    
    return offset;
}

static void
board_invert_colours(BoardManager *bm)
{
    for(u32 blockId = 0; blockId < bm->size; ++blockId)
    {
        u32 colour = bm->board[blockId];
        if(colour & 0xFF000000) {
            bm->board[blockId] = (colour & 0xFF000000) + (~colour & 0x00FFFFFF);
        }
    }
}