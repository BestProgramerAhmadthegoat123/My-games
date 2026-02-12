#include"colors.h"
#include<raylib.h>
#include<vector>

const Color darkGray = {26, 31, 40, 255};
const Color green = {47, 230, 23, 255};
const Color red = {232, 18, 18, 255};
const Color orange = {226, 116, 17, 255};
const Color yellow = {237, 234, 4, 255};
const Color purple = {166, 0, 247, 255};
const Color cyan = {21, 204, 209, 255};
const Color blue = {13, 64, 216, 255};
const Color lighetBlue = {59, 85, 162, 255};
const Color DarkBlue = {44, 4, 127, 255};

std::vector<Color> GetCellColors()
{
    return {darkGray, green, red, orange, yellow, purple, cyan, blue};
}