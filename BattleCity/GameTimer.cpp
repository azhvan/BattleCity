#include "GameTimer.h"
#include "Game.h"
void CGameTimer::Init() {
  enemySpawnTime = 3;
  currentEnemyTime = 2.5;
  playerSpawnTime = 1;
  currentPlayerTime = 0;
  currentLevelProtectTime = 0;
  levelProtectTime = 15.0;
  currentRessurectionPlayer = 0;
  currentGameLostTime = 0;
  gameLostTime = 6.0;

  levelProtect = false;

  enemiesAtOnce = 4;
  ressurectionPlayer = 0.5;
}

void CGameTimer::Start() {
  CGame::Get().Bullets()->DestroyAllBullets();
  CGame::Get().Enemies()->DestroyAllEnemies();
  CGame::Get().Enemies()->UnPause();

  CGame::Get().Player()->SetInvincibility(5);
  CGame::Get().Player()->Born();
  CGame::Get().Items()->DestroyItem();
  CGame::Get().StartGameplay();
  CGame::Get().StartGame();

  Init();
}

void CGameTimer::Update(double dt) {
  static int TailSize = CGame::Get().TailSize();
  if (CGame::Get().Enemies()->AliveEnemies() < enemiesAtOnce && CGame::Get().Enemies()->Enemies() < 20) {
    currentEnemyTime += dt;
    if (currentEnemyTime >= enemySpawnTime) {
      CGame::Get().Enemies()->CreateEnemy();
      currentEnemyTime = 0;
    }
    else if (currentEnemyTime >= enemySpawnTime - 0.5) {
      int x = CGame::Get().Enemies()->NextSpawnX();
    }

  }
  else if (CGame::Get().Enemies()->Enemies() == 20 && CGame::Get().Enemies()->AliveEnemies() == 0 && CGame::Get().GameLost() == false) {
    std::cout << "\nyou win";
    system("pause");
  }

  if (CGame::Get().Player()->Lifes() < 0) {
    CGame::Get().SetGameLost(true);
  }

  if (CGame::Get().GameLost()) {
    currentGameLostTime += dt;
    if (currentGameLostTime >= gameLostTime) {
      currentGameLostTime = 0.0;
    }
  }

  if (CGame::Get().Player()->Alive() == false && CGame::Get().Player()->Lifes() >= 0) {
    currentRessurectionPlayer += dt;
    if (currentRessurectionPlayer >= ressurectionPlayer) {
      CGame::Get().Player()->Born();
      currentRessurectionPlayer = 0;
    }
  }
    
  if (levelProtect) {
    currentLevelProtectTime += dt;
    if (currentLevelProtectTime >= levelProtectTime) {
      levelProtect = false;
      SetLevelProtect(false);
      currentLevelProtectTime = 0.0;
    }

    if (currentLevelProtectTime >= levelProtectTime - 0.5)
      SetLevelProtect(true);
    else if (currentLevelProtectTime >= levelProtectTime - 1)
      SetLevelProtect(false);
    else if (currentLevelProtectTime >= levelProtectTime - 1.5)
     SetLevelProtect(true);
    else if (currentLevelProtectTime >= levelProtectTime - 2)
      SetLevelProtect(false);
    }
}

void CGameTimer::SetLevelProtect(bool protect) {
  CGame::Get().Level()->SetLevelProtect(protect);
}
