#include <cmath>
#include <iostream>
#include "Enemy.h"
#include "Game.h"
using namespace std;

void CEnemy::Init(int x, int y, ENEMY_TYPE type) {
  this->x = x; this->y = y;
  speed = 3;

  frameDuration = 0.0;
  canShoot = true;
  bulletsInGame = 0;
  turnWaitTime = (rand() % 36 + 5) / 10.0;

  if (int(type) == 3 || int(type) == 7)
    level = 4;
  else
    level = 1;

  SetEnemyType(type);
  Drive();
}

void CEnemy::Drive() {
  SetXVelocity(0); SetYVelocity(0);

  switch (direction) {
  case DIR_UP:
    vy = -speed; break;
  case DIR_DOWN:
    vy = speed; break;
  case DIR_LEFT:
    vx = -speed; break;
  case DIR_RIGHT:
    vx = speed; break;
  }
}

void CEnemy::SetEnemyType(ENEMY_TYPE type) {
  delete spriteUp, spriteDown, spriteLeft, spriteRight;
  switch (type) {
  case ENEMY_SLOW: case ENEMY_SLOW_BONUS:
    spriteUp = createSprite("..\\data\\sprites\\enemy_slow_up.png");
    spriteDown = createSprite("..\\data\\sprites\\enemy_slow_down.png");
    spriteLeft = createSprite("..\\data\\sprites\\enemy_slow_left.png");
    spriteRight = createSprite("..\\data\\sprites\\enemy_slow_right.png");
    enemyType = type;
    bulletSpeed = 8;
    bulletPower = 1;
    maxBullets = 1;
    break;
  case ENEMY_FAST: case ENEMY_FAST_BONUS:
    spriteUp = createSprite("..\\data\\sprites\\enemy_fast_up.png");
    spriteDown = createSprite("..\\data\\sprites\\enemy_fast_down.png");
    spriteLeft = createSprite("..\\data\\sprites\\enemy_fast_left.png");
    spriteRight = createSprite("..\\data\\sprites\\enemy_fast_right.png");
    enemyType = type;
    speed = 7;
    bulletSpeed = 15;
    bulletPower = 1;
    maxBullets = 1;
    break;
  case ENEMY_MEDIUM: case ENEMY_MEDIUM_BONUS:
    spriteUp = createSprite("..\\data\\sprites\\enemy_medium_up.png");
    spriteDown = createSprite("..\\data\\sprites\\enemy_medium_down.png");
    spriteLeft = createSprite("..\\data\\sprites\\enemy_medium_left.png");
    spriteRight = createSprite("..\\data\\sprites\\enemy_medium_right.png");
    enemyType = type;
    bulletSpeed = 23;
    bulletPower = 1;
    maxBullets = 1;
    break;
  case ENEMY_BIG: case ENEMY_BIG_BONUS:
    spriteUp = createSprite("..\\data\\sprites\\enemy_big_up_0.png");
    spriteDown = createSprite("..\\data\\sprites\\enemy_big_down_0.png");
    spriteLeft = createSprite("..\\data\\sprites\\enemy_big_left_0.png");
    spriteRight = createSprite("..\\data\\sprites\\enemy_big_right_0.png");
    enemyType = type;
    speed = 3;
    bulletSpeed = 15;
    bulletPower = 1;
    maxBullets = 1;
    break;
  }
}

void CEnemy::Shoot() {
  if (canShoot) {
    CGame::Get().Bullets()->CreateBullet(x, y, direction, OWN_ENEMY, id, bulletSpeed);
    ++bulletsInGame;
    if (bulletsInGame >= maxBullets) {
      canShoot = false;
    }
  }
}

void CEnemy::DecreaseBullet() {
  --bulletsInGame;
  canShoot = true;
}

void CEnemy::Draw() {
  double pos_x = x * CGame::Get().TailSize();
  double pos_y = y * CGame::Get().TailSize();

  if (enemyType == ENEMY_BIG) {
    delete spriteUp, spriteDown, spriteLeft, spriteRight;
    switch (level) {
    case 3:
      spriteUp = createSprite("..\\data\\sprites\\enemy_big_up_1.png");
      spriteDown = createSprite("..\\data\\sprites\\enemy_big_down_1.png");
      spriteLeft = createSprite("..\\data\\sprites\\enemy_big_left_1.png");
      spriteRight = createSprite("..\\data\\sprites\\enemy_big_right_1.png");
      break;
    case 2: 
      spriteUp = createSprite("..\\data\\sprites\\enemy_big_up_2.png");
      spriteDown = createSprite("..\\data\\sprites\\enemy_big_down_2.png");
      spriteLeft = createSprite("..\\data\\sprites\\enemy_big_left_2.png");
      spriteRight = createSprite("..\\data\\sprites\\enemy_big_right_2.png");
      break;
    }
  }
  if (visible) {
    switch (direction) {
    case DIR_UP:
      drawSprite(spriteUp, pos_x, pos_y); break;
    case DIR_DOWN:
      drawSprite(spriteDown, pos_x, pos_y); break;
    case DIR_LEFT:
      drawSprite(spriteLeft, pos_x, pos_y); break;
    case DIR_RIGHT:
      drawSprite(spriteRight, pos_x, pos_y); break;
    }
  }
}

void CEnemy::Update(double dt) {
  if (Type() == ENEMY_SLOW_BONUS ||
    Type() == ENEMY_FAST_BONUS ||
    Type() == ENEMY_MEDIUM_BONUS ||
    Type() == ENEMY_BIG_BONUS) {
    frameDuration += 3*dt;
    if (frameDuration > 1) {
      visible = !visible;
      frameDuration = 0;
    }
  }

  x = x + vx * dt * speedRatio;
  y = y + vy * dt * speedRatio;
  colide = false;

  // колизия с границами карты
  if (x < 0.0) { x = 0.0; colide = true; }
  if (x > 24.0) { x = 24.0; colide = true; }
  if (y < 0.0) { y = 0.0; colide = true; }
  if (y > 24.0) { y = 24.0; colide = true; }

  int pos_x_l = x, pos_x_s = x + 1, pos_x_p = x + 2;
  int pos_y_d = y, pos_y_s = y + 1, pos_y_g = y + 2;

  // колизия с блоками уровня
  if (vx < 0 && x > 0.0) {
    if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_EAGLE ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_EAGLE) {
      x = pos_x_s;
      colide = true;
    }
  }
  else if (vx > 0 && x < 24.0) {
    if (CGame::Get().Level()->LevelField(pos_x_p, pos_y_d) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_d) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_d) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_d) == LVL_EAGLE ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_s) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_s) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_s) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_p, pos_y_s) == LVL_EAGLE) {
      x = pos_x_l;
      colide = true;
    }
  }
  else if (vy < 0 && y > 0.0) {
    if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_EAGLE ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_EAGLE) {
      y = pos_y_s;
      colide = true;
    }
  }
  else if (vy > 0 && y < 24.0) {
    if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_g) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_g) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_g) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_l, pos_y_g) == LVL_EAGLE ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_g) == LVL_BRICK ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_g) == LVL_BRICK_DAMAGED ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_g) == LVL_WHITE ||
      CGame::Get().Level()->LevelField(pos_x_s, pos_y_g) == LVL_EAGLE) {
      y = pos_y_d;
      colide = true;
    }
  }

  if (canShoot) {
    currentShootTime += dt;
    if (currentShootTime >= shootWaitTime) {
      Shoot();
      currentShootTime = 0;
    }
  }

  currentTurnWaitTime += dt;
  if (currentTurnWaitTime >= turnWaitTime) {
    ChooseDirection(false);
    currentTurnWaitTime = 0;
    currentCollisionTurnTime = 0;
  }

  if (colide) {
    currentCollisionTurnTime += dt;
    if (currentCollisionTurnTime > collisionTurnTime) {
      ChooseDirection(true);
      currentCollisionTurnTime = 0;
      collisionTurnTime = (rand() % 10) / (double(speed) * 1.5);
    }
  }
}

void CEnemy::ChooseDirection(bool collision) {
  if (colide) {
    int test = rand() % 4;
    if (test == 3 || test == 1) {
      turnWaitTime = 1;
      return;
    }
    else {
      turnWaitTime = 2;
    }
  }

  int randed;
  if (int(y) < 10) {
    randed = rand() % 5;
  }
  else {
    randed = rand() % 6;
  }

  DIRECTION dir;
  if (x < 10) {
    switch (randed) {
    case 0: dir = DIR_DOWN; break;
    case 1: dir = DIR_RIGHT; break;
    case 2: dir = DIR_LEFT; break;
    case 3: dir = DIR_UP; break;
    case 4: dir = DIR_RIGHT; break;
    case 5: dir = DIR_DOWN; break;
    default: dir = DIR_DOWN; break;
    }
  }
  else if (x <= 12) {
    switch (randed) {
    case 0: dir = DIR_DOWN; break;
    case 1: dir = DIR_RIGHT; break;
    case 2: dir = DIR_LEFT; break;
    case 3: dir = DIR_UP; break;
    case 4: dir = DIR_UP; break;
    default: dir = DIR_UP; break;
    }
  }
  else {
    switch (randed) {
    case 0: dir = DIR_DOWN; break;
    case 1: dir = DIR_RIGHT; break;
    case 2: dir = DIR_LEFT; break;
    case 3: dir = DIR_UP; break;
    case 4: dir = DIR_LEFT; break;
    case 5: dir = DIR_DOWN; break;
    default: dir = DIR_DOWN; break;
    }
  }

  SetDirection(dir); Drive();
  turnWaitTime = (rand() % 31 + 5) / (double(speed) * 1.5);
}

