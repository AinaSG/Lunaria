#ifndef EMPTYITEM_H
#define EMPTYITEM_H
#include "Item.h"
#include "ShaderProgram.h"

class EmptyItem : public Item
{
public:
  EmptyItem() : Item() {}
  void init(ShaderProgram &sp, int param = 0) override {
    amount = -1;
    type = "Empty";
  }
};

#endif // EMPTYITEM_H
