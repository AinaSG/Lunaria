#ifndef MENUSCENE_H
#define MENUSCENE_H
#include "Scene.h"

class MenuScene : public Scene
{
public:
  MenuScene();
  ~MenuScene();

  void init();
  void update(int deltaTime);
  void render();

  Sprite* bg, *arrow;
  int menuEntry = 0;

};

#endif // MENUSCENE_H
