#ifndef LAVA_FREEZE_H
#define LAVA_FREEZE_H

#include "generators/powerups/powerups.h"

#define LAVA_FREEZE_SPAWN_RATE 0.10f
#define LAVA_FREEZE_DURATION 5.0f
#define LAVA_FREEZE_COLOR_R 0.3f
#define LAVA_FREEZE_COLOR_G 0.8f
#define LAVA_FREEZE_COLOR_B 1.0f

class Lava;
class LavaFreeze : public Powerup
{
private:
    Lava *lava;
    float savedSpeed;
    float savedAcceleration;
    static GLuint texture;
    static bool textureLoaded;
    static void loadTexture();
    static void defaultDrawFunc(float x, float y);

public:
    LavaFreeze(float x, float y, Lava *lava);
    LavaFreeze(float x, float y, Lava *lava, void (*drawFunc)(float, float));
    ~LavaFreeze();

    void activate() override;
    void deactivate() override;
    void update(float deltaTime) override;
};

#endif
