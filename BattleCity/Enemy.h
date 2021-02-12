#pragma once
#include "Types.h"
#include "Framework.h"

class CEnemy {
public:
  CEnemy() : vx(0.0), vy(0.0), speedRatio(1.0), maxBullets(1), direction(DIR_DOWN),
    currentShootTime(0.2), shootWaitTime(0.7), currentTurnWaitTime(0.0),
    turnWaitTime(2.0), destroyed(false), currentCollisionTurnTime(0.0), collisionTurnTime(1), visible(true) { }
  void Init(int x, int y, ENEMY_TYPE type);

  void Update(double dt);
  void Draw();

  void Drive();
  void ChooseDirection(bool collision);

  void Shoot();
  void DecreaseBullet();
  void DecreaseLevel() { --level; }
  int Level() { return level; }

  int Id() { return id; }
  void SetId(int id) { this->id = id; }
  void SetColide(bool colide) { this->colide = colide; }
  void Destroy() { destroyed = true; }

  bool Destroyed() { return destroyed; }
  double GetX() { return x; }
  double GetY() { return y; }
  int Bullets() { return bulletsInGame; }
  int MaxBullets() { return maxBullets; }
  int EnemyType() { return enemyType; }
  int BulletPower() { return bulletPower; }
  DIRECTION GetDirection() { return direction; }
  ENEMY_TYPE Type() { return enemyType; }

  void SetEnemyType(ENEMY_TYPE type);
  void SetPosition(double x, double y) { this->x = x; this->y = y; }
  void SetDirection(DIRECTION dir) { direction = dir; }

  void SetXVelocity(double v) { vx = v; }
  void SetYVelocity(double v) { vy = v; }
private:
  int id;
  DIRECTION direction;
  ENEMY_TYPE enemyType;

  double frameDuration;
  bool visible;

  int bulletsInGame;
  int maxBullets;
  bool canShoot;
  bool destroyed;
  bool colide;
  int level;

  double currentShootTime;
  double shootWaitTime;

  double currentTurnWaitTime;
  double turnWaitTime;

  double currentCollisionTurnTime;
  double collisionTurnTime;

  double speed;
  double x, y;
  double vx, vy;
  double speedRatio;
  double bulletSpeed;
  int bulletPower;

  Sprite* spriteUp;
  Sprite* spriteDown;
  Sprite* spriteLeft;
  Sprite* spriteRight;
};

