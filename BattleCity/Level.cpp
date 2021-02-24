#include "Level.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Game.h"

void CLevel::InitSprite() {
  lvlNone      = createSprite("..\\data\\sprites\\lvl_none.png");
  lvlBrick     = createSprite("..\\data\\sprites\\lvl_brick.png");
  lvlBrickNone = createSprite("..\\data\\sprites\\lvl_brick_none.png");
  lvlWhite     = createSprite("..\\data\\sprites\\lvl_white.png");
  lvlEagle     = createSprite("..\\data\\sprites\\lvl_eagle.png");
  lvlEagleLost = createSprite("..\\data\\sprites\\lvl_eagle_lose.png");
}

void CLevel::LoadLevel(const std::string& fileName) {
  std::string line;
  int j = -1;
  std::ifstream in(fileName);
  if (in.is_open()) {
    while (!in.eof()) {
      std::getline(in, line);
      j++;
      for (int i = 0; i < line.size(); i++) {
        switch (line.at(i))
        {
        case '0':
          level[j][i] = LVL_NONE;
          break;
        case '1':
          level[j][i] = LVL_BRICK;
          break;
        case '2':
          level[j][i] = LVL_WHITE;
          break;
        case '9':
          level[j][i] = LVL_EAGLE;
          break;
        default:
          level[j][i] = LVL_NONE;
          break;
        }
      }
    }
  }  
  in.close();
  levelWidth = levelHeight = j + 1;
}

void CLevel::DrawLevel(bool up) {
  
  if (up) {
    static int tile_size = CGame::Get().TailSize();
    for (int i = 0; i < levelHeight; i++)
      for (int j = 0; j < levelWidth; j++)
        switch (level[i][j])
        {
        case LVL_NONE:
          drawSprite(lvlNone, j * tile_size, i * tile_size);
          break;
        case LVL_BRICK:
          drawSprite(lvlBrick, j * tile_size, i * tile_size);
          break;
        case LVL_WHITE:
          drawSprite(lvlWhite, j * tile_size, i * tile_size);
          break;
        case LVL_EAGLE:
          drawSprite(lvlNone, j * tile_size, i * tile_size);
          break;
        case LVL_BRICK_DAMAGED:
          if (!(bricks[i][j].bottomLeft && bricks[i][j].bottomRight && bricks[i][j].topLeft && bricks[i][j].topRight)) {
            drawSprite(lvlBrick, j * tile_size, i * tile_size);
            if(bricks[i][j].bottomLeft)
              drawSprite(lvlBrickNone, j * tile_size, i * tile_size + tile_size/2);
            if (bricks[i][j].bottomRight)
              drawSprite(lvlBrickNone, j * tile_size + tile_size/2, i * tile_size + tile_size/2);
            if (bricks[i][j].topLeft)
              drawSprite(lvlBrickNone, j * tile_size, i * tile_size);
            if (bricks[i][j].topRight)
              drawSprite(lvlBrickNone, j * tile_size + tile_size/2, i * tile_size);
          }
          break;
        }
    if (!CGame::Get().GameLost())
      drawSprite(lvlEagle, 12 * 16, 24 * 16);
    else
      drawSprite(lvlEagleLost, 12 * 16, 24 * 16);
  }
}

void CLevel::DestroyTile(int x1, int y1, int x2, int y2, int power, DIRECTION dir) {
  if (!level[y1][x1] == LVL_NONE) {
    if (power == 2)
      level[y1][x1] = LVL_NONE;
    else {
      if (level[y1][x1] == LVL_BRICK || level[y1][x1] == LVL_BRICK_DAMAGED) {
        level[y1][x1] = LVL_BRICK_DAMAGED;
        switch (dir) {
        case DIR_UP:
          if (bricks[y1][x1].bottomLeft && bricks[y1][x1].bottomRight)
            level[y1][x1] = LVL_NONE;
          bricks[y1][x1].bottomLeft = true;
          bricks[y1][x1].bottomRight = true;
          break;
        case DIR_RIGHT:
          if (bricks[y1][x1].topLeft && bricks[y1][x1].bottomLeft)
            level[y1][x1] = LVL_NONE;
          bricks[y1][x1].topLeft = true;
          bricks[y1][x1].bottomLeft = true;
          break;
        case DIR_DOWN:
          if (bricks[y1][x1].topLeft && bricks[y1][x1].topRight)
            level[y1][x1] = LVL_NONE;
          bricks[y1][x1].topLeft = true;
          bricks[y1][x1].topRight = true;
          break;
        case DIR_LEFT:
          if (bricks[y1][x1].topRight && bricks[y1][x1].bottomRight)
            level[y1][x1] = LVL_NONE;
          bricks[y1][x1].topRight = true;
          bricks[y1][x1].bottomRight = true;
          break;
        }
      }
      if(level[y2][x2] == LVL_BRICK || level[y2][x2] == LVL_BRICK_DAMAGED) {
        level[y2][x2] = LVL_BRICK_DAMAGED;
        switch (dir) {
        case DIR_UP:
          if (bricks[y2][x2].bottomLeft && bricks[y2][x2].bottomRight)
            level[y2][x2] = LVL_NONE;
          bricks[y2][x2].bottomLeft = true;
          bricks[y2][x2].bottomRight = true;
          break;
        case DIR_RIGHT:
          if (bricks[y2][x2].topLeft && bricks[y2][x2].bottomLeft)
            level[y2][x2] = LVL_NONE;
          bricks[y2][x2].topLeft = true;
          bricks[y2][x2].bottomLeft = true;
          break;
        case DIR_DOWN:
          if (bricks[y2][x2].topLeft && bricks[y2][x2].topRight)
            level[y2][x2] = LVL_NONE;
          bricks[y2][x2].topLeft = true;
          bricks[y2][x2].topRight = true;
          break;
        case DIR_LEFT:
          if (bricks[y2][x2].topRight && bricks[y2][x2].bottomRight)
            level[y2][x2] = LVL_NONE;
          bricks[y2][x2].topRight = true;
          bricks[y2][x2].bottomRight = true;
          break;
        }
      }
    }
  }
}

void CLevel::DumpLevel() {
  std::cout 
    << "m_level_width - " << levelWidth << std::endl
    << "m_level_height - " << levelHeight << std::endl;

  for (int i = 0; i < levelHeight; i++) {
    for (int j = 0; j < levelWidth; j++)
      switch (level[i][j]) {
      case LVL_NONE:
        std::cout << "0";
        break;
      case LVL_BRICK:
        std::cout << "1";
        break;
      case LVL_WHITE:
        std::cout << "2";
        break;
      case LVL_BRICK_DAMAGED:
        std::cout << "6";        
        break;
      case LVL_EAGLE:
        std::cout << "9";
        break;
      }
    std::cout << std::endl;
  }
}

void CLevel::SetLevelProtect(bool protect) {
  if (protect) {
    level[23][11] = LVL_WHITE;
    level[23][12] = LVL_WHITE;
    level[23][13] = LVL_WHITE;
    level[23][14] = LVL_WHITE;
    level[24][11] = LVL_WHITE;
    level[24][14] = LVL_WHITE;
    level[25][11] = LVL_WHITE;
    level[25][14] = LVL_WHITE;
  }
  else {
    level[23][11] = LVL_BRICK;
    level[23][12] = LVL_BRICK;
    level[23][13] = LVL_BRICK;
    level[23][14] = LVL_BRICK;
    level[24][11] = LVL_BRICK;
    level[24][14] = LVL_BRICK;
    level[25][11] = LVL_BRICK;
    level[25][14] = LVL_BRICK;
  }

}
