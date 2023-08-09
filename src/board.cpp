static u32
board_get_index(BoardManager *bm, u32 x, u32 y)
{
    return x + (y * bm->width);
}

static u32
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

static u32
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

static b32
board_spawn_tetromino(BoardManager *bm)
{
    bm->tetromino = {};
    bm->tetromino.shape = (Shape)(rand() % Shape_Amount);
    
    u32 tetrominoWidth = board_tetromino_width(bm->tetromino.shape);
    u32 tetrominoXOffset = board_tetromino_x_offset(bm->tetromino.shape);
    
    u32 fitCount = 0;
    u32 *fitPositions = (u32*)malloc(bm->width * sizeof(u32));
    
    for(u32 xPos = 0; xPos < (bm->width - (tetrominoWidth - 1)); ++xPos)
    {
        b32 fit = true;
        for(u32 xSubPos = 0; fit && (xSubPos < tetrominoWidth); ++xSubPos) {
            fit &= !(bm->board[board_get_index(bm, xPos + xSubPos, 0)]);
        }
        
        if(fit) {
            fitPositions[fitCount++] = xPos + tetrominoXOffset;
        }
    }
    
    if(fitCount) {
        u32 fitIndex = rand() % fitCount;
        bm->tetromino.xPos = fitPositions[fitIndex];
        bm->tetromino.colour = 0xFF000000 | (rand() % 0x01000000);
    }
    
    free(fitPositions);
    return fitCount;
}

static void
board_setup(BoardManager *bm, Image *screen)
{
    bm->width = screen->width / BLOCK_SIDE;
    bm->height = screen->height / BLOCK_SIDE;
    bm->size = bm->width * bm->height;
    
    if(bm->board) { free(bm->board); }
    bm->board = (u32*)malloc(bm->size * sizeof(u32));
    memset(bm->board, 0, bm->size * sizeof(u32));
    
    board_spawn_tetromino(bm);
}

static void
board_push_tetromino(BoardManager *bm, Image *screen)
{
    u32 xPos = bm->tetromino.xPos;
    u32 yPos = bm->tetromino.yPos;
    u32 colour = bm->tetromino.colour;
    
    // push centre block
    push_block(screen, xPos, yPos, colour);
    
    switch(bm->tetromino.shape)
    {
        case Shape_Square:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
                push_block(screen, xPos + 1, yPos - 1, colour);
            }
            
            push_block(screen, xPos + 1, yPos, colour);
        } break;
        
        case Shape_Long:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
            }
            
            if(yPos > 1) {
                push_block(screen, xPos, yPos - 2, colour);
            }
            
            if(yPos > 2) {
                push_block(screen, xPos, yPos - 3, colour);
            }
        } break;
        
        case Shape_L:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
            }
            
            if(yPos > 1) {
                push_block(screen, xPos, yPos - 2, colour);
            }
            
            push_block(screen, xPos + 1, yPos, colour);
        } break;
        
        case Shape_LM:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
            }
            
            if(yPos > 1) {
                push_block(screen, xPos, yPos - 2, colour);
            }
            
            push_block(screen, xPos - 1, yPos, colour);
        } break;
        
        case Shape_Zig:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
                push_block(screen, xPos + 1, yPos - 1, colour);
            }
            
            push_block(screen, xPos - 1, yPos, colour);
        } break;
        
        case Shape_ZigM:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
                push_block(screen, xPos - 1, yPos - 1, colour);
            }
            
            push_block(screen, xPos + 1, yPos, colour);
        } break;
        
        case Shape_T:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour);
                push_block(screen, xPos - 1, yPos - 1, colour);
                push_block(screen, xPos + 1, yPos - 1, colour);
            }
        } break;
        
        default: break;
    }
}

static void
board_push(BoardManager *bm, Image *screen)
{
    board_push_tetromino(bm, screen);
    
    for(u32 yBlock = 0; yBlock < bm->height; ++yBlock)
    {
        for(u32 xBlock = 0; xBlock < bm->width; ++xBlock)
        {
            u32 boardIndex = board_get_index(bm, xBlock, yBlock);
            u32 colour = bm->board[boardIndex];
            
            push_block(screen, xBlock, yBlock, colour);
        }
    }
}

static b32
board_move_tetromino(BoardManager *bm)
{
    u32 xPos = bm->tetromino.xPos;
    u32 yPos = bm->tetromino.yPos;
    
    b32 reachedBottom = (yPos == (bm->height - 1));
    b32 canMove = (!bm->board[board_get_index(bm, xPos, yPos + 1)]) && !reachedBottom;
    
    if(canMove)
    {
        switch(bm->tetromino.shape)
        {
            case Shape_Square: {
                canMove &= (!bm->board[board_get_index(bm, xPos + 1, yPos + 1)]);
            } break;
            
            case Shape_Long: {} break; // Doesn't need any extra checking unless rotated
            
            case Shape_L: {
                canMove &= (!bm->board[board_get_index(bm, xPos + 1, yPos + 1)]);
            } break;
            
            case Shape_LM: {
                canMove &= (!bm->board[board_get_index(bm, xPos - 1, yPos + 1)]);
            } break;
            
            case Shape_Zig: {
                canMove &= (!bm->board[board_get_index(bm, xPos - 1, yPos + 1)]);
                canMove &= (!bm->board[board_get_index(bm, xPos + 1, yPos)]);
            } break;
            
            case Shape_ZigM: {
                canMove &= (!bm->board[board_get_index(bm, xPos - 1, yPos)]);
                canMove &= (!bm->board[board_get_index(bm, xPos + 1, yPos + 1)]);
            } break;
            
            case Shape_T: {
                canMove &= (!bm->board[board_get_index(bm, xPos - 1, yPos)]);
                canMove &= (!bm->board[board_get_index(bm, xPos + 1, yPos)]);
            } break;
            
            default: break;
        }
    }
    
    if(canMove) {
        bm->tetromino.yPos++;
    }
    
    return canMove;
}

static void
board_place_tetromino(BoardManager *bm)
{
    u32 xPos = bm->tetromino.xPos;
    u32 yPos = bm->tetromino.yPos;
    u32 colour = bm->tetromino.colour;
    
    bm->board[board_get_index(bm, xPos, yPos)] = colour;
    
    switch(bm->tetromino.shape)
    {
        case Shape_Square:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
                bm->board[board_get_index(bm, xPos + 1, yPos - 1)] = colour;
            }
            
            bm->board[board_get_index(bm, xPos + 1, yPos)] = colour;
        } break;
        
        case Shape_Long:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
            }
            
            if(yPos > 1) {
                bm->board[board_get_index(bm, xPos, yPos - 2)] = colour;
            }
            
            if(yPos > 2) {
                bm->board[board_get_index(bm, xPos, yPos - 3)] = colour;
            }
        }
        break;
        
        case Shape_L:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
            }
            
            if(yPos > 1) {
                bm->board[board_get_index(bm, xPos, yPos - 2)] = colour;
            }
            
            bm->board[board_get_index(bm, xPos + 1, yPos)] = colour;
        } break;
        
        case Shape_LM:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
            }
            
            if(yPos > 1) {
                bm->board[board_get_index(bm, xPos, yPos - 2)] = colour;
            }
            
            bm->board[board_get_index(bm, xPos - 1, yPos)] = colour;
        } break;
        
        case Shape_Zig:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
                bm->board[board_get_index(bm, xPos + 1, yPos - 1)] = colour;
            }
            
            bm->board[board_get_index(bm, xPos - 1, yPos)] = colour;
        } break;
        
        case Shape_ZigM:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
                bm->board[board_get_index(bm, xPos - 1, yPos - 1)] = colour;
            }
            
            bm->board[board_get_index(bm, xPos + 1, yPos)] = colour;
        } break;
        
        case Shape_T:
        {
            if(yPos > 0) {
                bm->board[board_get_index(bm, xPos, yPos - 1)] = colour;
                bm->board[board_get_index(bm, xPos - 1, yPos - 1)] = colour;
                bm->board[board_get_index(bm, xPos + 1, yPos - 1)] = colour;
            }
        } break;
        
        default: break;
    }
}

static b32
board_update(BoardManager *bm)
{
    b32 running = true;
    
    if(!board_move_tetromino(bm)) {
        board_place_tetromino(bm);
        running = board_spawn_tetromino(bm);
    }
    
    usleep(100000);
    return running;
}