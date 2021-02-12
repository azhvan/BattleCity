#include "Items.h"
#include <string>;

void CItems::Draw() {
  static double tail_size = 16.0;
  if (currentItem.destroyed == false) {
    drawSprite(currentItem.sprite, currentItem.x * tail_size, currentItem.y * tail_size);
  }
}

void CItems::Update(double dt) {
  if (currentItem.destroyed == false) {
    currentItem.frame_duration += dt;
    if ((int)currentItem.frame_duration > 1) {
      currentItem.isVisible = !currentItem.isVisible;
      currentItem.frame_duration = 0;
    }
  }
}

void CItems::CreateItem() {
  currentItem.x = rand() % 21 + 2;
  currentItem.y = rand() % 21 + 2;
  currentItem.type = ITEM_TYPE(rand() % 6);
  currentItem.frame_duration = 0.0;
  currentItem.isVisible = true;
  currentItem.destroyed = false;
  std::string path = "..\\data\\sprites\\";
  switch (currentItem.type) {
  case ITEM_STAR:
    path = path + "item_star"; break;
  case ITEM_LIFE:
    path = path + "item_life"; break;
  case ITEM_INVINCIBILITY:
    path = path + "item_invincibility"; break;
  case ITEM_PROTECT:
    path = path + "item_protect"; break;
  case ITEM_TIME:
    path = path + "item_time"; break;
  case ITEM_GRANADE:
    path = path + "item_granade"; break;
  }
  path = path + ".png";
  currentItem.sprite = createSprite(path.c_str());
}