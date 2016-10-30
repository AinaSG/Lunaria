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

};

#endif // MENUSCENE_H
