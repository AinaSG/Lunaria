#ifndef DIAMONDITEM_H
#define DIAMONDITEM_H
#include "Item.h"

class DiamondItem : public Item
{
public:
  DiamondItem();
  void init(ShaderProgram &sp);
  void use(float deltaTime);
};

#endif // DIAMONDITEM_H
