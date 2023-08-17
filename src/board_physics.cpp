static b32
board_can_tetromino_move(BoardManager *bm, XY movement)
{
    b32 canMove = false;
    
    if(!bm->full)
    {
        canMove = true;
        for(u32 blockId = 0; canMove && (blockId < 4); ++blockId)
        {
            XY offset = board_shape_offset(bm->tetromino.shape, blockId, bm->tetromino.rotation);
            s32 destX = bm->tetromino.pos.x + offset.x + movement.x;
            s32 destY = bm->tetromino.pos.y + offset.y + movement.y;

            canMove &= (destX >= 0);
            canMove &= (destX < bm->width);
            canMove &= (destY < bm->height);

            if(destY > 0) { // Parts above board are ignored
                canMove &= (bm->board[board_get_index(bm, destX, destY)] == 0);
            }
        }
    }
    
    return canMove;
}

static b32
board_move_tetromino(BoardManager *bm, XY movement)
{
    if(board_can_tetromino_move(bm, movement)) {
        bm->tetromino.pos += movement;
        return true;
    }
    
    return false;
}

static b32
board_move_tetromino_left(BoardManager *bm)
{
    return board_move_tetromino(bm, { -1, 0 });
}

static b32
board_move_tetromino_right(BoardManager *bm)
{
    return board_move_tetromino(bm, { 1, 0 });
}

static b32
board_move_tetromino_down(BoardManager *bm)
{
    return board_move_tetromino(bm, { 0, 1 });
}

static u32
board_get_tetromino_drop_distance(BoardManager *bm)
{
    int dropDistance = 0;
    int maxDrop = (bm->height - 1) - bm->tetromino.pos.y;

    if(maxDrop > 0) {
        while(board_can_tetromino_move(bm, { 0, dropDistance + 1 }) && (dropDistance < maxDrop)) {
            dropDistance++;
        }
    }

    return dropDistance;
}

static b32
board_drop_tetromino(BoardManager *bm)
{
    int dropDistance = board_get_tetromino_drop_distance(bm);
    int maxDrop = (bm->height - 1) - bm->tetromino.pos.y;

    if(dropDistance > 0) {
        board_move_tetromino(bm, { 0, dropDistance });
        return true;
    }

    return false;
}

static b32
board_rotate_tetromino(BoardManager *bm)
{
    u32 prevRotation = bm->tetromino.rotation;
    u32 nextRotation = (prevRotation >= 3) ? 0 : (prevRotation + 1);
    bm->tetromino.rotation = nextRotation;

    if(board_can_tetromino_move(bm, { 0, 0 })) {
        return true;
    }
    else {
        bm->tetromino.rotation = prevRotation;
        return false;
    }
}