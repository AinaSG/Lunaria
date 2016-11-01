#ifndef INFOSCENE_H
#define INFOSCENE_H
#include "Scene.h"

class InfoScene : public Scene
{
public:
  InfoScene(const char* filename);
  ~InfoScene();

  void init();
  void update(int deltaTime);
  void render();

  Sprite* bg;
  string imageName;
};

#endif // INFOSCENE_H
