#include "generators/powerup_manager.h"
#include "player.h"

PowerupManager *PowerupManager::instance = nullptr;

PowerupManager::PowerupManager() : player(nullptr), damageInvincibilityTimer(0.0f)
{
}

PowerupManager::~PowerupManager()
{
    clear();
}

PowerupManager *PowerupManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new PowerupManager();
    }
    return instance;
}

void PowerupManager::destroy()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void PowerupManager::addPowerup(Powerup *powerup)
{
    if (powerup)
    {
        activePowerups.push_back(ActivePowerup(powerup, powerup->getDuration()));
    }
}

void PowerupManager::activateDamageShield()
{
    if (player)
    {
        damageInvincibilityTimer = DAMAGE_INVINCIBILITY_DURATION;
        player->setInvincible(true);
    }
}

void PowerupManager::update(float deltaTime)
{
    if (damageInvincibilityTimer > 0.0f)
    {
        damageInvincibilityTimer -= deltaTime;
        if (damageInvincibilityTimer <= 0.0f)
        {
            damageInvincibilityTimer = 0.0f;
            if (player)
            {
                player->setInvincible(false);
            }
        }
    }

    for (auto it = activePowerups.begin(); it != activePowerups.end();)
    {
        if (!it->isActivated)
        {
            it->powerup->activate();
            it->isActivated = true;
        }

        it->timeRemaining -= deltaTime;

        if (it->timeRemaining <= 0.0f)
        {
            it->powerup->deactivate();
            it = activePowerups.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PowerupManager::clear()
{
    for (auto &active : activePowerups)
    {
        if (active.isActivated)
        {
            active.powerup->deactivate();
        }
    }
    activePowerups.clear();
}
