
static void
board_push_tetromino(BoardManager *bm, Image *screen, int mouseBlockX, int mouseBlockY)
{
    s32 xPos = bm->tetromino.pos.x;
    s32 yPos = bm->tetromino.pos.y;
    u32 colour = bm->tetromino.colour;
    
    // push centre block
    push_block(screen, xPos, yPos, colour, mouseBlockX, mouseBlockY);
    
    switch(bm->tetromino.shape)
    {
        case Shape_Square:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
                push_block(screen, xPos + 1, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
            
            push_block(screen, xPos + 1, yPos, colour, mouseBlockX, mouseBlockY);
        } break;
        
        case Shape_Long:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
            
            if(yPos > 1) {
                push_block(screen, xPos, yPos - 2, colour, mouseBlockX, mouseBlockY);
            }
            
            if(yPos > 2) {
                push_block(screen, xPos, yPos - 3, colour, mouseBlockX, mouseBlockY);
            }
        } break;
        
        case Shape_L:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
            
            if(yPos > 1) {
                push_block(screen, xPos, yPos - 2, colour, mouseBlockX, mouseBlockY);
            }
            
            push_block(screen, xPos + 1, yPos, colour, mouseBlockX, mouseBlockY);
        } break;
        
        case Shape_LM:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
            
            if(yPos > 1) {
                push_block(screen, xPos, yPos - 2, colour, mouseBlockX, mouseBlockY);
            }
            
            push_block(screen, xPos - 1, yPos, colour, mouseBlockX, mouseBlockY);
        } break;
        
        case Shape_Zig:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
                push_block(screen, xPos + 1, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
            
            push_block(screen, xPos - 1, yPos, colour, mouseBlockX, mouseBlockY);
        } break;
        
        case Shape_ZigM:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
                push_block(screen, xPos - 1, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
            
            push_block(screen, xPos + 1, yPos, colour, mouseBlockX, mouseBlockY);
        } break;
        
        case Shape_T:
        {
            if(yPos > 0) {
                push_block(screen, xPos, yPos - 1, colour, mouseBlockX, mouseBlockY);
                push_block(screen, xPos - 1, yPos - 1, colour, mouseBlockX, mouseBlockY);
                push_block(screen, xPos + 1, yPos - 1, colour, mouseBlockX, mouseBlockY);
            }
        } break;
        
        default: break;
    }
}

static void
board_push(BoardManager *bm, Image *screen, int mouseBlockX, int mouseBlockY)
{
    board_push_tetromino(bm, screen, mouseBlockX, mouseBlockY);
    
    for(u32 yBlock = 0; yBlock < bm->height; ++yBlock)
    {
        for(u32 xBlock = 0; xBlock < bm->width; ++xBlock)
        {
            u32 boardIndex = board_get_index(bm, xBlock, yBlock);
            u32 colour = bm->board[boardIndex];
            
            push_block(screen, xBlock, yBlock, colour, mouseBlockX, mouseBlockY);
        }
    }
}