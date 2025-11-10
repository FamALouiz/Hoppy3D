#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <GL/glut.h>

#define BACKGROUND_SPRITE_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\world_tileset.png"
#define BACKGROUND_SPRITE_COLS 16
#define BACKGROUND_SPRITE_ROWS 16
#define BACKGROUND_SPRITE_ROW 1
#define BACKGROUND_SPRITE_COL 5
#define BACKGROUND_TILE_SIZE 0.2f

class Background
{
private:
    static GLuint spriteTexture;
    static bool textureLoaded;
    static void loadTexture();

public:
    Background();
    ~Background();

    static void draw(float camY);
    static void cleanupTexture();
};

#endif
