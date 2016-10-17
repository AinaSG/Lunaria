#include "ResourceManager.h"
#include <iostream>

Texture *ResourceManager::getTexture(const string &textureName)
{
  auto it = textures.find(textureName);
  if (it == textures.end()) {
    Texture* tex = new Texture();
    std::cout << TEXTURES_DIR + "/" + textureName << std::endl;
    if (!tex->loadFromFile(TEXTURES_DIR + "/" + textureName, TEXTURE_PIXEL_FORMAT_RGBA)) {
      return nullptr;
    }

    textures[textureName] = Res<Texture>(tex);
    return tex;

  } else {
    it->second.n++;
    return it->second.ptr;
  }
}

void ResourceManager::freeTexture(const string &textureName)
{
  auto it = textures.find(textureName);
  if (it != textures.end()) {
    it->second.n--;
    if (it->second.n <= 0) {
      delete[] it->second.ptr;
      textures.erase(it);
    }
  }
}
