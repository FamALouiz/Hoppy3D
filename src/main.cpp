#include "game_window/game_window.h"
#include "screens/start_screen.h"
#include <GL/glut.h>

int main(int argc, char **argv)
{
    GameWindow *window = GameWindow::getInstance();

    bool startFullscreen = true;
    window->create(argc, argv, 800, 600, "Hoppy - OpenGL Game", startFullscreen);

    StartScreen *startScreen = new StartScreen();
    window->setScreen(startScreen);

    window->run();

    GameWindow::destroy();

    return 0;
}
