#include <cmath>
#include <iostream>
#include <string>
#include "Framework.h"
#include "Player.h"
#include "Game.h"

void CPlayer::Init(int x, int y, int id) {
  this->x = x; this->y = y;
  this->id = id;
  speed = 5.5;
  canShoot = true;
  bulletsInGame = 0;
  startX = x;  startY = y;
  spritePlayerBorder = createSprite("..\\data\\sprites\\player_border.png");

  SetPlayerLevel(0);
}

void CPlayer::SetPlayerLevel(int level) {
  std::string player_num = "one";
  std::string path;
  delete spriteUp, spriteDown, spriteLeft, spriteRight;
  switch (level) {
  case 0:
    path = "..\\data\\sprites\\player_" + player_num + "_up_0.png";
    spriteUp = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_down_0.png";
    spriteDown = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_left_0.png";
    spriteLeft = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_right_0.png";
    spriteRight = createSprite(path.c_str());;

    playerLevel = 0;
    bulletSpeed = 8;
    bulletPower = 1;
    maxBullets = 1;
    break;
  case 1:
    path = "..\\data\\sprites\\player_" + player_num + "_up_1.png";
    spriteUp = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_down_1.png";
    spriteDown = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_left_1.png";
    spriteLeft = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_right_1.png";
    spriteRight = createSprite(path.c_str());;
    playerLevel = 1;
    speed = 12;
    bulletSpeed = 16;
    maxBullets = 1;
    bulletPower = 1;
    break;
  case 2:
    path = "..\\data\\sprites\\player_" + player_num + "_up_2.png";
    spriteUp = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_down_2.png";
    spriteDown = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_left_2.png";
    spriteLeft = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_right_2.png";
    spriteRight = createSprite(path.c_str());;
    playerLevel = 2;
    bulletSpeed = 16;
    maxBullets = 2;
    bulletPower = 1;
    break;
  case 3:
    path = "..\\data\\sprites\\player_" + player_num + "_up_3.png";
    spriteUp = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_down_3.png";
    spriteDown = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_left_3.png";
    spriteLeft = createSprite(path.c_str());
    path = "..\\data\\sprites\\player_" + player_num + "_right_3.png";
    spriteRight = createSprite(path.c_str());;
    playerLevel = 3;
    speed = 5.5;
    bulletSpeed = 16;
    maxBullets = 2;
    bulletPower = 2;
    break;
  }
}

void CPlayer::Drive() {
  SetStopping(false);
  SetYVelocity(0); SetXVelocity(0);
  state = PLAYER_DRIVE;
  switch (direction) {
  case DIR_UP:
    vy = -speed;
    break;
  case DIR_DOWN:
    vy = speed;
    break;
  case DIR_LEFT:
    vx = -speed;
    break;
  case DIR_RIGHT:
    vx = speed;
    break;
  }
}

void CPlayer::Stop(DIRECTION dir) {
  if (dir == DIR_LEFT || dir == DIR_RIGHT)
    SetXVelocity(0);
  else
    SetYVelocity(0);

  SetState(PLAYER_STOP);
}

void CPlayer::Shoot() {
  if (canShoot) {
    CGame::Get().Bullets()->CreateBullet(x, y, direction, OWN_PLAYER_ONE, id, bulletSpeed);
    ++bulletsInGame;
    if (bulletsInGame >= maxBullets) {
      canShoot = false;
    }
  }
}

void CPlayer::DecreaseBullet() {
  --bulletsInGame;
  if (bulletsInGame < 0) bulletsInGame = 0;
  canShoot = true;
}
void CPlayer::Draw() {
  double pos_x = x * CGame::Get().TailSize();
  double pos_y = y * CGame::Get().TailSize();

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
  if (invincibility) {
    drawSprite(spritePlayerBorder, pos_x, pos_y);
  }
}

void CPlayer::EarnStar() {
  switch (PlayerLevel()) {
  case 0: SetPlayerLevel(1); break;
  case 1: SetPlayerLevel(2); break;
  case 2: SetPlayerLevel(3); break;
  case 3: break;
  }
}

void CPlayer::EarnLife() {
  ++lifes;
}

void CPlayer::Born() {
  SetPosition(startX, startY);
  SetAlive(true);
  SetDirection(DIR_UP);
  SetState(PLAYER_STOP);
  SetXVelocity(0);
  SetYVelocity(0);
  SetInvincibility(3.0);
}

bool CPlayer::OnMove() {
  if (vx != 0 || vy != 0)
    return true;
  return false;
}

void CPlayer::Die() {
  static int TailSize = CGame::Get().TailSize();
  if (!invincibility) {
    SetPlayerLevel(0);
    LoseLife();
    SetAlive(false);
    if (Lifes() < 0)
      SetPosition(26, 26);
  }
}

void CPlayer::SetInvincibility(double time) {
  invincibility = true;
  invincibilityRemainTime = time;
}

void CPlayer::StripInvincibility() {
  invincibility = false;
  invincibilityRemainTime = 0.0;
}

void CPlayer::Update(double dt) {
  if (state == PLAYER_DRIVE && stopping) {
    Stop(stopping_dir);
  }

  x = x + vx * dt * speedRatio;
  y = y + vy * dt * speedRatio;


  if (invincibility) {
    invincibilityRemainTime -= dt;
    if (invincibilityRemainTime <= 0)    invincibility = false;
  }

  // колизия с границами карты
  if (x < 0.0)  x = 0.0;
  if (x > 24.0) x = 24.0;
  if (y < 0.0)  y = 0.0;
  if (y > 24.0) y = 24.0;

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
    }
  }
  
  // колизия с противниками
  std::list <CEnemy*> enemies = CGame::Get().Enemies()->EnemiesList(); // получаем список противников
  double x1, x2, x3, x4, y1, y2, y3, y4;

  for (std::list <CEnemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter) {
    x1 = GetX();                y1 = GetY();             // координаты игрока
    x2 = x1 + 2;                y2 = y1 + 2; 
    x3 = (*iter)->GetX();       y3 = (*iter)->GetY();    // координаты противника
    x4 = x3 + 2;                y4 = y3 + 2;

    if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
      switch (GetDirection()) {
      case DIR_UP:
        if (y1 < y3) {
          double new_y = y3 - 2; if (new_y < 0) new_y = 0;
          SetPosition(x1, new_y);
        } break;
      case DIR_DOWN:
        if (y1 > y3) {
          double new_y = y3 + 2; if (new_y > 24) new_y = 24;
          SetPosition(x1, new_y);
        } break;
      case DIR_LEFT:
        if (x1 > x3) {
          double new_x = x3 + 2; if (new_x > 24) new_x = 24;
          SetPosition(new_x, y1);
        } break;
      case DIR_RIGHT:
        if (x1 < x3) {
          double new_x = x3 - 2; if (new_x < 0) new_x = 0;
          SetPosition(new_x, y1);
        } break;
      }
    }
  } 

  // колизии с бонусами
  Item item = CGame::Get().Items()->GetCurrentItem();
  if (item.destroyed == false) {
    double x1, x2, x3, x4, y1, y2, y3, y4;
    x1 = item.x;        y1 = item.y;
    x2 = x1 + 2;        y2 = y1 + 2;
    x3 = GetX();        y3 = GetY();
    x4 = x3 + 2;        y4 = y3 + 2;

    if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
      switch (item.type) {
      case ITEM_STAR:
        EarnStar();
        break;
      case ITEM_LIFE:
        EarnLife();
        break;
      case ITEM_INVINCIBILITY:
        SetInvincibility();
        break;
      case ITEM_PROTECT:
        CGame::Get().GameTimer()->SetLevelProtect(true);
        break;
      case ITEM_TIME:
        CGame::Get().Enemies()->PauseEnemies();
        break;
      case ITEM_GRANADE:
        CGame::Get().Enemies()->DestroyAllEnemies(false);
        break;
      }
      CGame::Get().Items()->DestroyItem();
    }
  }
  
  // колизия с пулей
  std::list<Bullet> temp = CGame::Get().Bullets()->AllBullets(); // получение списка пуль

  for (std::list<Bullet>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
    if ((*iter).owner == OWN_ENEMY) {
      x1 = GetX();        y1 = GetY();     // координаты игрока
      x2 = x1 + 2;        y2 = y1 + 2;
      x3 = (*iter).x;     y3 = (*iter).y;  // координаты пули
      x4 = x3 + 0.5;      y4 = y3 + 0.5;

      if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
        Die();

        CGame::Get().Bullets()->DestroyBullet((*iter).id);
        if (CGame::Get().Enemies()->SingleEnemy((*iter).ownerId) != NULL)
          CGame::Get().Enemies()->SingleEnemy((*iter).ownerId)->DecreaseBullet();
        break;
      }
    }
  }

}
