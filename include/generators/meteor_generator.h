#ifndef METEOR_GENERATOR_H
#define METEOR_GENERATOR_H

#include "physics/objects.h"
#include "meteor.h"
#include <vector>

#define METEOR_SECTIONS 5
#define METEOR_SPACING 1.0f
#define METEOR_SIZE 0.08f
#define METEOR_TERMINAL_VELOCITY_X 5.0f
#define METEOR_TERMINAL_VELOCITY_Y 2.0f
#define METEOR_GRAVITY -20.0f
#define METEOR_COLOR_R 0.8f
#define METEOR_COLOR_G 0.2f
#define METEOR_COLOR_B 0.1f
#define METEOR_BATCH_SIZE 3

class MeteorGenerator
{
private:
    int sections;
    float spacing;
    float meteorSize;
    float screenLeft;
    float screenRight;
    float terminalVelocityX;
    float terminalVelocityY;
    float gravity;
    int batchSize;
    float lastGeneratedHeight = 2.0f;

    float getSectionCenter(int section);

public:
    MeteorGenerator(int sections, float spacing, float meteorSize,
                    float screenLeft, float screenRight,
                    float terminalVelocityX, float terminalVelocityY, float gravity,
                    int batchSize);

    void generateMeteors(std::vector<Meteor *> &meteors, float cameraY);
};

#endif
