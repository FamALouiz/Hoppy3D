#include "generators/powerups/super_key.h"
#include "player.h"
#include "stb_image.h"
#include <cmath>
#include <iostream>

GLuint SuperKey::texture = 0;
bool SuperKey::textureLoaded = false;

void SuperKey::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load("C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\superkey.png", &width, &height, &channels, 4);

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
        std::cerr << "Failed to load superkey texture: " << stbi_failure_reason() << std::endl;
    }
}

void SuperKey::defaultDrawFunc(float x, float y)
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
        glColor3f(SUPER_KEY_COLOR_R, SUPER_KEY_COLOR_G, SUPER_KEY_COLOR_B);
        glVertex2f(x - POWERUP_SIZE, y - POWERUP_SIZE);
        glVertex2f(x + POWERUP_SIZE, y - POWERUP_SIZE);
        glVertex2f(x + POWERUP_SIZE, y + POWERUP_SIZE);
        glVertex2f(x - POWERUP_SIZE, y + POWERUP_SIZE);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 8; i++)
        {
            float angle = 2.0f * 3.14159f * i / 8.0f;
            float nextAngle = 2.0f * 3.14159f * (i + 1) / 8.0f;
            float cx = x + POWERUP_SIZE * 0.6f * cos(angle);
            float cy = y + POWERUP_SIZE * 0.6f * sin(angle);
            float nx = x + POWERUP_SIZE * 0.6f * cos(nextAngle);
            float ny = y + POWERUP_SIZE * 0.6f * sin(nextAngle);
            glVertex2f(x, y);
            glVertex2f(cx, cy);
            glVertex2f(nx, ny);
        }
        glEnd();

        const int segments = 10;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.9f, 0.5f);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++)
        {
            float angle = 2.0f * 3.14159f * i / segments;
            float cx = x + POWERUP_SIZE * 0.35f * cos(angle);
            float cy = y + POWERUP_SIZE * 0.35f * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.9f, 0.7f, 0.3f);
        glVertex2f(x - POWERUP_SIZE * 0.15f, y - POWERUP_SIZE * 0.15f);
        glVertex2f(x + POWERUP_SIZE * 0.15f, y - POWERUP_SIZE * 0.15f);
        glVertex2f(x + POWERUP_SIZE * 0.15f, y + POWERUP_SIZE * 0.15f);
        glVertex2f(x - POWERUP_SIZE * 0.15f, y + POWERUP_SIZE * 0.15f);
        glEnd();
    }
}

SuperKey::SuperKey(float x, float y, Player *player)
    : Powerup(x, y, POWERUP_SUPER_KEY, SUPER_KEY_SPAWN_RATE, 0.0f, defaultDrawFunc), player(player)
{
    setCollisionBox(POWERUP_SIZE, POWERUP_SIZE);
}

SuperKey::SuperKey(float x, float y, Player *player, void (*drawFunc)(float, float))
    : Powerup(x, y, POWERUP_SUPER_KEY, SUPER_KEY_SPAWN_RATE, 0.0f, drawFunc), player(player)
{
    setCollisionBox(POWERUP_SIZE, POWERUP_SIZE);
}

SuperKey::~SuperKey()
{
}

void SuperKey::activate()
{
    if (player && !isActive)
    {
        isActive = true;
        player->setSuperKey(true);
    }
}

void SuperKey::deactivate()
{
    isActive = false;
}
