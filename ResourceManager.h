#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Texture.h"
#include <map>

template <typename T> struct Res {
  int n;
  T* ptr;

  Res<T>() : n(0), ptr(nullptr) {}
  Res<T>(T* P) : n(0), ptr(P) {}
};

template <typename T>
using ResourceMap = std::map< std::string, Res<T> >;

class ResourceManager
{
public:
  ResourceManager() {}

  const std::string TEXTURES_DIR = "images";

  static ResourceManager &instance()
  {
      static ResourceManager R;
      return R;
  }

  ResourceMap<Texture> textures;

  Texture *getTexture(const std::string &textureName);
  void freeTexture(const std::string &textureName);

};

#endif // RESOURCEMANAGER_H
