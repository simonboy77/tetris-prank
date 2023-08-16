#include "board_util.cpp"
#include "board_physics.cpp"
#include "board_draw.cpp"

static b32
board_spawn_tetromino(BoardManager *bm, b32 randomPosition = false)
{    
    b32 spawned = false;
    
    bm->tetromino = {};
    bm->tetromino.shape = (Shape)(rand() % Shape_Amount);
    bm->tetromino.colour = 0xFF000000 | (rand() % 0x01000000);
    //bm->tetromino.colour = 0x01000000 | (rand() % 0x01000000);

    if(randomPosition)
    {
        s32 tetrominoWidth = board_tetromino_width(bm->tetromino.shape, bm->tetromino.rotation);
        s32 tetrominoXOffset = board_tetromino_x_offset(bm->tetromino.shape, bm->tetromino.rotation);
        
        u32 fitCount = 0;
        s32 *fitPositions = (s32*)malloc(bm->width * sizeof(s32));
        
        for(s32 xPos = 0; xPos < (bm->width - (tetrominoWidth - 1)); ++xPos)
        {
            b32 fit = true;
            for(s32 xSubPos = 0; fit && (xSubPos < tetrominoWidth); ++xSubPos) {
                fit &= !(bm->board[board_get_index(bm, xPos + xSubPos, 0)]);
            }
            
            if(fit) {
                fitPositions[fitCount++] = xPos + tetrominoXOffset;
            }
        }
        
        if(fitCount) {
            u32 fitIndex = rand() % fitCount;
            bm->tetromino.pos.x = fitPositions[fitIndex];
            spawned = true;
        }
        
        free(fitPositions);
    }
    else
    {
        s32 xPos = bm->width / 2;
        s32 yPos = 0;

        b32 fit = true;
        for(u32 blockId = 0; fit && (blockId < 4); ++blockId)
        {
            XY offset = board_shape_offset(bm->tetromino.shape, blockId, bm->tetromino.rotation);
            s32 offsetX = xPos + offset.x;
            s32 offsetY = yPos + offset.y;

            if(offsetY < 0) { offsetY = 0; }
            fit &= !(bm->board[board_get_index(bm, offsetX, offsetY)]);
        }

        if(fit) {
            bm->tetromino.pos.x = xPos;
            spawned = true;
        }
    }

    if(!spawned) {
        bm->tetromino.pos = { -10, -10 };
        printf("No spots!\n");
    }

    return spawned;
}

static void
board_reset(BoardManager *bm)
{
    if(bm->board) { free(bm->board); }
    bm->board = (u32*)malloc(bm->size * sizeof(u32));

    memset(bm->board, 0, bm->size * sizeof(u32));
    bm->full = !board_spawn_tetromino(bm);
    bm->score = 0;

    printf("Board reset!\n");
}

static void
board_setup(BoardManager *bm, Image *screen)
{
    bm->width = screen->width / BLOCK_SIDE;
    bm->height = screen->height / BLOCK_SIDE;
    bm->size = bm->width * bm->height;
    
    board_reset(bm);
}

static void
board_place_tetromino(BoardManager *bm)
{
    for(u32 blockId = 0; blockId < 4; ++blockId)
    {
        XY offset = board_shape_offset(bm->tetromino.shape, blockId, bm->tetromino.rotation);
        s32 offsetX = bm->tetromino.pos.x + offset.x;
        s32 offsetY = bm->tetromino.pos.y + offset.y;

        if(offsetY >= 0) {
            bm->board[board_get_index(bm, offsetX, offsetY)] = bm->tetromino.colour;
        }
    }
}

static void
board_clear_rows(BoardManager *bm)
{
    u32 rowsCleared = 0;
    for(u32 row = 0; row < bm->height;)
    {
        b32 fullRow = true;
        for(u32 col = 0; fullRow && (col < bm->width); ++col) {
            fullRow &= (bm->board[board_get_index(bm, col, row)] != 0);
        }
        
        if(fullRow) {
            if(row > 0) {
                for(u32 removeRow = row; removeRow > 0; --removeRow) {
                    for(u32 col = 0; col < bm->width; ++col)
                    {
                        u32 blockIndex = board_get_index(bm, col, removeRow);
                        u32 newBlockIndex = board_get_index(bm, col, removeRow - 1);
                        bm->board[blockIndex] = bm->board[newBlockIndex];
                    }
                }
            }
            
            // Clear top row
            memset(bm->board, 0, bm->width * sizeof(u32));
            rowsCleared++;
        }
        else {
            row++;
        }
    }
    
    if(rowsCleared > 0) {
        if(rowsCleared == 4) {
            printf("Tetris!\n");
        }
        else {
            printf("Cleared %u rows!\n", rowsCleared);
        }
        
        bm->score += rowsCleared;
    }
}

static b32
board_update(BoardManager *bm, b32 resetOnFull = false, b32 randomPosition = false)
{
    if(!bm->full)
    {
        if(!board_move_tetromino_down(bm)) {
            board_place_tetromino(bm);
            board_clear_rows(bm);

            if(!board_spawn_tetromino(bm, randomPosition)) {
                if(resetOnFull) {
                    board_reset(bm);
                }
                else {
                    bm->full = true;
                }   
            }
        }
    }
    
    return !bm->full;
}