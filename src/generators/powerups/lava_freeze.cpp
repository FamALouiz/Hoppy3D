#include "generators/powerups/lava_freeze.h"
#include "lava.h"
#include "stb_image.h"
#include <iostream>
#include <cmath>

GLuint LavaFreeze::texture = 0;
bool LavaFreeze::textureLoaded = false;

void LavaFreeze::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load("C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\lavafreeze.png", &width, &height, &channels, 4);

    if (data)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        textureLoaded = true;
    }
    else
    {
        std::cerr << "Failed to load lavafreeze texture: " << stbi_failure_reason() << std::endl;
    }
}

void LavaFreeze::defaultDrawFunc(float x, float y)
{
    if (!textureLoaded)
    {
        loadTexture();
    }

    if (textureLoaded)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(x - POWERUP_SIZE, y - POWERUP_SIZE);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x + POWERUP_SIZE, y - POWERUP_SIZE);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x + POWERUP_SIZE, y + POWERUP_SIZE);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x - POWERUP_SIZE, y + POWERUP_SIZE);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glBegin(GL_QUADS);
        glColor3f(LAVA_FREEZE_COLOR_R, LAVA_FREEZE_COLOR_G, LAVA_FREEZE_COLOR_B);
        glVertex2f(x - POWERUP_SIZE, y - POWERUP_SIZE);
        glVertex2f(x + POWERUP_SIZE, y - POWERUP_SIZE);
        glVertex2f(x + POWERUP_SIZE, y + POWERUP_SIZE);
        glVertex2f(x - POWERUP_SIZE, y + POWERUP_SIZE);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x, y + POWERUP_SIZE * 0.5f);
        glVertex2f(x - POWERUP_SIZE * 0.4f, y - POWERUP_SIZE * 0.3f);
        glVertex2f(x + POWERUP_SIZE * 0.4f, y - POWERUP_SIZE * 0.3f);
        glEnd();

        const int segments = 6;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.8f, 0.9f, 1.0f);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++)
        {
            float angle = 2.0f * 3.14159f * i / segments;
            float cx = x + POWERUP_SIZE * 0.3f * cos(angle);
            float cy = y + POWERUP_SIZE * 0.3f * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.6f, 0.8f, 0.95f);
        glVertex2f(x - POWERUP_SIZE * 0.2f, y - POWERUP_SIZE * 0.6f);
        glVertex2f(x + POWERUP_SIZE * 0.2f, y - POWERUP_SIZE * 0.6f);
        glVertex2f(x + POWERUP_SIZE * 0.2f, y - POWERUP_SIZE * 0.8f);
        glVertex2f(x - POWERUP_SIZE * 0.2f, y - POWERUP_SIZE * 0.8f);
        glEnd();
    }
}

LavaFreeze::LavaFreeze(float x, float y, Lava *lava)
    : Powerup(x, y, POWERUP_LAVA_FREEZE, LAVA_FREEZE_SPAWN_RATE, LAVA_FREEZE_DURATION, defaultDrawFunc),
      lava(lava), savedSpeed(0.0f), savedAcceleration(0.0f)
{
}

LavaFreeze::LavaFreeze(float x, float y, Lava *lava, void (*drawFunc)(float, float))
    : Powerup(x, y, POWERUP_LAVA_FREEZE, LAVA_FREEZE_SPAWN_RATE, LAVA_FREEZE_DURATION, drawFunc),
      lava(lava), savedSpeed(0.0f), savedAcceleration(0.0f)
{
}

LavaFreeze::~LavaFreeze()
{
}

void LavaFreeze::activate()
{
    if (lava && !isActive)
    {
        isActive = true;
        savedSpeed = lava->getCurrentSpeed();
        savedAcceleration = lava->getAccelerationY();
        std::cout << "Saved lava speed: " << savedSpeed << ", acceleration: " << savedAcceleration << std::endl;
        lava->setVelocity(0.0f, 0.0f);
        lava->setAcceleration(0.0f, 0.0f);
        std::cout << "Lava freeze activated!" << std::endl;
    }
}

void LavaFreeze::deactivate()
{
    if (lava && isActive)
    {
        isActive = false;
        lava->setVelocity(0.0f, savedSpeed);
        lava->setAcceleration(0.0f, savedAcceleration);
        std::cout << "Lava freeze deactivated!" << std::endl;
    }
}

void LavaFreeze::update(float deltaTime)
{
    animationTimer += deltaTime;
    float offset = sin(animationTimer * POWERUP_BOB_SPEED) * POWERUP_BOB_AMPLITUDE;
    setPosition(baseX + offset, y);
}