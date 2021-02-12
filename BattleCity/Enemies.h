#pragma once
#include <list>
#include <iostream>
#include <vector>
#include "Enemy.h"
#include "Types.h"

class CEnemies {
public:
  CEnemies() : numberOfEnemies(0), pauseTime(15), enemiesPaused(false) { enemiesType.resize(20); }
  ~CEnemies() { DestroyAllEnemies(); }

  void Update(double dt);
  void Draw();
  void CreateEnemy(ENEMY_TYPE type);
  void CreateEnemy();
  std::list<CEnemy*>::iterator DestroyEnemy(std::list<CEnemy*>::iterator iter);
  void DestroyAllEnemies(bool wipe = true);
  void DecreaseBullet(int id);

  void PauseEnemies() { enemiesPaused = true; currentPauseTime = 0; }
  void UnPause() { enemiesPaused = false; currentPauseTime = 0; }
  void SetEnemyType(int id, ENEMY_TYPE type) { enemiesType[id] = type; }
  void SetEnemiesNum(int num) { numberOfEnemies = num; }

  bool Paused() { return enemiesPaused; }
  int Enemies() { return numberOfEnemies; }
  int AliveEnemies() { return enemies.size(); }
  int NextSpawnX();

  CEnemy* SingleEnemy(int id);
  std::list<CEnemy*> EnemiesList() { return enemies; }

private:
  bool enemiesPaused;
  double currentPauseTime;
  double pauseTime;

  std::list <CEnemy*> enemies;
  std::vector <ENEMY_TYPE> enemiesType;
  int numberOfEnemies;
};