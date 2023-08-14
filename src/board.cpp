#include "board_util.cpp"
#include "board_physics.cpp"
#include "board_draw.cpp"

static b32
board_spawn_tetromino(BoardManager *bm)
{    
    bm->tetromino = {};
    bm->tetromino.shape = (Shape)(rand() % Shape_Amount);
    
    s32 tetrominoWidth = board_tetromino_width(bm->tetromino.shape);
    s32 tetrominoXOffset = board_tetromino_x_offset(bm->tetromino.shape);
    
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
        //bm->tetromino.colour = 0xFF000000 | (rand() % 0x01000000);
        bm->tetromino.colour = 0x01000000 | (rand() % 0x01000000);
    }
    
    free(fitPositions);

    if(fitCount == 0) {
        printf("No spots!\n");
    }

    return (fitCount != 0);
}

static void
board_reset(BoardManager *bm)
{
    if(bm->board) { free(bm->board); }
    bm->board = (u32*)malloc(bm->size * sizeof(u32));

    memset(bm->board, 0, bm->size * sizeof(u32));
    bm->full = !board_spawn_tetromino(bm);

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
    s32 xPos = bm->tetromino.pos.x;
    s32 yPos = bm->tetromino.pos.y;
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
    
    if(rowsCleared == 4) {
        printf("Tetris!\n");
    }
    else if(rowsCleared > 0) {
        printf("Cleared %u rows!\n", rowsCleared);
    }
}

static b32
board_update(BoardManager *bm, b32 resetOnFull = false)
{
    if(!bm->full)
    {
        if(!board_move_tetromino(bm)) {
            board_place_tetromino(bm);
            board_clear_rows(bm);

            if(!board_spawn_tetromino(bm)) {
                if(resetOnFull) {
                    board_reset(bm);
                }
                else {
                    bm->full = true;
                }   
            }

            bm->full = !board_spawn_tetromino(bm);
        }
    }
    
    return !bm->full;
}