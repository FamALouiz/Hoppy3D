#ifndef POWERUP_MANAGER_H
#define POWERUP_MANAGER_H

#include "generators/powerups/powerups.h"
#include <vector>

#define DAMAGE_INVINCIBILITY_DURATION 2.0f

struct ActivePowerup
{
    Powerup *powerup;
    float timeRemaining;
    bool isActivated;

    ActivePowerup(Powerup *p, float time) : powerup(p), timeRemaining(time), isActivated(false) {}
};

class Player;

class PowerupManager
{
private:
    static PowerupManager *instance;
    std::vector<ActivePowerup> activePowerups;
    Player *player;
    float damageInvincibilityTimer;

    PowerupManager();
    PowerupManager(const PowerupManager &) = delete;
    PowerupManager &operator=(const PowerupManager &) = delete;

public:
    ~PowerupManager();

    static PowerupManager *getInstance();
    static void destroy();

    void setPlayer(Player *p) { player = p; }
    void addPowerup(Powerup *powerup);
    void activateDamageShield();
    void update(float deltaTime);
    void clear();
    bool hasDamageShield() const { return damageInvincibilityTimer > 0.0f; }
    const std::vector<ActivePowerup> &getActivePowerups() const { return activePowerups; }
};

#endif
