#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Input.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <sstream>
#include "BlockItem.h"
#include "ResourceManager.h"
#include <algorithm>

void Scene::init()
{
  projection = glm::ortho(0.f, float(Game::screenSize.x - 1), float(Game::screenSize.y - 1), 0.f);
  cameraPos = Game::halfScreenSize;
  initShaders();
}

void Scene::initShaders()
{
  Shader vShader, fShader;

  vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
  if(!vShader.isCompiled())
  {
    cout << "Vertex Shader Error" << endl;
    cout << "" << vShader.log() << endl << endl;
  }
  fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
  if(!fShader.isCompiled())
  {
    cout << "Fragment Shader Error" << endl;
    cout << "" << fShader.log() << endl << endl;
  }
  texProgram.init();
  texProgram.addShader(vShader);
  texProgram.addShader(fShader);
  texProgram.link();
  if(!texProgram.isLinked())
  {
    cout << "Shader Linking Error" << endl;
    cout << "" << texProgram.log() << endl << endl;
  }
  texProgram.bindFragmentOutput("outColor");
  vShader.free();
  fShader.free();
}

glm::ivec2 Scene::worldToScreen(const glm::ivec2 &p) { return p - (cameraPos - Game::halfScreenSize); }
glm::ivec2 Scene::screenToWorld(const glm::ivec2 &p) { return p + (cameraPos - Game::halfScreenSize); }
