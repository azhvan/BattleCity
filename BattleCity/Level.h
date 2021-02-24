#pragma once
#include <fstream>
#include "Types.h"
#include "Framework.h"

class string;

struct sBrick {
  sBrick() : topLeft(false), topRight(false), bottomLeft(false), bottomRight(false) { }
  bool topLeft;
  bool topRight;
  bool bottomLeft;
  bool bottomRight;
};

class CLevel {
public:
  CLevel() :levelWidth(26){ LoadLevel("..\\data\\level\\1.txt"); }
  void InitSprite();
  void LoadLevel(const std::string &fileName);
  void DrawLevel(bool up);
  void DestroyTile(int x1, int y1, int x2, int y2, int power, DIRECTION dir); // расчет урона блоку

  void DumpLevel();

  int LevelWidth() { return levelWidth; }
  int LevelHeight() { return levelHeight; }

  LVL_FIELD LevelField(int x, int y) { return level[y][x]; }  // возвращаем какая текстура на координатах x, y
  sBrick BrickField(int x, int y) { return bricks[y][x]; }    // возвращаем целосность текстури на координатах x, y

  void SetLevelProtect(bool protect);
  
private:
  int levelWidth;
  int levelHeight;

  Sprite* lvlNone;
  Sprite* lvlBrick;
  Sprite* lvlBrickNone;
  Sprite* lvlWhite;
  Sprite* lvlEagle;
  Sprite* lvlEagleLost;

  LVL_FIELD level[26][26];  // уровень
  sBrick bricks[26][26];    // целосность кажного блока уровня
};