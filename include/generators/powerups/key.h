#ifndef KEY_H
#define KEY_H

#include "generators/powerups/powerups.h"
#include "player.h"

#define KEY_COLOR_R 1.0f
#define KEY_COLOR_G 0.6f
#define KEY_COLOR_B 1.0f
#define KEY_SPAWN_RATE 0.1f

class Player;
class Key : public Powerup
{
private:
    Player *player;
    static GLuint texture;
    static bool textureLoaded;
    static void loadTexture();
    static void defaultDrawFunc(float x, float y);

public:
    Key(float x, float y, Player *player);
    Key(float x, float y, Player *player, void (*drawFunc)(float, float));
    ~Key();

    void activate() override;
    void deactivate() override;
};

#endif
