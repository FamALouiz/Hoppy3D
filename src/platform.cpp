#include "platform.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cstdlib>
#include "generators/platform_generator.h"

GLuint Platform::spriteTexture = 0;
bool Platform::textureLoaded = false;
Platform *Platform::currentDrawingPlatform = nullptr;

void Platform::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load(PLATFORM_SPRITE_PATH, &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Failed to load platform texture: " << PLATFORM_SPRITE_PATH << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    glGenTextures(1, &spriteTexture);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    textureLoaded = true;
}

void Platform::defaultDrawFunc(float x, float y)
{
    if (!currentDrawingPlatform)
        return;

    int tileCount = currentDrawingPlatform->tileCount;

    if (!textureLoaded)
        loadTexture();

    if (!textureLoaded)
    {
        float width = tileCount * PLATFORM_TILE_SIZE;
        glBegin(GL_QUADS);
        glColor3f(PLATFORM_COLOR_R, PLATFORM_COLOR_G, PLATFORM_COLOR_B);
        glVertex2f(x - width / 2.0f, y - PLATFORM_HEIGHT / 2.0f);
        glVertex2f(x + width / 2.0f, y - PLATFORM_HEIGHT / 2.0f);
        glVertex2f(x + width / 2.0f, y + PLATFORM_HEIGHT / 2.0f);
        glVertex2f(x - width / 2.0f, y + PLATFORM_HEIGHT / 2.0f);
        glEnd();
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    int row = PLATFORM_SPRITE_INDEX / PLATFORM_SPRITE_COLS;
    int col = PLATFORM_SPRITE_INDEX % PLATFORM_SPRITE_COLS;

    float texLeft = (float)col / PLATFORM_SPRITE_COLS;
    float texRight = (float)(col + 1) / PLATFORM_SPRITE_COLS;
    float texTop = (float)row / PLATFORM_SPRITE_ROWS;
    float texBottom = (float)(row + 1) / PLATFORM_SPRITE_ROWS;

    for (int i = 0; i < tileCount; i++)
    {
        float leftX = x - (tileCount * PLATFORM_TILE_SIZE) / 2.0f + i * PLATFORM_TILE_SIZE;
        float rightX = leftX + PLATFORM_TILE_SIZE;

        if (leftX <= SCREEN_LEFT)
            continue;
        if (rightX >= SCREEN_RIGHT)
            break;

        glBegin(GL_QUADS);
        glTexCoord2f(texLeft, texBottom);
        glVertex2f(leftX, y - PLATFORM_HEIGHT / 2.0f);
        glTexCoord2f(texRight, texBottom);
        glVertex2f(rightX, y - PLATFORM_HEIGHT / 2.0f);
        glTexCoord2f(texRight, texTop);
        glVertex2f(rightX, y + PLATFORM_HEIGHT / 2.0f);
        glTexCoord2f(texLeft, texTop);
        glVertex2f(leftX, y + PLATFORM_HEIGHT / 2.0f);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

Platform::Platform(float x, float y)
    : StaticObject(x, y, defaultDrawFunc), tileCount(PLATFORM_BASE_TILES)
{
    setCollisionBox(tileCount * PLATFORM_TILE_SIZE, PLATFORM_HEIGHT);
}

Platform::Platform(float x, float y, int tiles)
    : StaticObject(x, y, defaultDrawFunc), tileCount(tiles)
{
    setCollisionBox(tileCount * PLATFORM_TILE_SIZE, PLATFORM_HEIGHT);
}

Platform::Platform(float x, float y, void (*drawFunc)(float, float))
    : StaticObject(x, y, drawFunc), tileCount(PLATFORM_BASE_TILES)
{
    setCollisionBox(PLATFORM_WIDTH, PLATFORM_HEIGHT);
}

Platform::Platform(float x, float y, float width, float height)
    : StaticObject(x, y, defaultDrawFunc), tileCount(PLATFORM_BASE_TILES)
{
    setCollisionBox(width, height);
}

Platform::Platform(float x, float y, float width, float height, void (*drawFunc)(float, float))
    : StaticObject(x, y, drawFunc), tileCount(PLATFORM_BASE_TILES)
{
    setCollisionBox(width, height);
}

Platform::~Platform()
{
}

void Platform::draw()
{
    currentDrawingPlatform = this;
    StaticObject::draw();
    currentDrawingPlatform = nullptr;
}

void Platform::cleanupTexture()
{
    if (textureLoaded)
    {
        glDeleteTextures(1, &spriteTexture);
        textureLoaded = false;
    }
}
