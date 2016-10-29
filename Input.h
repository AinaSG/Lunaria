#ifndef INPUT_H
#define INPUT_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>


class Input
{
public:

    Input() {}


    static Input &instance()
    {
        static Input I;
        return I;
    }

    void update();

    // Input callback methods
    void keyPressed(int key);
    void keyReleased(int key);
    void specialKeyPressed(int key);
    void specialKeyReleased(int key);
    void mouseMove(int x, int y);
    void mousePress(int button);
    void mouseRelease(int button);
    void mouseWheel(int button, int state);

    void setMousePosition(glm::ivec2 pos);

    //Getters
    bool getKey(int key) const;
    bool getSpecialKey(int key) const;
    bool getMouseButton(int button) const;

    bool getKeyDown(int key) const;
    bool getSpecialKeyDown(int key) const;
    bool getMouseButtonDown(int button) const;

    bool getKeyHold(int key) const;
    bool getSpecialKeyHold(int key) const;
    bool getMouseButtonHold(int button) const;
    int getMouseWheel() const;

    glm::ivec2 getMouseScreenPos();

private:
    bool lastMouseDown[3], mouseDown[3]; //GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, and GLUT_RIGHT_BUTTON
    int mouseWheelState;
    glm::ivec2 mousePos;
    bool keys[256], lastKeys[256], specialKeys[256], lastSpecialKeys[256];
        // Store key states so that
        // we can have access at any time
};

#endif // INPUT_H
