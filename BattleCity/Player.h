#pragma once
#include "Types.h"
#include "Framework.h"
class string;
class CPlayer {
public:
  CPlayer() : state(PLAYER_STOP), vx(0.0), vy(0.0), speedRatio(1.0), 
    maxBullets(1), invincibility(false), lifes(2), stopping(false) { }
  void Init(int x, int y, int id);
  void Update(double dt);
  void Draw();
  void Shoot();
  void DecreaseBullet();
  void Die();
  void Born();
  void EarnStar();
  void EarnLife();
  void SetInvincibility(double time = 15.0);
  void StripInvincibility();

  int Lifes() { return lifes; }
  void LoseLife() { --lifes; }
  void SetLifes(int lifes) { this->lifes = lifes; if (lifes >= 0) SetAlive(true); }

  bool OnMove();
  bool Alive() { return alive; }
  void SetAlive(bool alive) { this->alive = alive; }
  bool Stopping() { return stopping; }
  void SetStopping(bool value) { stopping = value; }

  double GetX() { return x; }
  double GetY() { return y; }
  int Bullets() { return bulletsInGame; }
  int MaxBullets() { return maxBullets; }
  int PlayerLevel() { return playerLevel; }
  int BulletPower() { return bulletPower; }
  DIRECTION GetDirection() { return direction; }

  void SetPosition(double x, double y) { this->x = x; this->y = y; }
  void SetDirection(DIRECTION dir) { direction = dir; }
  void SetState(PLAYER_STATE state) { this->state = state; }
  void SetPlayerLevel(int level);
  void ResetBullets() { bulletsInGame = 0; canShoot = true; }

  void Drive();
  void Stop(DIRECTION dir);
  void SetXVelocity(double v) { vx = v; }
  void SetYVelocity(double v) { vy = v; }
private:
  DIRECTION direction;
  DIRECTION stopping_dir;
  PLAYER_STATE state;

  int id;
  int lifes;
  bool alive;

  int playerLevel;
  int bulletsInGame;
  int maxBullets;
  bool canShoot;

  bool invincibility;
  double invincibilityRemainTime;

  double startX;
  double startY;

  bool stopping;
  double speed;
  double x, y;
  double vx, vy;
  double speedRatio;
  double bulletSpeed;
  int bulletPower;

  Sprite* spriteUp;                     // текстура движения вверх
  Sprite* spriteDown;                   // текстура движения вниз
  Sprite* spriteLeft;                   // текстура движения влево
  Sprite* spriteRight;                  // текстура движения вправо
  Sprite* spritePlayerBorder;           // текстура щита
};

