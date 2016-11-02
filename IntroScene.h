#ifndef INTROSCENE_H
#define INTROSCENE_H
#include "Scene.h"

class IntroScene : public Scene
{
public:
  IntroScene();
  ~IntroScene();

  void init();
  void update(int deltaTime);
  void render();
private:
  Sprite* bg[5];
  float elapsedTime;
  int currentImage;
};

#endif // INTROSCENE_H
