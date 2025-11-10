#include "screens/start_screen.h"
#include "screens/main_screen.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "game_window/game_window.h"

#include "stb_image.h"

GLuint StartScreen::backgroundTexture = 0;
bool StartScreen::textureLoaded = false;

StartScreen::StartScreen() : titlePulse(0.0f), showPressStart(true), blinkTimer(0.0f)
{
    loadTexture();
}

void StartScreen::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *image = stbi_load(START_SCREEN_BG_PATH, &width, &height, &channels, STBI_rgb_alpha);

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
        printf("Start screen background texture loaded successfully\n");
    }
    else
    {
        printf("Failed to load start screen background texture: %s\n", START_SCREEN_BG_PATH);
    }
}

StartScreen::~StartScreen()
{
}

void StartScreen::init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void StartScreen::update(float deltaTime)
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

void StartScreen::display()
{
    drawBackground();
    drawTitle();
    drawPressStart();
}

void StartScreen::handleKeyboardUp(unsigned char key, int x, int y)
{
    if (key == ' ' || key == 13)
    {
        printf("Starting game...\n");
        MainScreen *mainScreen = new MainScreen();
        GameWindow::getInstance()->setScreen(mainScreen);
    }
}

void StartScreen::handleSpecialKeysUp(int key, int x, int y)
{
}

void StartScreen::handleKeyboardDown(unsigned char key, int x, int y)
{
}

void StartScreen::handleSpecialKeysDown(int key, int x, int y)
{
}

void StartScreen::drawBackground()
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
        glVertex2f(1.0f, -1.0f);
        glColor3f(0.2f, 0.2f, 0.4f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();
    }
}

void StartScreen::drawTitle()
{
    float pulse = sin(titlePulse) * 0.1f + 0.9f;
    glColor3f(pulse, pulse * 0.8f, 0.2f);

    // drawRect(-0.4f, 0.3f, 0.8f, 0.3f);

    glColor3f(0.1f, 0.1f, 0.2f);
    drawRect(0.0f, 0.45f, 0.7f, 0.15f);

    glColor3f(pulse, pulse * 0.8f, 0.2f);
    drawText("Hoppy - Ready to have fun?", 0.0f, 0.45f, 0.04f);
}

void StartScreen::drawPressStart()
{
    if (showPressStart)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        drawText("PRESS SPACE TO START", 0.0f, -0.3f, 0.05f);

        glColor3f(0.6f, 0.6f, 0.6f);
        drawText("ESC TO QUIT", 0.0f, -0.5f, 0.04f);
    }
}

void StartScreen::drawRect(float x, float y, float width, float height)
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
