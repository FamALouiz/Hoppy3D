#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <GL/glut.h>

class GameScreen
{
private:
    bool isActive;

public:
    GameScreen();
    virtual ~GameScreen();

    virtual void init();
    virtual void update(float deltaTime);
    virtual void display();
    virtual void handleKeyboardUp(unsigned char key, int x, int y);
    virtual void handleSpecialKeysUp(int key, int x, int y);
    virtual void handleKeyboardDown(unsigned char key, int x, int y);
    virtual void handleSpecialKeysDown(int key, int x, int y);
    virtual void handleMouse(int button, int state, int x, int y);
    virtual void handleMouseMotion(int x, int y);

    void setActive(bool active);
    bool getActive() const;
};

#endif
