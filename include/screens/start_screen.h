#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "game_window/game_screen.h"
#include <string>
#include <GL/glut.h>
#include "utils.h"

#define START_SCREEN_BG_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\starting_screen_bg.png"

class StartScreen : public GameScreen
{
private:
    float titlePulse;
    bool showPressStart;
    float blinkTimer;
    static GLuint backgroundTexture;
    static bool textureLoaded;

    static void loadTexture();
    void drawRect(float x, float y, float width, float height);
    void drawTitle();
    void drawPressStart();
    void drawBackground();

public:
    StartScreen();
    ~StartScreen();

    void init() override;
    void update(float deltaTime) override;
    void display() override;
    void handleKeyboardUp(unsigned char key, int x, int y) override;
    void handleKeyboardDown(unsigned char key, int x, int y) override;
    void handleSpecialKeysUp(int key, int x, int y) override;
    void handleSpecialKeysDown(int key, int x, int y) override;
};

#endif
