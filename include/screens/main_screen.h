#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "game_window/game_screen.h"
#include <string>
#include <GL/glut.h>
#include "physics/objects.h"
#include "player.h"
#include "platform.h"
#include "gate.h"
#include "lava.h"
#include "background.h"
#include "hud.h"
#include "generators/platform_generator.h"
#include "generators/meteor_generator.h"
#include "generators/powerup_generator.h"
#include "generators/powerup_manager.h"
#include "generators/powerups/powerups.h"
#include "meteor.h"
#include <vector>

#define GATE_DISTANCE_ABOVE_PLAYER 15.0f

class MainScreen : public GameScreen
{
private:
    std::vector<Platform *> platforms;
    std::vector<PhysicsObject *> objects;
    std::vector<Meteor *> meteors;
    std::vector<Powerup *> powerups;
    PlatformGenerator *platformGenerator;
    MeteorGenerator *meteorGenerator;
    PowerupGenerator *powerupGenerator;
    Player *player;
    Lava *lava;
    Gate *gate;
    Background *background;
    HUD *hud;
    bool doorOpening;

public:
    MainScreen();
    ~MainScreen();

    void addPlatform(Platform *platform) { platforms.push_back(platform); }
    void addObject(PhysicsObject *obj) { objects.push_back(obj); }
    void addMeteor(Meteor *meteor) { meteors.push_back(meteor); }
    void addPowerup(Powerup *powerup) { powerups.push_back(powerup); }
    Player *getPlayer();
    Lava *getLava();
    void init() override;
    void update(float deltaTime) override;
    void display() override;
    void handleKeyboardDown(unsigned char key, int x, int y) override;
    void handleSpecialKeysUp(int key, int x, int y) override;
    void handleSpecialKeysDown(int key, int x, int y) override;
    void handleKeyboardUp(unsigned char key, int x, int y) override;
};

#endif
