#pragma once
class CGameTimer {
public:
  CGameTimer() { Init(); }
  void Init();
  void Start();
  void Update(double dt);
  void SetLevelProtect(bool protect);  
private:
  int enemiesAtOnce;

  bool levelProtect;

  double currentEnemyTime;
  double currentPlayerTime;
  double currentLevelProtectTime;
  double currentRessurectionPlayer;
  double currentGameLostTime;

  double enemySpawnTime;
  double playerSpawnTime;
  double levelProtectTime;
  double ressurectionPlayer;
  double gameLostTime;
};


