#include "generators/powerup_generator.h"
#include "player.h"
#include "lava.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <generators/powerup_manager.h>

PowerupGenerator::PowerupGenerator(Player *player, Lava *lava, float checkInterval)
    : player(player), lava(lava), checkInterval(checkInterval), lastCheckedHeight(0.0f), superKeyGenerated(false)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
}

PowerupGenerator::~PowerupGenerator()
{
}

bool PowerupGenerator::shouldSpawnPowerup(float spawnRate)
{
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return random < spawnRate;
}

PowerupType PowerupGenerator::selectRandomPowerup()
{
    int random = rand() % 4;
    switch (random)
    {
    case 0:
        return POWERUP_LAVA_FREEZE;
    case 1:
        return POWERUP_SHIELD;
    case 2:
        return POWERUP_KEY;
    default:
        return POWERUP_LAVA_FREEZE;
    }
}

void PowerupGenerator::generatePowerups(std::vector<Powerup *> &powerups, std::vector<Platform *> &platforms, float cameraY)
{
    if (superKeyGenerated)
    {
        return;
    }

    if (player && player->getKeys() >= KEYS_NEEDED_FOR_SUPER_KEY && !player->getHasSuperKey())
    {
        for (Platform *platform : platforms)
        {
            float platformY = platform->getY();
            if (platformY > cameraY + 5.0f)
            {
                bool hasPowerup = false;
                for (Powerup *powerup : powerups)
                {
                    float distance = abs(powerup->getX() - platform->getX()) + abs(powerup->getY() - platformY);
                    if (distance < POWERUP_CHECK_DISTANCE)
                    {
                        hasPowerup = true;
                        break;
                    }
                }

                if (!hasPowerup)
                {
                    std::cout << "Spawning Super Key at platform Y: " << platformY << std::endl;
                    PowerupManager::getInstance()->clear();
                    SuperKey *superKey = new SuperKey(platform->getX(), platformY + 0.25f, player);
                    powerups.push_back(superKey);
                    superKeyGenerated = true;
                    return;
                }
            }
        }
    }

    if (cameraY - lastCheckedHeight >= checkInterval)
    {
        lastCheckedHeight = cameraY;

        for (Platform *platform : platforms)
        {
            float platformY = platform->getY();
            if (platformY > cameraY - 2.0f && platformY < cameraY + 10.0f)
            {
                bool hasPowerup = false;
                for (Powerup *powerup : powerups)
                {
                    float distance = abs(powerup->getX() - platform->getX()) + abs(powerup->getY() - platformY);
                    if (distance < POWERUP_CHECK_DISTANCE)
                    {
                        hasPowerup = true;
                        break;
                    }
                }

                if (!hasPowerup && platformY > cameraY * 1.5f) // Only spawn above the camera for better gameplay
                {
                    PowerupType type = selectRandomPowerup();
                    Powerup *newPowerup = nullptr;
                    float spawnRate = 0.0f;

                    switch (type)
                    {
                    case POWERUP_LAVA_FREEZE:
                        newPowerup = new LavaFreeze(platform->getX(), platformY + 0.15f, lava);
                        spawnRate = LAVA_FREEZE_SPAWN_RATE;
                        break;
                    case POWERUP_SHIELD:
                        newPowerup = new Shield(platform->getX(), platformY + 0.15f, player);
                        spawnRate = SHIELD_SPAWN_RATE;
                        break;
                    case POWERUP_KEY:
                        newPowerup = new Key(platform->getX(), platformY + 0.15f, player);
                        spawnRate = KEY_SPAWN_RATE;
                        break;
                    }

                    if (newPowerup && shouldSpawnPowerup(spawnRate))
                    {
                        powerups.push_back(newPowerup);
                    }
                    else if (newPowerup)
                    {
                        delete newPowerup;
                    }
                }
            }
        }
    }
}
