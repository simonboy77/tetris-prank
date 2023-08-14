static b32
board_drop_tetromino(BoardManager *bm)
{
    
    
    return false;
}

static b32
board_move_tetromino(BoardManager *bm)
{
    b32 canMove = false;
    
    if(!bm->full)
    {
        s32 xPos = bm->tetromino.pos.x;
        s32 yPos = bm->tetromino.pos.y;
        Shape shape = bm->tetromino.shape;
        
        b32 reachedBottom = (yPos == (bm->height - 1));
        canMove = !reachedBottom;
        
        for(u32 blockId = 0; canMove && (blockId < board_tetromino_width(shape)); ++blockId)
        {
            XY offset = board_shape_offset(shape, blockId);
            canMove &= (bm->board[board_get_index(bm, (xPos + offset.x), (yPos + offset.y + 1))] == 0);
        }
    }
    
    if(canMove) {
        bm->tetromino.pos.y++;
    }
    
    return canMove;
}

static b32
board_move_tetromino_old(BoardManager *bm)
{
    s32 xPos = bm->tetromino.pos.x;
    s32 yPos = bm->tetromino.pos.y;
    
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
        bm->tetromino.pos.y++;
    }
    
    return canMove;
}