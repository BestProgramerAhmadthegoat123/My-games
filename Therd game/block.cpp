#include "block.h"

Block::Block()
{
    cellSize = 30;
    rotationStat = 0;
    colors = GetCellColors();
    rowOffset = 0;
    columnOffset = 0;
}

void Block::Draw(int offsetX, int offsetY)
{
    std::vector<Position> tiles = GetCellPositions();
    for(Position item: tiles)
    {
        DrawRectangle(item.column * cellSize+offsetX, item.row * cellSize+offsetY, cellSize-1, cellSize-1, colors[id]);
    }
}

void Block::Move(int rows, int columns)
{
    rowOffset += rows;
    columnOffset += columns;
}

std::vector<Position> Block::GetCellPositions()
{
   std::vector<Position> tiles = cells[rotationStat];
   std::vector<Position> moveTiles;
   for(Position item: tiles)
   {
    Position newPos = Position(item.row + rowOffset, item.column + columnOffset);
    moveTiles.push_back(newPos);
   }
   return moveTiles;
}

void Block::Rotate()
{
    rotationStat ++;
    if(rotationStat == (int)cells.size())
    {
       rotationStat = 0;
    }
}

void Block::UndoRotation()
{
    rotationStat --;
    if(rotationStat == -1)
    {
        rotationStat = cells.size() -1;
    }
}
