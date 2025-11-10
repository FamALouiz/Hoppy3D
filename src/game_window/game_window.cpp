#include "game_window/game_window.h"
#include <stdio.h>

GameWindow *GameWindow::instance = nullptr;

static void displayWrapper();
static void keyboardDownWrapper(unsigned char key, int x, int y);
static void keyboardUpWrapper(unsigned char key, int x, int y);
static void specialDownWrapper(int key, int x, int y);
static void specialUpWrapper(int key, int x, int y);
static void mouseWrapper(int button, int state, int x, int y);
static void motionWrapper(int x, int y);
static void timerWrapper(int value);

GameWindow::GameWindow()
    : width(800), height(600), windowID(-1), isFullscreen(true),
      title("GLUT Window"), windowedWidth(800), windowedHeight(600),
      windowedPosX(100), windowedPosY(100), currentScreen(nullptr)
{
}

GameWindow::~GameWindow()
{
    if (currentScreen != nullptr)
    {
        delete currentScreen;
        currentScreen = nullptr;
    }

    if (windowID != -1)
    {
        glutDestroyWindow(windowID);
    }
}

GameWindow *GameWindow::getInstance()
{
    if (instance == nullptr)
    {
        instance = new GameWindow();
    }
    return instance;
}

void GameWindow::create(int argc, char **argv, int w, int h, const char *windowTitle, bool fullscreen)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    title = windowTitle;

    if (fullscreen)
    {
        width = glutGet(GLUT_SCREEN_WIDTH);
        height = glutGet(GLUT_SCREEN_HEIGHT);
        isFullscreen = true;

        windowedWidth = w;
        windowedHeight = h;

        glutInitWindowPosition(0, 0);
        glutInitWindowSize(width, height);
        windowID = glutCreateWindow(title.c_str());
        glutFullScreen();

        printf("Window created in fullscreen mode: %dx%d\n", width, height);
    }
    else
    {
        width = w;
        height = h;
        windowedWidth = w;
        windowedHeight = h;
        isFullscreen = false;

        int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
        int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
        windowedPosX = (screenWidth - width) / 2;
        windowedPosY = (screenHeight - height) / 2;

        glutInitWindowPosition(windowedPosX, windowedPosY);
        glutInitWindowSize(width, height);
        windowID = glutCreateWindow(title.c_str());

        printf("Window created in windowed mode: %dx%d\n", width, height);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameWindow::toggleFullscreen()
{
    setFullscreen(!isFullscreen);
}

void GameWindow::setFullscreen(bool fullscreen)
{
    if (fullscreen == isFullscreen)
    {
        return;
    }

    if (fullscreen)
    {
        windowedPosX = glutGet(GLUT_WINDOW_X);
        windowedPosY = glutGet(GLUT_WINDOW_Y);
        windowedWidth = width;
        windowedHeight = height;

        glutFullScreen();
        width = glutGet(GLUT_SCREEN_WIDTH);
        height = glutGet(GLUT_SCREEN_HEIGHT);
        isFullscreen = true;

        printf("Switched to fullscreen: %dx%d\n", width, height);
    }
    else
    {
        glutReshapeWindow(windowedWidth, windowedHeight);
        glutPositionWindow(windowedPosX, windowedPosY);
        width = windowedWidth;
        height = windowedHeight;
        isFullscreen = false;

        printf("Switched to windowed mode: %dx%d\n", width, height);
    }
}

void GameWindow::resize(int w, int h)
{
    if (!isFullscreen)
    {
        width = w;
        height = h;
        windowedWidth = w;
        windowedHeight = h;
        glutReshapeWindow(w, h);
        printf("Window resized to: %dx%d\n", w, h);
    }
}

void GameWindow::setTitle(const char *newTitle)
{
    title = newTitle;
    glutSetWindowTitle(newTitle);
}

void GameWindow::screenToNormalized(int screenX, int screenY, float &normX, float &normY) const
{
    normX = (2.0f * screenX) / width - 1.0f;
    normY = 1.0f - (2.0f * screenY) / height;
}

void GameWindow::normalizedToScreen(float normX, float normY, int &screenX, int &screenY) const
{
    screenX = static_cast<int>((normX + 1.0f) * width / 2.0f);
    screenY = static_cast<int>((1.0f - normY) * height / 2.0f);
}

void GameWindow::setScreen(GameScreen *screen)
{
    if (currentScreen != nullptr)
    {
        currentScreen->setActive(false);
    }

    currentScreen = screen;

    if (currentScreen != nullptr)
    {
        currentScreen->setActive(true);
        currentScreen->init();
    }
}

GameScreen *GameWindow::getScreen() const
{
    return currentScreen;
}

void GameWindow::run()
{
    printf("Starting main loop...\n");

    glutDisplayFunc(displayWrapper);
    glutKeyboardFunc(keyboardDownWrapper);
    glutKeyboardUpFunc(keyboardUpWrapper);
    glutSpecialFunc(specialDownWrapper);
    glutSpecialUpFunc(specialUpWrapper);
    glutMouseFunc(mouseWrapper);
    glutMotionFunc(motionWrapper);
    glutTimerFunc(16, timerWrapper, 0);

    glutMainLoop();
}

void GameWindow::destroy()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
        printf("GameWindow destroyed\n");
    }
}

static void displayWrapper()
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    glClear(GL_COLOR_BUFFER_BIT);

    if (screen != nullptr && screen->getActive())
    {
        screen->display();
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glFlush();
    glutSwapBuffers();
}

static void keyboardDownWrapper(unsigned char key, int x, int y)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (key == 27)
    {
        GameWindow::destroy();
        exit(0);
    }
    else if (key == 'f' || key == 'F')
    {
        window->toggleFullscreen();
    }

    if (screen != nullptr && screen->getActive())
    {
        screen->handleKeyboardDown(key, x, y);
    }
}

static void keyboardUpWrapper(unsigned char key, int x, int y)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (screen != nullptr && screen->getActive())
    {
        screen->handleKeyboardUp(key, x, y);
    }
}

static void specialDownWrapper(int key, int x, int y)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (key == GLUT_KEY_F11)
    {
        window->toggleFullscreen();
    }

    if (screen != nullptr && screen->getActive())
    {
        screen->handleSpecialKeysDown(key, x, y);
    }
}

static void specialUpWrapper(int key, int x, int y)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (screen != nullptr && screen->getActive())
    {
        screen->handleSpecialKeysUp(key, x, y);
    }
}

static void mouseWrapper(int button, int state, int x, int y)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (screen != nullptr && screen->getActive())
    {
        screen->handleMouse(button, state, x, y);
    }
}

static void motionWrapper(int x, int y)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (screen != nullptr && screen->getActive())
    {
        screen->handleMouseMotion(x, y);
    }
}

static void timerWrapper(int value)
{
    GameWindow *window = GameWindow::getInstance();
    GameScreen *screen = window->getScreen();

    if (screen != nullptr && screen->getActive())
    {
        screen->update(TIME_DELTA);
    }

    glutPostRedisplay();
    glutTimerFunc(16, timerWrapper, 0);
}
