#include "background.h"
#include "stb_image.h"
#include <iostream>

GLuint Background::spriteTexture = 0;
bool Background::textureLoaded = false;

void Background::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load(BACKGROUND_SPRITE_PATH, &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Failed to load background texture: " << BACKGROUND_SPRITE_PATH << std::endl;
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

Background::Background()
{
    loadTexture();
}

Background::~Background()
{
}

void Background::draw(float camY)
{
    if (!textureLoaded)
        loadTexture();

    if (!textureLoaded)
    {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    float texLeft = (float)BACKGROUND_SPRITE_COL / BACKGROUND_SPRITE_COLS;
    float texRight = (float)(BACKGROUND_SPRITE_COL + 1) / BACKGROUND_SPRITE_COLS;
    float texTop = (float)BACKGROUND_SPRITE_ROW / BACKGROUND_SPRITE_ROWS;
    float texBottom = (float)(BACKGROUND_SPRITE_ROW + 1) / BACKGROUND_SPRITE_ROWS;

    float textEdgeLeft = (float)(BACKGROUND_SPRITE_COL + 3) / BACKGROUND_SPRITE_COLS;
    float textEdgeRight = (float)(BACKGROUND_SPRITE_COL + 4) / BACKGROUND_SPRITE_COLS;
    float textEdgeTop = (float)BACKGROUND_SPRITE_ROW / BACKGROUND_SPRITE_ROWS;
    float textEdgeBottom = (float)(BACKGROUND_SPRITE_ROW + 1) / BACKGROUND_SPRITE_ROWS;

    float screenLeft = -1.0f;
    float screenRight = 1.0f;
    float screenWidth = screenRight - screenLeft;
    float screenHeight = 5.0f;

    int tilesX = (int)(screenWidth / BACKGROUND_TILE_SIZE);
    int tilesY = (int)(screenHeight / BACKGROUND_TILE_SIZE) + 2;

    float startY = camY - screenHeight / 2.0f;
    startY = ((int)(startY / BACKGROUND_TILE_SIZE)) * BACKGROUND_TILE_SIZE;

    for (int y = 0; y < tilesY; y++)
    {
        for (int x = 0; x < tilesX; x++)
        {
            if (x == 0 || x == 1 || x == tilesX - 2 || x == tilesX - 1)
            {
                float posX = screenLeft + x * BACKGROUND_TILE_SIZE;
                float posY = startY + y * BACKGROUND_TILE_SIZE;

                glBegin(GL_QUADS);
                glTexCoord2f(textEdgeLeft, textEdgeBottom);
                glVertex2f(posX, posY);
                glTexCoord2f(textEdgeRight, textEdgeBottom);
                glVertex2f(posX + BACKGROUND_TILE_SIZE, posY);
                glTexCoord2f(textEdgeRight, textEdgeTop);
                glVertex2f(posX + BACKGROUND_TILE_SIZE, posY + BACKGROUND_TILE_SIZE);
                glTexCoord2f(textEdgeLeft, textEdgeTop);
                glVertex2f(posX, posY + BACKGROUND_TILE_SIZE);
                glEnd();
                continue;
            }
            float posX = screenLeft + x * BACKGROUND_TILE_SIZE;
            float posY = startY + y * BACKGROUND_TILE_SIZE;

            glBegin(GL_QUADS);
            glTexCoord2f(texLeft, texBottom);
            glVertex2f(posX, posY);
            glTexCoord2f(texRight, texBottom);
            glVertex2f(posX + BACKGROUND_TILE_SIZE, posY);
            glTexCoord2f(texRight, texTop);
            glVertex2f(posX + BACKGROUND_TILE_SIZE, posY + BACKGROUND_TILE_SIZE);
            glTexCoord2f(texLeft, texTop);
            glVertex2f(posX, posY + BACKGROUND_TILE_SIZE);
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);
}

void Background::cleanupTexture()
{
    if (textureLoaded)
    {
        glDeleteTextures(1, &spriteTexture);
        textureLoaded = false;
    }
}
