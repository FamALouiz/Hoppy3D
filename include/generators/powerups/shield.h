#ifndef SHIELD_H
#define SHIELD_H

#include "generators/powerups/powerups.h"

#define SHIELD_SPAWN_RATE 0.12f
#define SHIELD_DURATION 4.0f
#define SHIELD_COLOR_R 1.0f
#define SHIELD_COLOR_G 0.84f
#define SHIELD_COLOR_B 0.0f

class Player;
class Shield : public Powerup
{
private:
    Player *player;
    static GLuint texture;
    static bool textureLoaded;
    static void loadTexture();
    static void defaultDrawFunc(float x, float y);

public:
    Shield(float x, float y, Player *player);
    Shield(float x, float y, Player *player, void (*drawFunc)(float, float));
    ~Shield();

    void activate() override;
    void deactivate() override;
};

#endif
