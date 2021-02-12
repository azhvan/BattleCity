#pragma once
#include <list>
#include "Types.h"
#include "Framework.h"
struct Bullet {
  Bullet() {}
  Bullet(double x, double y, OWNER owner) : x(x), y(y), owner(owner) { }

  int id;
  double x, y;
  double xv, yv;
  OWNER owner;
  int ownerId;
  bool destroyed;
};

class CBullets {
public:
  CBullets() : speed(10), bulletsNum(0) { }
  void InitSprite();
  void Update(double dt);
  void Draw();
  void CreateBullet(double x, double y, DIRECTION dir,
    OWNER owner, int owner_id, double speed);

  std::list<Bullet>::iterator DestroyBullet(
    std::list<Bullet>::iterator iter, DIRECTION dir);

  void DestroyBullet(int id);
  void DestroyAllBullets();

  int BulletsNum() { return bullets.size(); }
  std::list <Bullet> AllBullets() { return bullets; }

private:
  std::list <Bullet> bullets;
  double speed;
  int bulletsNum;

  Sprite* spriteUp;
  Sprite* spriteDown;
  Sprite* spriteLeft;
  Sprite* spriteRight;
};
