#include "Game.h"
void CGame::StartGameplay() {
  Player()->SetPosition(8, 25);
  Player()->SetAlive(false);
  Player()->SetDirection(DIR_UP);
  Player()->SetState(PLAYER_STOP);
  Player()->SetXVelocity(0);
  Player()->SetYVelocity(0);
}
void CGame::Init() {
  Player()->Init(8, 25, 1);
  GameTimer()->Start();
  Level()->InitSprite();
  Bullets()->InitSprite();
  spriteGameOver = createSprite("..\\data\\sprites\\game_over.png");
}
void CGame::StartGame() {
  Bullets()->DestroyAllBullets();
  Enemies()->DestroyAllEnemies();
  StartGameplay();
}
void CGame::run() {  
  Update();
  Draw();
}

void CGame::Draw() {
  Level()->DrawLevel(true);
  Enemies()->Draw();  
  if (Player()->Alive())
    Player()->Draw();
  Bullets()->Draw();
  if (Items()->GetCurrentItem().destroyed == false && Items()->GetCurrentItem().isVisible)
    Items()->Draw();
  if (GameLost()) {
    drawSprite(spriteGameOver, 9 * tail_size, 12 * tail_size);
  }
}

void CGame::Update() {
  double timeStep = 1.0 / 60;
  GameTimer()->Update(timeStep);
  if (Player()->Alive())
    Player()->Update(timeStep);
  Bullets()->Update(timeStep);
  if (Items()->GetCurrentItem().destroyed == false)
    Items()->Update(timeStep);
  Enemies()->Update(timeStep);
}

void CGame::PlayerMove(FRKey k, bool released) {
  if (!released) {
    if (k == FRKey::UP && Player()->Alive() && !GameLost()) {
      Player()->SetDirection(DIR_UP);
      Player()->Drive();
    }
    else if (k == FRKey::LEFT && Player()->Alive() && !GameLost()) {
      Player()->SetDirection(DIR_LEFT);
      Player()->Drive();
    }
    else if (k == FRKey::RIGHT && Player()->Alive() && !GameLost()) {
      Player()->SetDirection(DIR_RIGHT);
      Player()->Drive();
    }
    else if (k == FRKey::DOWN && Player()->Alive() && !GameLost()) {
      Player()->SetDirection(DIR_DOWN);
      Player()->Drive();
    }
  }
  else if (released) {
    if (k == FRKey::UP)
      Player()->Stop(DIR_UP);
    else if (k == FRKey::LEFT) 
      Player()->Stop(DIR_LEFT);
    else if (k == FRKey::RIGHT) 
      Player()->Stop(DIR_RIGHT);
    else if (k == FRKey::DOWN) 
      Player()->Stop(DIR_DOWN);
  }
}

void CGame::PlayerFire() {
  if (!GameLost())
    Player()->Shoot();
}