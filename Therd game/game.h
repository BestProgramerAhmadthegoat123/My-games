#pragma once
#include "grid.h"
#include "blocks.cpp"
class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandelInput();
    void MoveBlockDown();
    bool gameOver;
    int score;
    Music music;
    
private:
    void MoveBlockLeft();
    void MoveBlockRight();
    bool IsBlockOutside();
    Block GetRandomeBlock();
    std::vector<Block> GeAllBlocks();
    void RoataeBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UPdateScore(int LineCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Sound roatetSound;
    Sound clearSound;
};