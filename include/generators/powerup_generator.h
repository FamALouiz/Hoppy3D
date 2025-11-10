#ifndef POWERUP_GENERATOR_H
#define POWERUP_GENERATOR_H

#include "generators/powerups/powerups.h"
#include "generators/powerups/lava_freeze.h"
#include "generators/powerups/shield.h"
#include "generators/powerups/key.h"
#include "generators/powerups/super_key.h"
#include "platform.h"
#include <vector>

#define POWERUP_CHECK_DISTANCE 0.5f
#define KEYS_NEEDED_FOR_SUPER_KEY 5

class Player;
class Lava;

class PowerupGenerator
{
private:
    Player *player;
    Lava *lava;
    float lastCheckedHeight;
    float checkInterval;
    bool superKeyGenerated;

    bool shouldSpawnPowerup(float spawnRate);
    PowerupType selectRandomPowerup();

public:
    PowerupGenerator(Player *player, Lava *lava, float checkInterval = 1.0f);
    ~PowerupGenerator();

    void generatePowerups(std::vector<Powerup *> &powerups, std::vector<Platform *> &platforms, float cameraY);
    bool hasSuperKeyGenerated() const { return superKeyGenerated; }
};

#endif
