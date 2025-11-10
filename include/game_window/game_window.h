#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <GL/glut.h>
#include <string>
#include "game_window/utils.h"
#include "game_window/game_screen.h"

#define TIME_DELTA 0.0069f

class GameWindow
{
private:
    static GameWindow *instance;

    int width;
    int height;
    int windowID;
    bool isFullscreen;
    std::string title;

    int windowedWidth;
    int windowedHeight;
    int windowedPosX;
    int windowedPosY;

    GameScreen *currentScreen;

    GameWindow();
    GameWindow(const GameWindow &) = delete;
    GameWindow &operator=(const GameWindow &) = delete;

public:
    ~GameWindow();

    static GameWindow *getInstance();

    void create(int argc, char **argv, int w, int h, const char *windowTitle, bool fullscreen = false);

    void toggleFullscreen();
    void setFullscreen(bool fullscreen);
    void resize(int w, int h);
    void setTitle(const char *newTitle);

    void setScreen(GameScreen *screen);
    GameScreen *getScreen() const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getWindowID() const { return windowID; }
    bool getIsFullscreen() const { return isFullscreen; }
    float getAspectRatio() const { return static_cast<float>(width) / static_cast<float>(height); }

    void screenToNormalized(int screenX, int screenY, float &normX, float &normY) const;
    void normalizedToScreen(float normX, float normY, int &screenX, int &screenY) const;

    void run();
    static void destroy();
};

#endif
