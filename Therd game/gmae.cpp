#include "game.h"
#include<random>

Game::Game()
{
    grid = Grid();
    blocks = GeAllBlocks();
    currentBlock = GetRandomeBlock();
    nextBlock = GetRandomeBlock();
    gameOver = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sound/Music.mp3");
    PlayMusicStream(music);
}

Game::~Game()
{
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomeBlock()
{
    if(blocks.empty())
    {
        blocks = GeAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}
std::vector<Block> Game::GeAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}
void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11,11);
    switch(nextBlock.id)
    {
        case 3:
           nextBlock.Draw(255,290);
           break;
        case 4:
           nextBlock.Draw(255,280);
           break;
        default:
           nextBlock.Draw(270,270);
           break;
    }
}

void Game::HandelInput()
{
    int KeyPressed = GetKeyPressed();
    if(gameOver && KeyPressed != 0)
    {
        gameOver = false;
        Reset();
    }
    switch(KeyPressed)
    {
       case KEY_A:
       MoveBlockLeft();
       break;
       case KEY_D:
       MoveBlockRight();
       break;
       case KEY_S:
       MoveBlockDown();
       UPdateScore(0,1);
       break;
       case KEY_E:
          RoataeBlock();
          break;
    }
}

void Game::MoveBlockLeft()
{
    if(!gameOver) {
    currentBlock.Move(0,-1);
    if(IsBlockOutside()|| BlockFits() == false)
    {
        currentBlock.Move(0,1);
    }
}
}
void Game::MoveBlockRight()
{
    if(!gameOver) {
    currentBlock.Move(0,1);
    if(IsBlockOutside()|| BlockFits() == false)
    {
        currentBlock.Move(0,-1);
    }
}
}
void Game::MoveBlockDown()
{
    if(!gameOver) {
    currentBlock.Move(1,0);
    if(IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(-1,0);
        LockBlock();
    }
}
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.isCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RoataeBlock()
{
    if(!gameOver) {
    currentBlock.Rotate();
    if(IsBlockOutside()|| BlockFits() == false)
    {
        currentBlock.UndoRotation();
    }
}
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if(BlockFits() == false)
    {
         gameOver = true;
    }
    nextBlock = GetRandomeBlock();
    int rowsCleared = grid.ClearFullRows();
    UPdateScore(rowsCleared, 0);

}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.isCellEmpty(item.row, item.column) == false)
        {
            return false;
        }

    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GeAllBlocks();
    currentBlock = GetRandomeBlock();
    nextBlock = GetRandomeBlock();
    score = 0;
}

void Game::UPdateScore(int LineCleared, int moveDownPoints)
{
    switch (LineCleared)
    {
    case 1:
    score +=100;
        break;
    case 2:
    score +=300;
    break;
    case 3:
    score +=500;
    break;
    default:
        break;
    }
    score += moveDownPoints;
}
