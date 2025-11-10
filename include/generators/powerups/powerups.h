#ifndef POWERUPS_H
#define POWERUPS_H

#include "physics/objects.h"
#include <GL/glut.h>

#define POWERUP_SIZE 0.04f
#define POWERUP_BOB_SPEED 3.5f
#define POWERUP_BOB_AMPLITUDE 0.015f

enum PowerupType
{
    POWERUP_LAVA_FREEZE,
    POWERUP_SHIELD,
    POWERUP_KEY,
    POWERUP_SUPER_KEY
};

class Powerup : public StaticObject
{
protected:
    PowerupType type;
    float spawnRate;
    float duration;
    bool isActive;
    bool isCollected;
    float animationTimer;
    float baseY;
    float baseX;

public:
    Powerup(float x, float y, PowerupType type, float spawnRate, float duration, void (*drawFunc)(float, float));
    virtual ~Powerup();

    PowerupType getType() const { return type; }
    float getSpawnRate() const { return spawnRate; }
    float getDuration() const { return duration; }
    bool getIsActive() const { return isActive; }
    bool getIsCollected() const { return isCollected; }
    void setCollected(bool collected) { isCollected = collected; }
    void update(float deltaTime) override;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

#endif
