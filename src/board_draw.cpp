
static void
board_push_tetromino(BoardManager *bm, Image *screen, int mouseBlockX, int mouseBlockY)
{
    for(u32 blockId = 0; blockId < 4; ++blockId)
    {
        XY offset = board_shape_offset(bm->tetromino.shape, blockId, bm->tetromino.rotation);
        s32 offsetX = bm->tetromino.pos.x + offset.x;
        s32 offsetY = bm->tetromino.pos.y + offset.y;

        if((offsetX >= 0) && (offsetX < bm->width) && (offsetY >= 0) && (offsetY < bm->height)) {
            push_block(screen, offsetX, offsetY, bm->tetromino.colour, mouseBlockX, mouseBlockY);
        }
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