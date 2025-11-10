#include "hud.h"
#include "stb_image.h"
#include "generators/powerup_manager.h"
#include <iostream>
#include <cmath>

GLuint HUD::spriteTexture = 0;
GLuint HUD::lavaWarningTexture = 0;
bool HUD::textureLoaded = false;

void HUD::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load(HUD_SPRITE_PATH, &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Failed to load HUD texture: " << HUD_SPRITE_PATH << std::endl;
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

HUD::HUD(Player *player, Lava *lava) : player(player), lava(lava)
{
    loadTexture();
}

HUD::~HUD()
{
}

void HUD::drawBackground(float camY)
{
    glDisable(GL_TEXTURE_2D);

    float screenTop = camY + 1.0f;
    float hudRight = 1.0f;
    float hudLeft = hudRight - HUD_WIDTH;
    float hudTop = screenTop;
    float hudBottom = hudTop - HUD_HEIGHT;

    glBegin(GL_QUADS);
    glColor4f(HUD_BACKGROUND_COLOR_R, HUD_BACKGROUND_COLOR_G, HUD_BACKGROUND_COLOR_B, HUD_BACKGROUND_ALPHA);
    glVertex2f(hudLeft, hudBottom);
    glVertex2f(hudRight, hudBottom);
    glVertex2f(hudRight, hudTop);
    glVertex2f(hudLeft, hudTop);
    glEnd();
}

void HUD::drawHeart(float x, float y, bool filled)
{
    if (!textureLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    int row = filled ? HUD_HEART_FILLED_ROW : HUD_HEART_EMPTY_ROW;
    int col = filled ? HUD_HEART_FILLED_COL : HUD_HEART_EMPTY_COL;

    float texLeft = (float)col / HUD_SPRITE_COLS;
    float texRight = (float)(col + 2) / HUD_SPRITE_COLS;
    float texTop = (float)row / HUD_SPRITE_ROWS;
    float texBottom = (float)(row + 2) / HUD_SPRITE_ROWS;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x, y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + HUD_HEART_SIZE, y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + HUD_HEART_SIZE, y + HUD_HEART_SIZE);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x, y + HUD_HEART_SIZE);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    float centerX = x + HUD_HEART_SIZE / 2;
    float centerY = y + HUD_HEART_SIZE / 2;
    float decorSize = HUD_HEART_SIZE * 0.15f;

    glBegin(GL_TRIANGLES);
    if (filled)
    {
        glColor4f(1.0f, 0.8f, 0.8f, 0.6f);
    }
    else
    {
        glColor4f(0.5f, 0.5f, 0.5f, 0.4f);
    }
    glVertex2f(centerX - decorSize, centerY);
    glVertex2f(centerX + decorSize, centerY);
    glVertex2f(centerX, centerY + decorSize);
    glEnd();

    const int segments = 6;
    glBegin(GL_TRIANGLE_FAN);
    if (filled)
    {
        glColor4f(1.0f, 0.9f, 0.9f, 0.3f);
    }
    else
    {
        glColor4f(0.6f, 0.6f, 0.6f, 0.3f);
    }
    glVertex2f(centerX, centerY);
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.14159f * i / segments;
        float cx = centerX + decorSize * 0.5f * cos(angle);
        float cy = centerY + decorSize * 0.5f * sin(angle);
        glVertex2f(cx, cy);
    }
    glEnd();
}

void HUD::drawIcon(float x, float y, int row, int col, float size)
{
    if (!textureLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);

    float texLeft = (float)col / HUD_SPRITE_COLS;
    float texRight = (float)(col + 1) / HUD_SPRITE_COLS;
    float texTop = (float)row / HUD_SPRITE_ROWS;
    float texBottom = (float)(row + 1) / HUD_SPRITE_ROWS;

    glBegin(GL_QUADS);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x, y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + size, y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + size, y + size);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x, y + size);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    float centerX = x + size / 2;
    float centerY = y + size / 2;
    float decorSize = size * 0.2f;

    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.5f, 0.0f, 0.5f);
    glVertex2f(centerX - decorSize, centerY - decorSize);
    glVertex2f(centerX + decorSize, centerY - decorSize);
    glVertex2f(centerX + decorSize, centerY + decorSize);
    glVertex2f(centerX - decorSize, centerY + decorSize);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor4f(1.0f, 0.3f, 0.0f, 0.6f);
    glVertex2f(centerX, centerY + decorSize * 1.5f);
    glVertex2f(centerX - decorSize, centerY);
    glVertex2f(centerX + decorSize, centerY);
    glEnd();
}

void HUD::drawPowerupIcon(float x, float y, int row, int col, float size, float alpha)
{
    if (!textureLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);

    float texLeft = (float)col / HUD_SPRITE_COLS;
    float texRight = (float)(col + 1) / HUD_SPRITE_COLS;
    float texTop = (float)row / HUD_SPRITE_ROWS;
    float texBottom = (float)(row + 1) / HUD_SPRITE_ROWS;

    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x, y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + size, y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + size, y + size);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x, y + size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void HUD::drawProgressBar(float x, float y, float width, float height, float progress)
{
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glColor4f(0.3f, 0.3f, 0.3f, 0.6f);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    float filledWidth = width * progress;
    glBegin(GL_QUADS);
    glColor4f(0.2f, 0.8f, 0.2f, 0.8f);
    glVertex2f(x, y);
    glVertex2f(x + filledWidth, y);
    glVertex2f(x + filledWidth, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void HUD::drawPowerups(float startX, float &currentY, float camY)
{
    const std::vector<ActivePowerup> &activePowerups = PowerupManager::getInstance()->getActivePowerups();

    if (activePowerups.empty())
        return;

    currentY -= HUD_SPACING * 2.0f;

    for (size_t i = 0; i < activePowerups.size(); i++)
    {
        const ActivePowerup &active = activePowerups[i];
        Powerup *powerup = active.powerup;

        int row = 0;
        int col = 0;

        switch (powerup->getType())
        {
        case POWERUP_LAVA_FREEZE:
            row = HUD_LAVA_FREEZE_ROW;
            col = HUD_LAVA_FREEZE_COL;
            break;
        case POWERUP_SHIELD:
            row = HUD_SHIELD_ROW;
            col = HUD_SHIELD_COL;
            break;
        case POWERUP_KEY:
            row = HUD_KEY_ROW;
            col = HUD_KEY_COL;
            break;
        case POWERUP_SUPER_KEY:
            row = HUD_SUPER_KEY_ROW;
            col = HUD_SUPER_KEY_COL;
            break;
        }

        float iconY = currentY - HUD_POWERUP_ICON_SIZE;

        float alpha = 1.0f;
        if (active.timeRemaining < 1.0f)
        {
            alpha = 0.5f + 0.5f * (active.timeRemaining * 2.0f);
        }
        drawPowerupIcon(startX, iconY, row, col, HUD_POWERUP_ICON_SIZE, alpha);

        float barX = startX + HUD_POWERUP_ICON_SIZE + HUD_POWERUP_SPACING;
        float barY = iconY + HUD_POWERUP_ICON_SIZE * 0.3f;
        float barWidth = 0.1f;
        float barHeight = 0.015f;
        float progress = active.timeRemaining / powerup->getDuration();

        drawProgressBar(barX, barY, barWidth, barHeight, progress);

        int timeLeft = (int)(active.timeRemaining + 0.5f);
        float timeX = barX + barWidth + HUD_POWERUP_SPACING;
        drawNumber(timeX, iconY + HUD_POWERUP_ICON_SIZE * 0.25f, timeLeft);

        currentY -= HUD_POWERUP_ICON_SIZE + HUD_POWERUP_SPACING;
    }
}

void HUD::drawWarningLevel(float x, float y, int row, int col, float size, int warningLevel)
{

    int width, height, channels;
    std::string texturePath = std::string(LAVA_WARNING_SPRITE_PATH) + "_" + std::to_string(warningLevel) + ".png";
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "Failed to load lava warning texture: " << texturePath << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    glGenTextures(1, &lavaWarningTexture);
    glBindTexture(GL_TEXTURE_2D, lavaWarningTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, lavaWarningTexture);

    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + size, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + size, y + size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + size);
    glEnd();
}

void HUD::drawNumber(float x, float y, int number)
{
    if (!textureLoaded)
        return;

    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    std::string numStr = std::to_string(number);
    float charWidth = 0.04f;
    float charHeight = 0.06f;

    for (size_t i = 0; i < numStr.length(); i++)
    {
        char c = numStr[i];
        float offsetX = x + i * charWidth;

        glRasterPos2f(offsetX, y);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

int HUD::getLavaWarningLevel(float camY)
{
    if (!player || !lava)
        return 2;

    float playerY = player->getY();
    float lavaY = lava->getY() + LAVA_HEIGHT / 2.0f;
    float distance = playerY - lavaY;

    if (distance <= LAVA_CRITICAL_DISTANCE)
    {
        return 0;
    }
    else if (distance <= LAVA_WARNING_DISTANCE)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

void HUD::draw(float camY)
{
    if (!player)
        return;

    drawBackground(camY);

    float screenTop = camY + 1.0f;
    float hudRight = 1.0f;
    float startX = hudRight - HUD_WIDTH + HUD_PADDING;
    float currentY = screenTop - HUD_PADDING;

    int maxLives = 3;
    int currentLives = player->getLives();

    for (int i = 0; i < maxLives; i++)
    {
        float heartX = startX + i * (HUD_HEART_SIZE + HUD_SPACING);
        bool filled = i < currentLives;
        drawHeart(heartX, currentY - HUD_HEART_SIZE, filled);
    }

    currentY -= HUD_HEART_SIZE + HUD_SPACING * 2.0f;

    float iconRowY = currentY - HUD_ICON_SIZE;

    drawIcon(startX, iconRowY, HUD_KEY_ROW, HUD_KEY_COL, HUD_ICON_SIZE);

    float numberX = startX + HUD_ICON_SIZE + HUD_SPACING;
    drawNumber(numberX, iconRowY + 0.01f, player->getKeys());

    int warningLevel = getLavaWarningLevel(camY);
    int warningRow = HUD_WARNING_LOW_ROW;
    int warningCol = HUD_WARNING_LOW_COL;

    if (warningLevel == 1)
    {
        warningRow = HUD_WARNING_MED_ROW;
        warningCol = HUD_WARNING_MED_COL;
    }
    else if (warningLevel == 2)
    {
        warningRow = HUD_WARNING_HIGH_ROW;
        warningCol = HUD_WARNING_HIGH_COL;
    }

    float warningX = numberX + 0.08f;
    drawWarningLevel(warningX, iconRowY, warningRow, warningCol, HUD_ICON_SIZE, warningLevel);

    currentY -= HUD_ICON_SIZE + HUD_SPACING * 2.0f;
    drawPowerups(startX, currentY, camY);
}

void HUD::drawWarningSprite(float x, float y, float size, float alpha)
{
    if (!textureLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);

    int row = HUD_WARNING_LOW_ROW;
    int col = HUD_WARNING_LOW_COL;

    float texLeft = (float)col / HUD_SPRITE_COLS;
    float texRight = (float)(col + 1) / HUD_SPRITE_COLS;
    float texTop = (float)row / HUD_SPRITE_ROWS;
    float texBottom = (float)(row + 1) / HUD_SPRITE_ROWS;

    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.0f, 0.0f, alpha);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x, y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + size, y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + size, y + size);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x, y + size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void HUD::cleanupTexture()
{
    if (textureLoaded)
    {
        glDeleteTextures(1, &spriteTexture);
        textureLoaded = false;
    }
}
