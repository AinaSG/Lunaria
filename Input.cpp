#include "Input.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

void Input::update(){
  std::copy(std::begin(keys), std::end(keys), std::begin(lastKeys));
  std::copy(std::begin(specialKeys), std::end(specialKeys), std::begin(lastSpecialKeys));
  std::copy(std::begin(mouseDown), std::end(mouseDown), std::begin(lastMouseDown));
}

void Input::keyPressed(int key)
{
    keys[key] = true;
}

void Input::keyReleased(int key)
{
    keys[key] = false;
}

void Input::specialKeyPressed(int key)
{
    specialKeys[key] = true;
}

void Input::specialKeyReleased(int key)
{
    specialKeys[key] = false;
}

void Input::mouseMove(int x, int y)
{
    mousePos = glm::ivec2(x,y);
}

void Input::mousePress(int button)
{
    mouseDown[button] = true;
}

void Input::mouseRelease(int button)
{
   mouseDown[button] = false;
}


bool Input::getKey(int key) const { return keys[key]; }
bool Input::getSpecialKey(int key) const { return specialKeys[key]; }
bool Input::getMouseButton(int button) const { return mouseDown[button]; }

bool Input::getKeyDown(int key) const { return keys[key] && !lastKeys[key]; }
bool Input::getSpecialKeyDown(int key) const { return specialKeys[key] && !lastSpecialKeys[key]; }
bool Input::getMouseButtonDown(int button) const { return mouseDown[button] && !lastMouseDown[button]; }

bool Input::getKeyHold(int key) const { return keys[key] && lastKeys[key]; }
bool Input::getSpecialKeyHold(int key) const { return specialKeys[key] && lastSpecialKeys[key]; }
bool Input::getMouseButtonHold(int button) const { return mouseDown[button] && lastMouseDown[button]; }

glm::ivec2 Input::getMouseScreenPos()
{
    return mousePos;
}
