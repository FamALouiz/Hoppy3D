#include "generators/powerups/powerups.h"
#include <cmath>
#include <iostream>

Powerup::Powerup(float x, float y, PowerupType type, float spawnRate, float duration, void (*drawFunc)(float, float))
    : StaticObject(x, y, drawFunc), type(type), spawnRate(spawnRate), duration(duration), isActive(false), isCollected(false), animationTimer(0.0f), baseY(y), baseX(x)
{
    setCollisionBox(POWERUP_SIZE * 2.0f, POWERUP_SIZE * 2.0f);
}

Powerup::~Powerup()
{
}

void Powerup::update(float deltaTime)
{
    animationTimer += deltaTime;
    float offset = sin(animationTimer * POWERUP_BOB_SPEED) * POWERUP_BOB_AMPLITUDE;
    setPosition(x, baseY + offset);
}
