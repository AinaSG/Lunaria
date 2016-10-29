#ifndef DRILLITEM_H
#define DRILLITEM_H


class DrillItem : public Item
{
public:
  DrillItem() {}
  void init(ShaderProgram &sp, int param = 0) { Item::init("taladro.png", sp); }
};

#endif // DRILLITEM_H
