#ifndef BLOCKITEM_H
#define BLOCKITEM_H
#include "Item.h"

class BlockItem : public Item
{
private:
  int blockID;
  const string filenames[3] = { "moonground.png", "moondiamonds.png", "moonrubies.png" };

public:
  BlockItem();
  void init(ShaderProgram &sp, int param = 0);
  void use(float deltaTime);
  int getBlockID() { return blockID; }

};

#endif // BLOCKITEM_H
