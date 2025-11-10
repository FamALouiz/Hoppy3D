#include "screens/win_screen.h"
#include "screens/main_screen.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "game_window/game_window.h"
#include "stb_image.h"

GLuint WinScreen::backgroundTexture = 0;
bool WinScreen::textureLoaded = false;

WinScreen::WinScreen() : titlePulse(0.0f), showPressStart(true), blinkTimer(0.0f)
{
    loadTexture();
}

void WinScreen::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *image = stbi_load(WIN_SCREEN_BG_PATH, &width, &height, &channels, STBI_rgb_alpha);

    if (image)
    {
        glGenTextures(1, &backgroundTexture);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        stbi_image_free(image);
        textureLoaded = true;
        printf("Win screen background texture loaded successfully\n");
    }
    else
    {
        printf("Failed to load win screen background texture: %s\n", WIN_SCREEN_BG_PATH);
    }
}

WinScreen::~WinScreen()
{
}

void WinScreen::init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WinScreen::update(float deltaTime)
{
    titlePulse += deltaTime * 2.0f;
    if (titlePulse > 6.28f)
    {
        titlePulse -= 6.28f;
    }

    blinkTimer += deltaTime;
    if (blinkTimer > 0.5f)
    {
        showPressStart = !showPressStart;
        blinkTimer = 0.0f;
    }
}

void WinScreen::display()
{
    drawBackground();
    drawTitle();
    drawPressStart();
}

void WinScreen::handleKeyboardUp(unsigned char key, int x, int y)
{
    if (key == ' ' || key == 13)
    {
        printf("Starting game...\n");
        MainScreen *mainScreen = new MainScreen();
        GameWindow::getInstance()->setScreen(mainScreen);
    }
}

void WinScreen::handleSpecialKeysUp(int key, int x, int y)
{
}

void WinScreen::handleKeyboardDown(unsigned char key, int x, int y)
{
}

void WinScreen::handleSpecialKeysDown(int key, int x, int y)
{
}

void WinScreen::drawBackground()
{
    if (textureLoaded)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glBegin(GL_QUADS);
        glColor3f(0.1f, 0.1f, 0.2f);
        glVertex2f(-1.0f, -1.0f);
    }
}

void WinScreen::drawTitle()
{
    float pulse = sin(titlePulse) * 0.1f + 0.9f;
    glColor3f(pulse, pulse * 0.8f, 0.2f);

    glColor3f(0.1f, 0.1f, 0.2f);
    drawRect(0.0f, 0.55f, 0.6f, 0.15f);

    glColor3f(pulse, pulse * 0.8f, 0.2f);
    drawText("You win!", 0.0f, 0.55f, 0.05f);

    glColor3f(0.1f, 0.1f, 0.2f);
    drawRect(0.0f, 0.35f, 0.8f, 0.15f);

    glColor3f(pulse, pulse * 0.8f, 0.2f);
    drawText("Good job knight!", 0.0f, 0.35f, 0.045f);
}

void WinScreen::drawPressStart()
{
    if (showPressStart)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        drawText("PRESS SPACE TO START", 0.0f, -0.3f, 0.05f);

        glColor3f(0.6f, 0.6f, 0.6f);
        drawText("ESC TO QUIT", 0.0f, -0.5f, 0.04f);
    }
}

void WinScreen::drawRect(float x, float y, float width, float height)
{
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    glBegin(GL_QUADS);
    glVertex2f(x - halfWidth, y - halfHeight);
    glVertex2f(x + halfWidth, y - halfHeight);
    glVertex2f(x + halfWidth, y + halfHeight);
    glVertex2f(x - halfWidth, y + halfHeight);
    glEnd();
}
