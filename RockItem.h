#ifndef BLOCKITEM_H
#define BLOCKITEM_H
#include "Item.h"

class RockItem : public Item
{
public:
  RockItem();
  void init(ShaderProgram &sp);
  void use(float deltaTime);

};

#endif // BLOCKITEM_H
