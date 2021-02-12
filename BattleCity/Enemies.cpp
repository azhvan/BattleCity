#include <iostream>
#include "Enemies.h"
#include "Enemy.h"
#include "Game.h"
#include "Bullets.h"

CEnemy* CEnemies::SingleEnemy(int id) {
  for (std::list <CEnemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter) {
    if ((*iter)->Id() == id) 
      return (*iter);
  }
  return NULL;
}

void CEnemies::CreateEnemy() {
  if (Enemies() < 20)
    CreateEnemy(enemiesType[Enemies()]);
}

int CEnemies::NextSpawnX() {
  switch (numberOfEnemies % 3) {
  case 0: return 12; break;
  case 1: return 24; break;
  case 2: return 0; break;
  }
}

void CEnemies::CreateEnemy(ENEMY_TYPE type) {
  CEnemy* temp_enemy = new CEnemy;

  int x = NextSpawnX();
  temp_enemy->SetDirection(DIR_DOWN);
  if(Enemies() == 3 || 
    Enemies() == 10 ||
    Enemies() == 17)
    switch (type)
    {
    case ENEMY_SLOW:
      type = ENEMY_SLOW_BONUS;
      break;
    case ENEMY_FAST:
      type = ENEMY_FAST_BONUS;
      break;
    case ENEMY_MEDIUM:
      type = ENEMY_MEDIUM_BONUS;
      break;
    case ENEMY_BIG:
      type = ENEMY_BIG_BONUS;
      break;
    }
  temp_enemy->Init(x, 0, type);
  temp_enemy->SetId(Enemies());

  if (int(temp_enemy->Type()) >= 4) {
    CGame::Get().Items()->DestroyItem();
  }

  enemies.push_back(temp_enemy);
  ++numberOfEnemies;
}

void CEnemies::Draw() {
  for (std::list<CEnemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter) {
    (*iter)->Draw();
  }
}

void CEnemies::DestroyAllEnemies(bool wipe) {
  static int TailSize = CGame::Get().TailSize();
  for (std::list<CEnemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter) {
    delete (*iter);
  }
  enemies.clear();
  if (wipe)
    numberOfEnemies = 0;
}

void CEnemies::DecreaseBullet(int id) {
  for (std::list<CEnemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter) {
    if ((*iter)->Id() == id) (*iter)->DecreaseBullet();
  }
}

void CEnemies::Update(double dt) {
  if (enemiesPaused) {
    currentPauseTime += dt;
    if (currentPauseTime >= pauseTime) {
      enemiesPaused = false;
      currentPauseTime = 0.0;
    }
  }

  for (std::list<CEnemy*>::iterator iter = enemies.begin(); iter != enemies.end(); ) {
    if (!enemiesPaused)
      (*iter)->Update(dt);
    
    if ((*iter)->Level() <= 0) {
      (*iter)->Destroy();
    }

    if ((*iter)->Destroyed())
      iter = DestroyEnemy(iter);
    else {
      double x1, x2, x3, x4, y1, y2, y3, y4;
      for (std::list<CEnemy*>::iterator iter2 = enemies.begin(); iter2 != enemies.end(); ++iter2) {
        if ((*iter)->Id() != (*iter2)->Id()) {
          x1 = (*iter)->GetX();       y1 = (*iter)->GetY();
          x2 = x1 + 2;                y2 = y1 + 2;
          x3 = (*iter2)->GetX();      y3 = (*iter2)->GetY();
          x4 = x3 + 2;                y4 = y3 + 2;

          if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
            bool colide = true;

            switch ((*iter)->GetDirection()) {
            case DIR_UP:
              if ((*iter2)->GetY() > (*iter)->GetY()) {
                double new_y = (*iter2)->GetY() - 2; if (new_y < 0) new_y = 0;
                (*iter)->SetPosition((*iter)->GetX(), new_y);   (*iter)->SetColide(true);
              } break;
            case DIR_DOWN:
              if ((*iter2)->GetY() < (*iter)->GetY()) {
                double new_y = (*iter2)->GetY() + 2; if (new_y > 24) new_y = 24;
                (*iter)->SetPosition((*iter)->GetX(), new_y);   (*iter)->SetColide(true);
              } break;
            case DIR_LEFT:
              if ((*iter2)->GetX() < (*iter)->GetX()) {
                double new_x = (*iter2)->GetX() + 2; if (new_x > 24) new_x = 24;
                (*iter)->SetPosition(new_x, (*iter)->GetY());   (*iter)->SetColide(true);
              } break;
            case DIR_RIGHT:
              if ((*iter2)->GetX() > (*iter)->GetX()) {
                double new_x = (*iter2)->GetX() - 2; if (new_x < 0) new_x = 0;
                (*iter)->SetPosition(new_x, (*iter)->GetY());   (*iter)->SetColide(true);
              } break;
            default: colide = false; break;
            }
          }
        }
      }

      x1 = (*iter)->GetX();                   y1 = (*iter)->GetY();
      x2 = x1 + 2;                            y2 = y1 + 2;
      x3 = CGame::Get().Player()->GetX();     y3 = CGame::Get().Player()->GetY();
      x4 = x3 + 2;                            y4 = y3 + 2;

      if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
        switch ((*iter)->GetDirection()) {
        case DIR_UP:
          if (y3 > y1) {
            double new_y = y3 - 2; if (new_y < 0) new_y = 0;
            (*iter)->SetPosition(x1, new_y);    (*iter)->SetColide(true);
          } break;
        case DIR_DOWN:
          if (y3 < y1) {
            double new_y = y3 + 2; if (new_y > 24) new_y = 24;
            (*iter)->SetPosition(x1, new_y);    (*iter)->SetColide(true);
          } break;
        case DIR_LEFT:
          if (x3 < x1) {
            double new_x = x3 + 2; if (new_x > 24) new_x = 24;
            (*iter)->SetPosition(new_x, y1);    (*iter)->SetColide(true);
          } break;
          break;
        case DIR_RIGHT:
          if (x3 > x1) {
            double new_x = x3 - 2; if (new_x < 0) new_x = 0;
            (*iter)->SetPosition(new_x, y1);    (*iter)->SetColide(true);
          } break;
        }
      }



      std::list<Bullet> temp = CGame::Get().Bullets()->AllBullets();
      for (std::list<Bullet>::iterator iter2 = temp.begin(); iter2 != temp.end(); ++iter2) {
        if ((*iter2).owner != OWN_ENEMY) {
          double x1, x2, x3, x4, y1, y2, y3, y4;
          x1 = (*iter)->GetX();       y1 = (*iter)->GetY();
          x2 = x1 + 2;                y2 = y1 + 2;
          x3 = (*iter2).x;            y3 = (*iter2).y;
          x4 = x3 + 0.5;              y4 = y3 + 0.5;

          if (TwoRectangles(x1, y1, x2, y2, x3, y3, x4, y4)) {
            (*iter)->DecreaseLevel();
            if ((*iter)->Level() == 3 && (*iter)->EnemyType() == ENEMY_BIG_BONUS) {
              (*iter)->SetEnemyType(ENEMY_BIG);
              CGame::Get().Items()->CreateItem();
            }
            CGame::Get().Bullets()->DestroyBullet((*iter2).id);
            break;
          }
        }
      }
      ++iter;
    }
  }
}

std::list<CEnemy*>::iterator CEnemies::DestroyEnemy(std::list<CEnemy*>::iterator iter) {
  if ((*iter)->Level() <= 0) {
    if (int((*iter)->Type()) >= 4) {
      CGame::Get().Items()->CreateItem();
    }
    static int TailSize = CGame::Get().TailSize();
    delete (*iter);
    return enemies.erase(iter);
  }
  else 
    return ++iter;
  
}