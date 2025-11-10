#include "generators/powerups/key.h"
#include "stb_image.h"
#include <iostream>
#include <cmath>

GLuint Key::texture = 0;
bool Key::textureLoaded = false;

void Key::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load("C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\coin.png", &width, &height, &channels, 4);

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
        std::cerr << "Failed to load coin texture: " << stbi_failure_reason() << std::endl;
    }
}

void Key::defaultDrawFunc(float x, float y)
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

        float textLeft = 0.0f;
        float textRight = 1.0f / 12.0f;
        float textTop = 1.0f;
        float textBottom = 0.0f;

        float size = POWERUP_SIZE * 1.5f;

        glBegin(GL_QUADS);
        glTexCoord2f(textLeft, textTop);
        glVertex2f(x - size, y - size);
        glTexCoord2f(textRight, textTop);
        glVertex2f(x + size, y - size);
        glTexCoord2f(textRight, textBottom);
        glVertex2f(x + size, y + size);
        glTexCoord2f(textLeft, textBottom);
        glVertex2f(x - size, y + size);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glColor3f(KEY_COLOR_R, KEY_COLOR_G, KEY_COLOR_B);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - POWERUP_SIZE, y - POWERUP_SIZE * 0.5f);
        glVertex2f(x + POWERUP_SIZE, y - POWERUP_SIZE * 0.5f);
        glVertex2f(x, y + POWERUP_SIZE);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(x - POWERUP_SIZE * 0.5f, y - POWERUP_SIZE * 0.5f);
        glVertex2f(x + POWERUP_SIZE * 0.5f, y - POWERUP_SIZE * 0.5f);
        glVertex2f(x + POWERUP_SIZE * 0.5f, y - POWERUP_SIZE);
        glVertex2f(x - POWERUP_SIZE * 0.5f, y - POWERUP_SIZE);
        glEnd();

        const int segments = 8;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.9f, 0.3f);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++)
        {
            float angle = 2.0f * 3.14159f * i / segments;
            float cx = x + POWERUP_SIZE * 0.4f * cos(angle);
            float cy = y + POWERUP_SIZE * 0.4f * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.8f, 0.7f, 0.2f);
        glVertex2f(x - POWERUP_SIZE * 0.2f, y + POWERUP_SIZE * 0.2f);
        glVertex2f(x + POWERUP_SIZE * 0.2f, y + POWERUP_SIZE * 0.2f);
        glVertex2f(x + POWERUP_SIZE * 0.2f, y + POWERUP_SIZE * 0.6f);
        glVertex2f(x - POWERUP_SIZE * 0.2f, y + POWERUP_SIZE * 0.6f);
        glEnd();
    }
}

Key::Key(float x, float y, Player *player)
    : Powerup(x, y, POWERUP_KEY, KEY_SPAWN_RATE, 0.0f, Key::defaultDrawFunc), player(player)
{
}

Key::Key(float x, float y, Player *player, void (*drawFunc)(float, float))
    : Powerup(x, y, POWERUP_KEY, KEY_SPAWN_RATE, 0.0f, drawFunc), player(player)
{
}

Key::~Key()
{
}

void Key::activate()
{
    std::cout << "Key collected!" << std::endl;
    if (player && !isActive)
    {
        isActive = true;
        player->addKey();
    }
}

void Key::deactivate()
{
    isActive = false;
}