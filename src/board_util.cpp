static u32
board_get_index(BoardManager *bm, s32 x, s32 y)
{
    return x + (y * bm->width);
}

static XY
board_shape_offset(Shape shape, u32 index, u32 rotation)
{
    XY offset = { 0, 0 };
    
    switch(shape)
    {
        case Shape_Square:
        {
            switch(index) // This ignores rotation
            {
                case 0: { offset = {  0, -1 }; } break; //
                case 1: { offset = {  1, -1 }; } break; //  **
                case 2: { offset = {  0,  0 }; } break; //  **
                case 3: { offset = {  1,  0 }; } break; //
            }
        }; break;

        case Shape_Long:
        {
            switch(rotation)
            {
                case 0:
                case 2:
                {
                    switch(index)
                    {
                        case 0: { offset = {  0, -2 }; } break; //  *
                        case 1: { offset = {  0, -1 }; } break; //  *
                        case 2: { offset = {  0,  0 }; } break; //  *
                        case 3: { offset = {  0,  1 }; } break; //  *
                    }
                } break;

                case 1:
                case 3:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1,  0 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; //
                        case 2: { offset = {  1,  0 }; } break; // ****
                        case 3: { offset = {  2,  0 }; } break; //
                    }
                } break;
            }
        }; break;

        case Shape_L:
        {
            switch(rotation)
            {
                case 0:
                {
                    switch(index)
                    {
                        case 0: { offset = {  0, -1 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; //  *
                        case 2: { offset = {  0,  1 }; } break; //  *
                        case 3: { offset = {  1,  1 }; } break; //  **
                    }
                } break;

                case 1:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1,  0 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; //  ***
                        case 2: { offset = {  1,  0 }; } break; //  *
                        case 3: { offset = { -1,  1 }; } break; //
                    }
                } break;

                case 2:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1, -1 }; } break; //
                        case 1: { offset = {  0, -1 }; } break; //  **
                        case 2: { offset = {  0,  0 }; } break; //   *
                        case 3: { offset = {  0,  1 }; } break; //   *
                    }
                } break;

                case 3:
                {
                    switch(index)
                    {
                        case 0: { offset = {  1, -1 }; } break; //
                        case 1: { offset = { -1,  0 }; } break; //    *
                        case 2: { offset = {  0,  0 }; } break; //  ***
                        case 3: { offset = {  1,  0 }; } break; //
                    }
                } break;
            }
        }; break;

        case Shape_LM:
        {
            switch(rotation)
            {
                case 0:
                {
                    switch(index)
                    {
                        case 0: { offset = {  0, -1 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; //   *
                        case 2: { offset = {  0,  1 }; } break; //   *
                        case 3: { offset = { -1,  1 }; } break; //  **
                    }
                } break;

                case 1:
                {
                    switch(index)
                    {

                        case 0: { offset = { -1, -1 }; } break; //
                        case 1: { offset = { -1,  0 }; } break; //  *
                        case 2: { offset = {  0,  0 }; } break; //  ***
                        case 3: { offset = {  1,  0 }; } break; //
                    }
                } break;

                case 2:
                {
                    switch(index)
                    {

                        case 0: { offset = {  0, -1 }; } break; //
                        case 1: { offset = {  1, -1 }; } break; //  **
                        case 2: { offset = {  0,  0 }; } break; //  *
                        case 3: { offset = {  0,  1 }; } break; //  *
                    }
                } break;

                case 3:
                {
                    switch(index)
                    {

                        case 0: { offset = { -1,  0 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; // ***
                        case 2: { offset = {  1,  0 }; } break; //   *
                        case 3: { offset = {  1,  1 }; } break; //
                    }
                } break;
            }
        }; break;

        case Shape_Zig:
        {
            switch(rotation)
            {
                case 0:
                case 2:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1,  0 }; } break; //
                        case 1: { offset = {  0, -1 }; } break; //  **
                        case 2: { offset = {  0,  0 }; } break; // **
                        case 3: { offset = {  1, -1 }; } break; //
                    }
                } break;

                case 1:
                case 3:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1, -2 }; } break; //
                        case 1: { offset = { -1, -1 }; } break; //  *
                        case 2: { offset = {  0, -1 }; } break; //  **
                        case 3: { offset = {  0,  0 }; } break; //   *
                    }
                } break;
            }
            

        }; break;

        case Shape_ZigM:
        {
            switch(rotation)
            {
                case 0:
                case 2:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1, -1 }; } break; //
                        case 1: { offset = {  0, -1 }; } break; // **
                        case 2: { offset = {  0,  0 }; } break; //  **
                        case 3: { offset = {  1,  0 }; } break; //
                    }
                } break;

                case 1:
                case 3:
                {
                    switch(index)
                    {
                        case 0: { offset = {  1, -2 }; } break; //
                        case 1: { offset = {  0, -1 }; } break; //   *
                        case 2: { offset = {  1, -1 }; } break; //  **
                        case 3: { offset = {  0,  0 }; } break; //  *
                    }
                } break;
            }
        }; break;

        case Shape_T:
        {
            switch(rotation)
            {
                case 0:
                {
                    switch(index)
                    {
                        case 0: { offset = { -1,  0 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; // ***
                        case 2: { offset = {  1,  0 }; } break; //  *
                        case 3: { offset = {  0,  1 }; } break; //
                    }
                } break;

                case 1:
                {
                    switch(index)
                    {
                        case 0: { offset = {  0, -1 }; } break; //
                        case 1: { offset = { -1,  0 }; } break; //  *
                        case 2: { offset = {  0,  0 }; } break; // **
                        case 3: { offset = {  0,  1 }; } break; //  *
                    }
                } break;

                case 2:
                {
                    switch(index)
                    {
                        case 0: { offset = {  0, -1 }; } break; //
                        case 1: { offset = { -1,  0 }; } break; //  *
                        case 2: { offset = {  0,  0 }; } break; // ***
                        case 3: { offset = {  1,  0 }; } break; //
                    }
                } break;

                case 3:
                {
                    switch(index)
                    {
                        case 0: { offset = {  0, -1 }; } break; //
                        case 1: { offset = {  0,  0 }; } break; //  *
                        case 2: { offset = {  1,  0 }; } break; //  **
                        case 3: { offset = {  0,  1 }; } break; //  *
                    }
                } break;
            }
        } break;
    }
    
    return offset;
}

static s32
board_tetromino_width(Shape shape, u32 rotation)
{
    s32 lowestX = 0, highestX = 0;
    for(u32 blockId = 0; blockId < 4; ++blockId)
    {
        XY offset = board_shape_offset(shape, blockId, rotation);
        lowestX  = (offset.x < lowestX)  ? offset.x : lowestX;
        highestX = (offset.x > highestX) ? offset.x : highestX;
    }

    return (highestX - lowestX) + 1;
}

static s32
board_tetromino_x_offset(Shape shape, u32 rotation)
{
    s32 lowestX = 0;
    for(u32 blockId = 0; blockId < 4; ++blockId)
    {
        XY offset = board_shape_offset(shape, blockId, rotation);
        lowestX  = (offset.x < lowestX)  ? offset.x : lowestX;
    }

    return -lowestX;
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