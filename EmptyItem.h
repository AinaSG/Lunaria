#ifndef EMPTYITEM_H
#define EMPTYITEM_H
#include "Item.h"
#include "ShaderProgram.h"

class EmptyItem : public Item
{
public:
  EmptyItem() : Item() {}
  void init(ShaderProgram &sp) override { }
  void use(float deltaTime) override;
};

#endif // EMPTYITEM_H
