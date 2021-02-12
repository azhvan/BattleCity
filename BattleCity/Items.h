#pragma once
#include "Types.h"
#include "Framework.h"

struct Item {
  int x, y;
  double frame_duration;
  bool isVisible, destroyed;
  ITEM_TYPE type;
  Sprite* sprite;
};

class CItems {
public:
  CItems() { DestroyItem(); }
  void CreateItem();
  void DestroyItem() { currentItem.destroyed = true; }
  void Draw();
  void Update(double dt);

  Item GetCurrentItem() { return currentItem; }
private:
  Item currentItem;
};

