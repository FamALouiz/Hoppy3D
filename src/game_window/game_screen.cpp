#include "game_window/game_screen.h"

GameScreen::GameScreen() : isActive(false)
{
}

GameScreen::~GameScreen()
{
}

void GameScreen::init()
{
}

void GameScreen::update(float deltaTime)
{
}

void GameScreen::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void GameScreen::handleKeyboardUp(unsigned char key, int x, int y)
{
}

void GameScreen::handleKeyboardDown(unsigned char key, int x, int y)
{
}

void GameScreen::handleSpecialKeysUp(int key, int x, int y)
{
}

void GameScreen::handleSpecialKeysDown(int key, int x, int y)
{
}

void GameScreen::handleMouse(int button, int state, int x, int y)
{
}

void GameScreen::handleMouseMotion(int x, int y)
{
}

void GameScreen::setActive(bool active)
{
    isActive = active;
}

bool GameScreen::getActive() const
{
    return isActive;
}
