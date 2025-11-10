#include "generators/powerups/shield.h"
#include "player.h"
#include "stb_image.h"
#include <cmath>
#include <iostream>

GLuint Shield::texture = 0;
bool Shield::textureLoaded = false;

void Shield::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load("C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\shield.png", &width, &height, &channels, 4);

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
        std::cerr << "Failed to load shield texture: " << stbi_failure_reason() << std::endl;
    }
}

void Shield::defaultDrawFunc(float x, float y)
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
        glColor3f(SHIELD_COLOR_R, SHIELD_COLOR_G, SHIELD_COLOR_B);
        glVertex2f(x - POWERUP_SIZE, y - POWERUP_SIZE);
        glVertex2f(x + POWERUP_SIZE, y - POWERUP_SIZE);
        glVertex2f(x + POWERUP_SIZE, y + POWERUP_SIZE);
        glVertex2f(x - POWERUP_SIZE, y + POWERUP_SIZE);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 1.0f);
        int segments = 8;
        for (int i = 0; i < segments; i++)
        {
            float angle = 2.0f * 3.14159f * i / segments;
            float cx = x + POWERUP_SIZE * 0.5f * cos(angle);
            float cy = y + POWERUP_SIZE * 0.5f * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.7f, 0.7f, 0.9f);
        glVertex2f(x, y + POWERUP_SIZE * 0.6f);
        glVertex2f(x - POWERUP_SIZE * 0.4f, y - POWERUP_SIZE * 0.2f);
        glVertex2f(x + POWERUP_SIZE * 0.4f, y - POWERUP_SIZE * 0.2f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.7f);
        glVertex2f(x - POWERUP_SIZE * 0.3f, y - POWERUP_SIZE * 0.4f);
        glVertex2f(x + POWERUP_SIZE * 0.3f, y - POWERUP_SIZE * 0.4f);
        glVertex2f(x + POWERUP_SIZE * 0.3f, y - POWERUP_SIZE * 0.6f);
        glVertex2f(x - POWERUP_SIZE * 0.3f, y - POWERUP_SIZE * 0.6f);
        glEnd();
    }
}

Shield::Shield(float x, float y, Player *player)
    : Powerup(x, y, POWERUP_SHIELD, SHIELD_SPAWN_RATE, SHIELD_DURATION, defaultDrawFunc), player(player)
{
}

Shield::Shield(float x, float y, Player *player, void (*drawFunc)(float, float))
    : Powerup(x, y, POWERUP_SHIELD, SHIELD_SPAWN_RATE, SHIELD_DURATION, drawFunc), player(player)
{
}

Shield::~Shield()
{
}

void Shield::activate()
{
    if (player && !isActive)
    {
        isActive = true;
        player->setInvincible(true);
        player->setFlashingDuration(SHIELD_DURATION);
        std::cout << "Shield activated!" << std::endl;
    }
}

void Shield::deactivate()
{
    if (player && isActive)
    {
        isActive = false;
        player->setInvincible(false);
        std::cout << "Shield deactivated!" << std::endl;
    }
}
