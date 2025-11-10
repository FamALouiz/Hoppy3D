#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include "player.h"
#include "lava.h"

#define HUD_SPRITE_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\hud.png"
#define LAVA_WARNING_SPRITE_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\lava_warning"
#define HUD_SPRITE_COLS 16
#define HUD_SPRITE_ROWS 16
#define HUD_HEART_FILLED_ROW 6
#define HUD_HEART_FILLED_COL 8
#define HUD_HEART_EMPTY_ROW 6
#define HUD_HEART_EMPTY_COL 12
#define HUD_KEY_ROW 0
#define HUD_KEY_COL 0
#define HUD_SHIELD_ROW 0
#define HUD_SHIELD_COL 2
#define HUD_LAVA_FREEZE_ROW 0
#define HUD_LAVA_FREEZE_COL 3
#define HUD_SUPER_KEY_ROW 0
#define HUD_SUPER_KEY_COL 4
#define HUD_WARNING_LOW_ROW 8
#define HUD_WARNING_LOW_COL 0
#define HUD_WARNING_MED_ROW 8
#define HUD_WARNING_MED_COL 1
#define HUD_WARNING_HIGH_ROW 8
#define HUD_WARNING_HIGH_COL 2
#define HUD_HEART_SIZE 0.12f
#define HUD_ICON_SIZE 0.08f
#define HUD_BACKGROUND_COLOR_R 0.1f
#define HUD_BACKGROUND_COLOR_G 0.1f
#define HUD_BACKGROUND_COLOR_B 0.15f
#define HUD_BACKGROUND_ALPHA 0.4f
#define HUD_WIDTH 0.5f
#define HUD_HEIGHT 0.4f
#define HUD_PADDING 0.02f
#define HUD_SPACING 0.015f
#define HUD_POWERUP_ICON_SIZE 0.06f
#define HUD_POWERUP_SPACING 0.01f
#define LAVA_CRITICAL_DISTANCE 0.2f
#define LAVA_WARNING_DISTANCE 0.9f

class HUD
{
private:
    Player *player;
    Lava *lava;
    static GLuint spriteTexture;
    static GLuint lavaWarningTexture;
    static bool textureLoaded;
    static void loadTexture();
    void drawHeart(float x, float y, bool filled);
    void drawIcon(float x, float y, int row, int col, float size);
    void drawWarningLevel(float x, float y, int row, int col, float size, int warningLevel);
    void drawBackground(float camY);
    void drawNumber(float x, float y, int number);
    void drawPowerups(float startX, float &currentY, float camY);
    void drawPowerupIcon(float x, float y, int row, int col, float size, float alpha = 1.0f);
    void drawProgressBar(float x, float y, float width, float height, float progress);
    int getLavaWarningLevel(float camY);

public:
    HUD(Player *player, Lava *lava);
    ~HUD();

    void draw(float camY);
    void drawWarningSprite(float x, float y, float size, float alpha);
    static void cleanupTexture();
};

#endif
