#include <GL/glut.h>
#include "screens/main_screen.h"
#include "screens/end_screen.h"
#include "screens/win_screen.h"
#include "game_window/game_window.h"
#include "physics/core.h"
#include <iostream>
#include <cmath>

MainScreen::MainScreen() : platformGenerator(nullptr), meteorGenerator(nullptr), powerupGenerator(nullptr), player(nullptr), lava(nullptr), gate(nullptr), background(nullptr), hud(nullptr), doorOpening(false)
{
}

MainScreen::~MainScreen()
{
    if (player)
    {
        delete player;
        player = nullptr;
    }

    if (lava)
    {
        delete lava;
        lava = nullptr;
    }

    if (gate)
    {
        delete gate;
        gate = nullptr;
    }

    if (background)
    {
        delete background;
        background = nullptr;
    }

    if (hud)
    {
        delete hud;
        hud = nullptr;
    }

    for (PhysicsObject *obj : objects)
    {
        delete obj;
    }
    objects.clear();

    for (Meteor *meteor : meteors)
    {
        delete meteor;
    }
    meteors.clear();

    for (Powerup *powerup : powerups)
    {
        delete powerup;
    }
    powerups.clear();

    for (Platform *platform : platforms)
    {
        delete platform;
    }
    platforms.clear();

    if (platformGenerator)
    {
        delete platformGenerator;
        platformGenerator = nullptr;
    }

    if (meteorGenerator)
    {
        delete meteorGenerator;
        meteorGenerator = nullptr;
    }

    if (powerupGenerator)
    {
        delete powerupGenerator;
        powerupGenerator = nullptr;
    }

    PowerupManager::destroy();
}

void MainScreen::init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    background = new Background();

    Platform *ground = new Platform(0.0f, -0.8f);
    addPlatform(ground);

    float platformTop = ground->getY() + ground->getCollisionHeight() / 2.0f;
    player = new Player(0.0f, platformTop);

    auto platformDrawFunc = [](float x, float y)
    {
        glBegin(GL_QUADS);
        glColor3f(PLATFORM_COLOR_R, PLATFORM_COLOR_G, PLATFORM_COLOR_B);
        glVertex2f(x - PLATFORM_WIDTH / 2.0f, y - PLATFORM_HEIGHT / 2.0f);
        glVertex2f(x + PLATFORM_WIDTH / 2.0f, y - PLATFORM_HEIGHT / 2.0f);
        glVertex2f(x + PLATFORM_WIDTH / 2.0f, y + PLATFORM_HEIGHT / 2.0f);
        glVertex2f(x - PLATFORM_WIDTH / 2.0f, y + PLATFORM_HEIGHT / 2.0f);
        glEnd();
    };

    platformGenerator = new PlatformGenerator(
        SCREEN_SECTIONS,
        PLATFORM_SPACING,
        PLATFORM_WIDTH,
        PLATFORM_HEIGHT,
        SCREEN_LEFT,
        SCREEN_RIGHT,
        GENERATION_AHEAD,
        platformDrawFunc);

    meteorGenerator = new MeteorGenerator(
        METEOR_SECTIONS,
        METEOR_SPACING,
        METEOR_SIZE,
        SCREEN_LEFT,
        SCREEN_RIGHT,
        METEOR_TERMINAL_VELOCITY_X,
        METEOR_TERMINAL_VELOCITY_Y,
        METEOR_GRAVITY,
        METEOR_BATCH_SIZE);

    lava = new Lava(0.0f, SCREEN_BOTTOM - LAVA_HEIGHT / 2.0f - 0.3f);

    hud = new HUD(player, lava);

    powerupGenerator = new PowerupGenerator(player, lava);
    PowerupManager::getInstance()->setPlayer(player);
}

void MainScreen::update(float deltaTime)
{
    if (player)
    {
        player->updateControls(deltaTime);
        player->update(deltaTime);

        player->checkBoundaries(SCREEN_LEFT - 0.1f, SCREEN_RIGHT + 0.1f);

        if (platformGenerator)
        {
            platformGenerator->generatePlatforms(platforms, player->getY());
        }

        if (meteorGenerator)
        {
            meteorGenerator->generateMeteors(meteors, player->getY());
        }

        if (powerupGenerator)
        {
            powerupGenerator->generatePowerups(powerups, platforms, player->getY());

            if (powerupGenerator->hasSuperKeyGenerated() && !gate)
            {
                gate = new Gate(0.0f, player->getY() + GATE_DISTANCE_ABOVE_PLAYER);
                platforms.push_back(gate);
            }
        }

        std::vector<_Object *> collisions = player->getCollidingObjects();
        player->handleCollisions(collisions);

        if (gate && player->getHasSuperKey() && !doorOpening)
        {
            if (player->getCollisionBox() && gate->getCollisionBox())
            {
                if (CollisionDetector::getInstance()->checkCollision(player->getCollisionBox(), gate->getCollisionBox()))
                {
                    doorOpening = true;
                    gate->startOpening();
                    player->setInvincible(true);
                    player->setVelocity(0.0f, 0.0f);
                    player->setAcceleration(0.0f, 0.0f);
                    player->setGrounded(true);
                    player->setIsFrozen(true);
                }
            }
        }

        if (doorOpening && gate && gate->getIsOpen())
        {
            WinScreen *winScreen = new WinScreen();
            GameWindow::getInstance()->setScreen(winScreen);
        }

        for (auto it = powerups.begin(); it != powerups.end();)
        {
            Powerup *powerup = *it;
            powerup->update(deltaTime);

            if (!powerup->getIsCollected() && player->getCollisionBox() && powerup->getCollisionBox())
            {
                if (CollisionDetector::getInstance()->checkCollision(player->getCollisionBox(), powerup->getCollisionBox()))
                {
                    powerup->setCollected(true);
                    PowerupManager::getInstance()->addPowerup(powerup);
                    it = powerups.erase(it);
                    continue;
                }
            }
            ++it;
        }
        if (player->getLives() <= 0)
        {
            EndScreen *endScreen = new EndScreen();
            GameWindow::getInstance()->setScreen(endScreen);
        }
    }

    PowerupManager::getInstance()->update(deltaTime);

    if (lava)
    {
        lava->update(deltaTime);
    }

    if (gate)
    {
        gate->update(deltaTime);
    }

    for (PhysicsObject *obj : objects)
    {
        if (obj != player)
            obj->update(deltaTime);
    }

    for (Meteor *meteor : meteors)
    {
        meteor->update(deltaTime);

        if (lava && lava->getCollisionBox() && meteor->getCollisionBox())
        {
            if (CollisionDetector::getInstance()->checkCollision(lava->getCollisionBox(), meteor->getCollisionBox()))
            {
                meteor->markForRemoval();
            }
        }
    }

    for (auto it = meteors.begin(); it != meteors.end();)
    {
        if ((*it)->getShouldRemove())
        {
            delete *it;
            it = meteors.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MainScreen::display()
{
    float camY = 0.0f;

    if (player)
    {
        camY = player->getY();

        glLoadIdentity();
        gluLookAt(0.0f, camY, 1.0f, 0.0f, camY, 0.0f, 0.0f, 1.0f, 0.0f);
    }
    else
    {
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }

    if (background)
    {
        background->draw(camY);
    }

    if (player)
    {
        if (player->getIsInvincible())
        {
            glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
            player->draw();
        }
        else
        {
            glColor3f(PLAYER_COLOR_R, PLAYER_COLOR_G, PLAYER_COLOR_B);
            player->draw();
        }
    }

    for (Platform *platform : platforms)
    {
        if (platform != gate)
        {
            platform->draw();
        }
    }

    for (Powerup *powerup : powerups)
    {
        powerup->draw();
    }

    if (lava)
    {
        lava->draw();
    }

    for (Meteor *meteor : meteors)
    {
        meteor->draw();
    }

    glLoadIdentity();
    gluLookAt(0.0f, camY, 1.0f, 0.0f, camY, 0.0f, 0.0f, 1.0f, 0.0f);

    if (hud)
    {
        for (Meteor *meteor : meteors)
        {
            if (meteor->getShowWarning() && meteor->getY() > camY + 0.8f)
            {
                float warningY = camY + 0.88f;
                float warningX = meteor->getWarningX();
                float warningSize = 0.08f;

                float flashSpeed = 10.0f;
                float alpha = 0.5f + 0.5f * sin(meteor->getWarningTimer() * flashSpeed);

                hud->drawWarningSprite(warningX - warningSize / 2.0f, warningY, warningSize, alpha);
            }
            else if (meteor->getShowWarning())
            {
                meteor->setShowWarning(false);
            }
        }
    }

    for (PhysicsObject *obj : objects)
    {
        obj->draw();
    }

    if (gate)
    {
        gate->draw();
    }

    if (hud)
    {
        hud->draw(camY);
    }
}

Player *MainScreen::getPlayer()
{
    return player;
}

Lava *MainScreen::getLava()
{
    return lava;
}

void MainScreen::handleKeyboardUp(unsigned char key, int x, int y)
{
    if (player)
    {
        player->handleKeyUp(key);
    }
}

void MainScreen::handleKeyboardDown(unsigned char key, int x, int y)
{
    if (player)
    {
        player->handleKeyDown(key);
    }
}

void MainScreen::handleSpecialKeysDown(int key, int x, int y)
{
    if (player)
    {
        player->handleSpecialKeyDown(key);
    }
}

void MainScreen::handleSpecialKeysUp(int key, int x, int y)
{
    if (player)
    {
        player->handleSpecialKeyUp(key);
    }
}