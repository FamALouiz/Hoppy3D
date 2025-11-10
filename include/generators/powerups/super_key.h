#ifndef SUPER_KEY_H
#define SUPER_KEY_H

#include "generators/powerups/powerups.h"

#define SUPER_KEY_COLOR_R 1.0f
#define SUPER_KEY_COLOR_G 0.84f
#define SUPER_KEY_COLOR_B 0.0f
#define SUPER_KEY_SPAWN_RATE 1.0f

class Player;
class SuperKey : public Powerup
{
private:
    Player *player;
    static GLuint texture;
    static bool textureLoaded;
    static void loadTexture();
    static void defaultDrawFunc(float x, float y);

public:
    SuperKey(float x, float y, Player *player);
    SuperKey(float x, float y, Player *player, void (*drawFunc)(float, float));
    ~SuperKey();

    void activate() override;
    void deactivate() override;
};

#endif
