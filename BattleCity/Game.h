#pragma once
#include <string>
#include <sstream>
#include "Framework.h"
#include "Player.h"
#include "Bullets.h"
#include "Items.h"
#include "Level.h"
#include "Enemies.h"
#include "GameTimer.h"
class CGame {
public:
  static CGame& Get() { 
    static CGame Game;
    return Game;
  }
  void StartGameplay();
  void Init();

  void run();
  void Draw();
  void Update();

  void PlayerMove(FRKey k, bool released);
  void PlayerFire();

  void StartGame();
  void SetGameLost(bool status) { game_lost = status; }
  bool GameLost() { return game_lost; }

  int TailSize() { return tail_size; }

  CPlayer* Player() { return player; }
  CBullets* Bullets() { return bullets; }
  CLevel* Level() { return level; }
  CEnemies* Enemies() { return enemies; }
  CGameTimer* GameTimer() { return gameTimer; }
  CItems* Items() { return items; }
  private:
  CGame() {
    player = new CPlayer;
    bullets = new CBullets;
    level = new CLevel;
    enemies = new CEnemies;
    gameTimer = new CGameTimer;
    items = new CItems;    
    tail_size = 16;
  }  
  ~CGame() {
    delete player;
    delete bullets;
    delete level;
    delete enemies;
    delete gameTimer;
    delete items;
  }
private:
  bool game_lost;
  int tail_size;

  Sprite* spriteGameOver;
  CPlayer* player;
  CBullets* bullets;
  CLevel* level;
  CEnemies* enemies;
  CGameTimer* gameTimer;
  CItems* items;
};

inline bool TwoRectangles(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
  if (x1 > x3) {
    std::swap(x1, x3); std::swap(y1, y3);
    std::swap(x2, x4); std::swap(y2, y4);
  }
  if (y1 > y2) std::swap(y1, y2);
  if (y3 > y4) std::swap(y3, y4);
  if (x1 > x2) std::swap(x1, x2);
  if (x3 > x4) std::swap(x3, x4);

  if (y1 > y3) { std::swap(y1, y3); std::swap(y2, y4); }
  if (y1 == y3 && y2 > y4) std::swap(y2, y4);

  if (x3 < x2 && y3 < y2) {
    return true;
  }
  return false;
}