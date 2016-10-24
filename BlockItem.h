#ifndef BLOCKITEM_H
#define BLOCKITEM_H
#include "Item.h"

class BlockItem : public Item
{
public:
  BlockItem();
  void init(ShaderProgram &sp);
  void use(float deltaTime);

};

#endif // BLOCKITEM_H
