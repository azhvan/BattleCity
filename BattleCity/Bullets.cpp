#include "Bullets.h"
#include "Game.h"

void CBullets::DestroyAllBullets() {
  bullets.clear();
  bulletsNum = 0;
  CGame::Get().Player()->ResetBullets();
}

void CBullets::CreateBullet(double x, double y, DIRECTION dir, OWNER owner, int ownerId, double speed = 0) {
  Bullet temp_bullet;
  temp_bullet.owner = owner;
  temp_bullet.ownerId = ownerId;
  temp_bullet.destroyed = false;
  temp_bullet.id = bulletsNum + 1;
  ++bulletsNum;

  static int tile_size = CGame::Get().TailSize();

  if (speed == 0) speed = speed;

  switch (dir) {
  case DIR_UP:
    temp_bullet.x = x + (12 / double(tile_size));
    temp_bullet.y = y + (24 / double(tile_size));
    temp_bullet.xv = 0.0;
    temp_bullet.yv = speed;
    break;
  case DIR_DOWN:
    temp_bullet.x = x + (12 / double(tile_size));
    temp_bullet.y = y;
    temp_bullet.xv = 0.0;
    temp_bullet.yv = -speed;
    break;
  case DIR_LEFT:
    temp_bullet.x = x;
    temp_bullet.y = y + (12 / double(tile_size));
    temp_bullet.xv = -speed;
    temp_bullet.yv = 0.0;
    break;
  case DIR_RIGHT:
    temp_bullet.x = x + (24 / double(tile_size));
    temp_bullet.y = y + (12 / double(tile_size));
    temp_bullet.xv = speed;
    temp_bullet.yv = 0.0;
    break;
  }

  bullets.insert(bullets.end(), temp_bullet);
}

void CBullets::Draw() {
  double pos_x;
  double pos_y;
  for (std::list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end(); ++iter) {
    pos_x = (*iter).x * CGame::Get().TailSize();
    pos_y = (*iter).y * CGame::Get().TailSize();

    if ((*iter).xv < 0)
      drawSprite(spriteLeft, pos_x, pos_y);
    else if ((*iter).xv > 0)
      drawSprite(spriteRight, pos_x, pos_y);
    else if ((*iter).yv < 0) {
      pos_y += 16;
      drawSprite(spriteDown, pos_x, pos_y);
    }
    else if ((*iter).yv > 0) {
      pos_y -= 16;
      drawSprite(spriteUp, pos_x, pos_y);
    }
  }
}

void CBullets::InitSprite() {
  spriteUp = createSprite("..\\data\\sprites\\bullet_up.png");
  spriteDown = createSprite("..\\data\\sprites\\bullet_down.png");
  spriteLeft = createSprite("..\\data\\sprites\\bullet_left.png");
  spriteRight = createSprite("..\\data\\sprites\\bullet_right.png");
}

void CBullets::Update(double dt) {
  for (std::list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end(); ) {
    (*iter).x = (*iter).x + (*iter).xv * dt;
    (*iter).y = (*iter).y - (*iter).yv * dt;

    bool already_destroyed = false;
    // колизия с границами карты
    if ((*iter).x <= 0) { (*iter).x = -1; iter = DestroyBullet(iter, DIR_LEFT); already_destroyed = true; }
    else if (((*iter).x + 0.5) >= 26) { iter = DestroyBullet(iter, DIR_RIGHT); already_destroyed = true; }
    else if ((*iter).y <= 0) { (*iter).y = -1; iter = DestroyBullet(iter, DIR_DOWN); already_destroyed = true; }
    else if (((*iter).y + 0.5) >= 26) { iter = DestroyBullet(iter, DIR_UP); already_destroyed = true; }
    else {
      int pos_x_l = (*iter).x, pos_x_s = ((*iter).x + 0.5);
      int pos_y_d = (*iter).y, pos_y_s = ((*iter).y + 0.5);

      sBrick brick1, brick2;
      // колизия с блоками уровня
      if ((*iter).xv < 0 && (*iter).x > 0.0) {
        brick1 = CGame::Get().Level()->BrickField(pos_x_l, pos_y_d);
        brick2 = CGame::Get().Level()->BrickField(pos_x_l, pos_y_s);
        if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK_DAMAGED && (brick1.topRight || brick1.topLeft)) ||
          CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_WHITE ||
          CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_BRICK_DAMAGED && (brick2.bottomRight || brick2.bottomLeft)) ||
          CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_WHITE) {
          iter = DestroyBullet(iter, DIR_LEFT); already_destroyed = true;
        }
        else if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_EAGLE) {
          CGame::Get().SetGameLost(true); already_destroyed = true;
        }
      }
      else if ((*iter).xv > 0 && (*iter).x < 25.5) {
        brick1 = CGame::Get().Level()->BrickField(pos_x_s, pos_y_d);
        brick2 = CGame::Get().Level()->BrickField(pos_x_s, pos_y_s);
        if (CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_BRICK_DAMAGED && (brick1.topLeft || brick1.topRight)) ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_WHITE ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_s) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_s, pos_y_s) == LVL_BRICK_DAMAGED && (brick2.bottomLeft || brick2.bottomRight)) ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_s) == LVL_WHITE) {
          iter = DestroyBullet(iter, DIR_RIGHT); already_destroyed = true;
        }
        else if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_EAGLE) {
          CGame::Get().SetGameLost(true); already_destroyed = true;
        }
      }
      else if ((*iter).yv < 0 && (*iter).y > 0.0) {
        brick1 = CGame::Get().Level()->BrickField(pos_x_l, pos_y_d);
        brick2 = CGame::Get().Level()->BrickField(pos_x_s, pos_y_d);
        if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_BRICK_DAMAGED && (brick1.topRight || brick1.bottomRight)) ||
          CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_WHITE ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_BRICK_DAMAGED && (brick2.topLeft || brick2.bottomLeft)) ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_d) == LVL_WHITE) {
          iter = DestroyBullet(iter, DIR_DOWN); already_destroyed = true;
        }
        else if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_EAGLE) {
          CGame::Get().SetGameLost(true); already_destroyed = true;
        }
      }
      else if ((*iter).yv > 0 && (*iter).y < 25.5) {
        brick1 = CGame::Get().Level()->BrickField(pos_x_l, pos_y_s);
        brick2 = CGame::Get().Level()->BrickField(pos_x_s, pos_y_s);
        if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_BRICK_DAMAGED && (brick1.bottomRight || brick1.topRight)) ||
          CGame::Get().Level()->LevelField(pos_x_l, pos_y_s) == LVL_WHITE ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_s) == LVL_BRICK ||
          (CGame::Get().Level()->LevelField(pos_x_s, pos_y_s) == LVL_BRICK_DAMAGED && (brick2.bottomLeft || brick2.topLeft)) ||
          CGame::Get().Level()->LevelField(pos_x_s, pos_y_s) == LVL_WHITE) {
          iter = DestroyBullet(iter, DIR_UP); already_destroyed = true;
        }
        // колизия с базой (орлом)
        else if (CGame::Get().Level()->LevelField(pos_x_l, pos_y_d) == LVL_EAGLE) {
          CGame::Get().SetGameLost(true); already_destroyed = true;
        }
      }
    }

    if (!already_destroyed) {
      double x1, x2, x3, x4, y1, y2, y3, y4;
      // колизия пуль друг с другом
      for (std::list<Bullet>::iterator iter2 = bullets.begin(); iter2 != bullets.end(); ++iter2) {
        if (iter != iter2) {
          x1 = (*iter).x;     y1 = (*iter).y;   // координыты пули 1
          x2 = x1 + 0.5;      y2 = y1 + 0.5;
          x3 = (*iter2).x;    y3 = (*iter2).y;  // координаты пули 2
          x4 = x3 + 0.5;      y4 = y3 + 0.5;

          if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
            (*iter).destroyed = true;
            (*iter2).destroyed = true;
            break;
          }
        }
      }

      if ((*iter).destroyed) {
        if ((*iter).x > 12)
          iter = DestroyBullet(iter, DIR_LEFT);
        else
          iter = DestroyBullet(iter, DIR_RIGHT);
      }
      else
        ++iter;
    }
  }
}

std::list<Bullet>::iterator CBullets::DestroyBullet(std::list<Bullet>::iterator iter, DIRECTION dir) {
  static int TailSize = CGame::Get().TailSize();
  int power;

  if ((*iter).owner == OWN_PLAYER_ONE) {
    power = CGame::Get().Player()->BulletPower();
    CGame::Get().Player()->DecreaseBullet();
  }
  else {
    power = 1;
    CGame::Get().Enemies()->DecreaseBullet((*iter).ownerId);
  }

  int pos_x = (*iter).x;
  int pos_y = (*iter).y;
  switch (dir) {
  case DIR_UP:
    CGame::Get().Level()->DestroyTile(pos_x, pos_y + 1, pos_x + 1, pos_y + 1, power, dir); break;
  case DIR_DOWN:
    CGame::Get().Level()->DestroyTile(pos_x, pos_y, pos_x + 1, pos_y, power, dir); break;
  case DIR_LEFT:
    CGame::Get().Level()->DestroyTile(pos_x, pos_y, pos_x, pos_y + 1, power, dir); break;
  case DIR_RIGHT:
    CGame::Get().Level()->DestroyTile(pos_x + 1, pos_y, pos_x + 1, pos_y + 1, power, dir); break;
  }
return bullets.erase(iter);
}

void CBullets::DestroyBullet(int id) {
  for (std::list<Bullet>::iterator iter = bullets.begin(); iter != bullets.end();) {
    if ((*iter).id == id) {
      if ((*iter).owner == OWN_PLAYER_ONE)
        CGame::Get().Player()->DecreaseBullet();
      iter = bullets.erase(iter); break;
    }
    else
      ++iter;
  }
}

