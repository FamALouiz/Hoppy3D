#ifndef PLAYER_H
#define PLAYER_H

#include "physics/objects.h"
#include "physics/core.h"
#include <GL/glut.h>

#define PLAYER_WIDTH 0.2f
#define PLAYER_HEIGHT 0.25f
#define PLAYER_TERMINAL_VELOCITY_X 1.5f
#define PLAYER_TERMINAL_VELOCITY_Y 1.5f
#define PLAYER_MOVEMENT_SPEED 25.0f
#define PLAYER_JUMP_SPEED 50.0f
#define PLAYER_COLOR_R 0.0f
#define PLAYER_COLOR_G 0.8f
#define PLAYER_COLOR_B 0.0f

#define PLAYER_SPRITE_COLS 8
#define PLAYER_SPRITE_ROWS 8
#define PLAYER_SPRITE_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\knight.png"

#define IDLE_ROW 0
#define IDLE_FRAMES 4
#define RUN_ROW 2
#define RUN_FRAMES 8
#define HIT_ROW 6
#define HIT_FRAMES 4
#define JUMP_ROW 5
#define JUMP_COL 0

#define ANIMATION_SPEED 0.07f

class Player : public PhysicsObject
{
private:
    bool moveLeft;
    bool moveRight;
    bool canJump;
    bool isInvincible = false;
    int lives = 3;
    int keys = 0;
    bool hasSuperKey = false;
    bool facingRight = true;
    float flashingDuration = 0;
    bool isFlashing = false;

    static GLuint spriteTexture;
    static bool textureLoaded;

    float animationTimer;
    int currentFrame;
    float hitAnimationTimer;
    bool isHit;
    bool isFrozen = false;

    static void loadTexture();
    static void defaultDrawFunc(float x, float y);
    void updateAnimation(float deltaTime);
    int getCurrentSpriteIndex();

public:
    Player(float x, float y);
    Player(float x, float y, void (*drawFunc)(float, float));
    ~Player();

    void handleKeyDown(int key);
    void handleKeyUp(int key);
    void handleSpecialKeyDown(int key);
    void handleSpecialKeyUp(int key);
    void updateControls(float deltaTime);
    void handleCollisions(const std::vector<_Object *> &collisions);
    void checkBoundaries(float screenLeft, float screenRight);
    void update(float deltaTime) override;
    void loseLife()
    {
        if (lives > 0)
        {
            lives--;
            isHit = true;
            hitAnimationTimer = 0.0f;
            currentFrame = 0;
        }
    }
    bool isDead() const { return lives <= 0; }
    int getLives() const { return lives; }
    bool getIsInvincible() const { return isInvincible; }
    void setInvincible(bool invincible) { isInvincible = invincible; }
    int getKeys() const { return keys; }
    void addKey() { keys++; }
    bool getHasSuperKey() const { return hasSuperKey; }
    void setSuperKey(bool superKey) { hasSuperKey = superKey; }
    static void cleanupTexture();
    void setFlashingDuration(float duration) { flashingDuration = duration; }
    float getFlashingDuration() const { return flashingDuration; }
    bool getIsFlashing() const { return isFlashing; }
    void setIsFlashing(bool flashing) { isFlashing = flashing; }
    bool getIsFrozen() const { return isFrozen; }
    void setIsFrozen(bool frozen) { isFrozen = frozen; }
};

#endif
