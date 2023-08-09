void board_reset_tetromino(BoardManager *bm)
{
    bm->tetromino.xPos = rand() % bm->width;
    bm->tetromino.yPos = 0;
    bm->tetromino.shape = (Shape)(rand() % Shape_Amount);
    bm->tetromino.colour = 0xFF000000 | (rand() % 0x01000000);
}

void board_setup(BoardManager *bm, Image *screen)
{
    bm->width = screen->width / BLOCK_SIDE;
    bm->height = screen->height / BLOCK_SIDE;
    bm->size = bm->width * bm->height;
    
    bm->board = (u32*)malloc(bm->size * sizeof(u32));
    memset(bm->board, 0, bm->size * sizeof(u32));
    // bm->board[14] = 0xFF808000;
    
    board_reset_tetromino(bm);
}

u32 index_from_xyw(u32 x, u32 y, u32 width)
{
    return x + (y * width);
}

void board_push(BoardManager *bm, Image *screen)
{
    // Draw tetromino
    {
        u32 xPos = bm->tetromino.xPos * BLOCK_SIDE;
        u32 yPos = bm->tetromino.yPos * BLOCK_SIDE;
        
        draw_rect(*screen, xPos, yPos, BLOCK_SIDE, BLOCK_SIDE, bm->tetromino.colour);
    }
    
    // Draw board
    for(u32 yBlock = 0; yBlock < bm->height; ++yBlock)
    {
        u32 yPos = yBlock * BLOCK_SIDE;
        
        for(u32 xBlock = 0; xBlock < bm->width; ++xBlock)
        {
            u32 xPos = xBlock * BLOCK_SIDE;
            u32 boardIndex = index_from_xyw(xBlock, yBlock, bm->width);
            u32 colour = bm->board[boardIndex];
            
            draw_rect(*screen, xPos, yPos, BLOCK_SIDE, BLOCK_SIDE, colour);
        }
    }
}

void board_update(BoardManager *bm)
{
    b32 reachedBottom = (bm->tetromino.yPos == (bm->height - 1));
    b32 canMove = (!bm->board[index_from_xyw(bm->tetromino.xPos, bm->tetromino.yPos + 1, bm->width)]);
    
    if(canMove && !reachedBottom) {
        bm->tetromino.yPos++;
    }
    else
    {
        u32 boardIndex = index_from_xyw(bm->tetromino.xPos, bm->tetromino.yPos, bm->width);
        bm->board[boardIndex] = bm->tetromino.colour;
        
        board_reset_tetromino(bm);
    }
    
    usleep(100000);
}