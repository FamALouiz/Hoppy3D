#include "player.h"
#include <iostream>
#include <cmath>
#include "platform.h"
#include <lava.h>
#include "generators/powerup_manager.h"
#include "stb_image.h"
#include "game_window/game_window.h"
#include "screens/main_screen.h"

GLuint Player::spriteTexture = 0;
bool Player::textureLoaded = false;

void Player::loadTexture()
{
    if (textureLoaded)
        return;

    int width, height, channels;
    unsigned char *data = nullptr;

    data = stbi_load(PLAYER_SPRITE_PATH, &width, &height, &channels, 4);
    if (!data)
    {
        std::cerr << "Failed to load player texture from all paths" << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    glGenTextures(1, &spriteTexture);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    textureLoaded = true;
}

int Player::getCurrentSpriteIndex()
{
    if (isHit)
    {
        return HIT_ROW * PLAYER_SPRITE_COLS + (currentFrame % HIT_FRAMES);
    }

    if (!getIsGrounded())
    {
        return JUMP_ROW * PLAYER_SPRITE_COLS + JUMP_COL;
    }

    if (moveLeft || moveRight)
    {
        return RUN_ROW * PLAYER_SPRITE_COLS + (currentFrame % RUN_FRAMES);
    }

    return IDLE_ROW * PLAYER_SPRITE_COLS + (currentFrame % IDLE_FRAMES);
}

void Player::updateAnimation(float deltaTime)
{
    if (isHit)
    {
        hitAnimationTimer += deltaTime;

        int hitFrame = (int)(hitAnimationTimer / ANIMATION_SPEED);

        if (hitFrame >= HIT_FRAMES)
        {
            isHit = false;
            hitAnimationTimer = 0.0f;
            currentFrame = 0;
            animationTimer = 0.0f;
        }
        else
        {
            currentFrame = hitFrame;
        }
        return;
    }

    int maxFrames = IDLE_FRAMES;

    if (!getIsGrounded())
    {
        maxFrames = 1;
    }
    else if (moveLeft || moveRight)
    {
        maxFrames = RUN_FRAMES;
    }

    animationTimer += deltaTime;
    if (animationTimer >= ANIMATION_SPEED)
    {
        currentFrame++;
        animationTimer = 0.0f;
        if (currentFrame >= maxFrames)
            currentFrame = 0;
    }
}

void Player::defaultDrawFunc(float x, float y)
{
    if (!textureLoaded)
        loadTexture();

    if (!textureLoaded)
    {
        glBegin(GL_QUADS);
        glColor3f(PLAYER_COLOR_R, PLAYER_COLOR_G, PLAYER_COLOR_B);
        glVertex2f(x - PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2);
        glVertex2f(x + PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2);
        glVertex2f(x + PLAYER_WIDTH / 2, y + PLAYER_HEIGHT / 2);
        glVertex2f(x - PLAYER_WIDTH / 2, y + PLAYER_HEIGHT / 2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.9f, 0.7f, 0.3f);
        glVertex2f(x, y + PLAYER_HEIGHT / 2);
        glVertex2f(x - PLAYER_WIDTH / 3, y + PLAYER_HEIGHT / 4);
        glVertex2f(x + PLAYER_WIDTH / 3, y + PLAYER_HEIGHT / 4);
        glEnd();

        const int segments = 8;
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.8f, 0.5f, 0.2f);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++)
        {
            float angle = 3.14159f * i / segments;
            float cx = x + (PLAYER_WIDTH / 4) * cos(angle);
            float cy = y + (PLAYER_HEIGHT / 4) * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glVertex2f(x - PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 4);
        glVertex2f(x + PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 4);
        glVertex2f(x + PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 2);
        glVertex2f(x - PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 2);
        glEnd();

        return;
    }

    Player *player = nullptr;
    MainScreen *mainScreen = dynamic_cast<MainScreen *>(GameWindow::getInstance()->getScreen());

    if (mainScreen)
    {
        player = mainScreen->getPlayer();
    }
    int spriteIndex = 0;
    bool facingRight = true;

    if (player)
    {
        spriteIndex = player->getCurrentSpriteIndex();
        facingRight = player->facingRight;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);

    float alpha = 1.0f;
    if (player && player->getIsFlashing())
    {
        float flashTime = fmod(player->getFlashingDuration() * 10.0f, 1.0f);
        alpha = 0.5f + 0.2f * flashTime;
    }

    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    int row = spriteIndex / PLAYER_SPRITE_COLS;
    int col = spriteIndex % PLAYER_SPRITE_COLS;

    float texLeft = (float)col / PLAYER_SPRITE_COLS;
    float texRight = (float)(col + 1) / PLAYER_SPRITE_COLS;
    float texTop = (float)row / PLAYER_SPRITE_ROWS;
    float texBottom = (float)(row + 1) / PLAYER_SPRITE_ROWS;

    if (!facingRight)
    {
        float temp = texLeft;
        texLeft = texRight;
        texRight = temp;
    }

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x - PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + PLAYER_WIDTH / 2, y + PLAYER_HEIGHT / 2);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x - PLAYER_WIDTH / 2, y + PLAYER_HEIGHT / 2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.9f, 0.7f, 0.3f);
    glVertex2f(x, y + PLAYER_HEIGHT / 2);
    glVertex2f(x - PLAYER_WIDTH / 3, y + PLAYER_HEIGHT / 4);
    glVertex2f(x + PLAYER_WIDTH / 3, y + PLAYER_HEIGHT / 4);
    glEnd();

    const int segments = 8;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.8f, 0.5f, 0.2f);
    glVertex2f(x, y);
    for (int i = 0; i <= segments; i++)
    {
        float angle = 3.14159f * i / segments;
        float cx = x + (PLAYER_WIDTH / 4) * cos(angle);
        float cy = y + (PLAYER_HEIGHT / 4) * sin(angle);
        glVertex2f(cx, cy);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(x - PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 4);
    glVertex2f(x + PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 4);
    glVertex2f(x + PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 2);
    glVertex2f(x - PLAYER_WIDTH / 4, y - PLAYER_HEIGHT / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

Player::Player(float x, float y)
    : PhysicsObject(x, y, PLAYER_TERMINAL_VELOCITY_X, PLAYER_TERMINAL_VELOCITY_Y, defaultDrawFunc),
      moveLeft(false), moveRight(false), canJump(false), animationTimer(0.0f), currentFrame(0), hitAnimationTimer(0.0f), isHit(false)
{
    setVelocity(0.0f, 0.0f);
    setAcceleration(0.0f, GRAVITY);
    setCollisionBox(PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Player::Player(float x, float y, void (*drawFunc)(float, float))
    : PhysicsObject(x, y, PLAYER_TERMINAL_VELOCITY_X, PLAYER_TERMINAL_VELOCITY_Y, drawFunc),
      moveLeft(false), moveRight(false), canJump(false), animationTimer(0.0f), currentFrame(0), hitAnimationTimer(0.0f), isHit(false)
{
    setVelocity(0.0f, 0.0f);
    setAcceleration(0.0f, GRAVITY);
    setCollisionBox(PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}

Player::~Player()
{
}

void Player::handleKeyDown(int key)
{
}

void Player::handleKeyUp(int key)
{
}

void Player::handleSpecialKeyDown(int key)
{
    if (getIsFrozen())
    {
        moveLeft = false;
        moveRight = false;
        canJump = false;
        return;
    }
    if (key == GLUT_KEY_LEFT)
    {
        moveLeft = true;
    }
    if (key == GLUT_KEY_RIGHT)
    {
        moveRight = true;
    }
    if (key == GLUT_KEY_UP)
    {
        canJump = true;
    }
}

void Player::handleSpecialKeyUp(int key)
{
    if (getIsFrozen())
    {
        moveLeft = false;
        moveRight = false;
        canJump = false;
        return;
    }
    if (key == GLUT_KEY_LEFT)
    {
        moveLeft = false;
    }
    if (key == GLUT_KEY_RIGHT)
    {
        moveRight = false;
    }
    if (key == GLUT_KEY_UP)
    {
        canJump = false;
    }
}

void Player::updateControls(float deltaTime)
{
    float currentVelX = 0.0f;

    if (moveLeft)
    {
        currentVelX = -PLAYER_MOVEMENT_SPEED;
        facingRight = false;
    }
    if (moveRight)
    {
        currentVelX = PLAYER_MOVEMENT_SPEED;
        facingRight = true;
    }

    setVelocity(currentVelX, getVelocityY());

    if (canJump && getIsGrounded())
    {
        setVelocity(getVelocityX(), PLAYER_JUMP_SPEED);
    }
}

void Player::update(float deltaTime)
{
    updateAnimation(deltaTime);

    if (flashingDuration > 0)
    {
        flashingDuration -= deltaTime;
        isFlashing = true;

        if (flashingDuration <= 0)
        {
            flashingDuration = 0;
            isFlashing = false;
        }
    }

    PhysicsObject::update(deltaTime);
}

void Player::handleCollisions(const std::vector<_Object *> &collisions)
{

    bool grounded = false;

    for (_Object *obj : collisions)
    {
        if (typeid(*obj) == typeid(Platform))
        {
            Platform *platform = dynamic_cast<Platform *>(obj);
            if (isCollidingFromTop(platform))
            {
                float platformTop = platform->getY() + platform->getCollisionHeight() / 2.0f;
                setPosition(getX(), platformTop + getCollisionHeight() / 2.0f);

                if (getVelocityY() < 0)
                {
                    setVelocity(getVelocityX(), 0.0f);
                }

                grounded = true;
            }
            else if (isCollidingFromBottom(platform))
            {
                float platformBottom = platform->getY() - platform->getCollisionHeight() / 2.0f;
                setPosition(getX(), platformBottom - getCollisionHeight() / 2.0f);

                if (getVelocityY() > 0)
                {
                    setVelocity(getVelocityX(), 0.0f);
                }
            }
            else if (isCollidingFromLeft(platform))
            {
                float platformLeft = platform->getX() - platform->getCollisionWidth() / 2.0f;
                setPosition(platformLeft - getCollisionWidth() / 2.0f, getY());
                setVelocity(0.0f, getVelocityY());
            }
            else if (isCollidingFromRight(platform))
            {
                float platformRight = platform->getX() + platform->getCollisionWidth() / 2.0f;
                setPosition(platformRight + getCollisionWidth() / 2.0f, getY());
                setVelocity(0.0f, getVelocityY());
            }
        }
        else if (typeid(*obj) == typeid(Lava) && !getIsInvincible())
        {
            loseLife();
            loseLife();
            loseLife();
            PowerupManager::getInstance()->activateDamageShield();
            std::cout << "Player hit by lava! Lives left: " << getLives() << std::endl;
        }
        else if (typeid(*obj) == typeid(Meteor) && !getIsInvincible())
        {
            loseLife();
            PowerupManager::getInstance()->activateDamageShield();
            std::cout << "Player hit by meteor! Lives left: " << getLives() << std::endl;
            setFlashingDuration(DAMAGE_INVINCIBILITY_DURATION);

            Meteor *meteorObj = dynamic_cast<Meteor *>(obj);
            if (getCollisionBox() && meteorObj && meteorObj->getCollisionBox())
            {
                if (CollisionDetector::getInstance()->checkCollision(getCollisionBox(), meteorObj->getCollisionBox()))
                {
                    meteorObj->markForRemoval();
                }
            }
        }
    }

    setGrounded(grounded);

    if (grounded || getIsFrozen())
    {
        setAcceleration(0.0f, 0.0f);
    }
    else
    {
        setAcceleration(0.0f, GRAVITY);
    }
}

void Player::checkBoundaries(float screenLeft, float screenRight)
{
    float playerX = getX();
    float playerWidth = getCollisionWidth() / 2.0f;

    if (playerX - playerWidth < screenLeft)
    {
        setPosition(screenLeft + playerWidth, getY());
        setVelocity(0.0f, getVelocityY());
    }
    else if (playerX + playerWidth > screenRight)
    {
        setPosition(screenRight - playerWidth, getY());
        setVelocity(0.0f, getVelocityY());
    }
}

void Player::cleanupTexture()
{
    if (textureLoaded)
    {
        glDeleteTextures(1, &spriteTexture);
        textureLoaded = false;
    }
}
