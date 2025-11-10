#ifndef PLATFORM_H
#define PLATFORM_H

#include "physics/objects.h"
#include <GL/glut.h>

#define PLATFORM_WIDTH 0.4f
#define PLATFORM_HEIGHT 0.1f
#define PLATFORM_TILE_SIZE 0.057f
#define PLATFORM_BASE_TILES 7
#define PLATFORM_MIN_TILES 5
#define PLATFORM_MAX_TILES 9
#define PLATFORM_COLOR_R 0.5f
#define PLATFORM_COLOR_G 0.3f
#define PLATFORM_COLOR_B 0.1f

#define PLATFORM_SPRITE_COLS 16
#define PLATFORM_SPRITE_ROWS 16
#define PLATFORM_SPRITE_INDEX 0
#define PLATFORM_SPRITE_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\world_tileset.png"

class Platform : public StaticObject
{
private:
    int tileCount;
    static GLuint spriteTexture;
    static bool textureLoaded;
    static void loadTexture();
    static void defaultDrawFunc(float x, float y);
    static Platform *currentDrawingPlatform;

public:
    Platform(float x, float y);
    Platform(float x, float y, int tiles);
    Platform(float x, float y, void (*drawFunc)(float, float));
    Platform(float x, float y, float width, float height);
    Platform(float x, float y, float width, float height, void (*drawFunc)(float, float));
    ~Platform();

    void draw() override;
    int getTileCount() const { return tileCount; }
    static float getDefaultWidth() { return PLATFORM_WIDTH; }
    static float getDefaultHeight() { return PLATFORM_HEIGHT; }
    static void cleanupTexture();
};

#endif
