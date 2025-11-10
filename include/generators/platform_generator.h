#ifndef PLATFORM_GENERATOR_H
#define PLATFORM_GENERATOR_H

#include "physics/objects.h"
#include "platform.h"
#include <vector>

#define SCREEN_SECTIONS 5
#define PLATFORM_SPACING 0.5f
#define SCREEN_LEFT -0.5f
#define SCREEN_RIGHT 0.5f
#define SCREEN_BOTTOM -1.0f
#define SCREEN_TOP 1.0f
#define GENERATION_AHEAD 10.0f

class PlatformGenerator
{
private:
    float lastGeneratedHeight;
    int sections;
    float spacing;
    float platformWidth;
    float platformHeight;
    float screenLeft;
    float screenRight;
    float generationAhead;
    int lastSection = 2;

    float getSectionCenter(int section);
    void (*platformDrawFunc)(float, float);

public:
    PlatformGenerator(int sections, float spacing, float platformWidth, float platformHeight,
                      float screenLeft, float screenRight, float generationAhead,
                      void (*drawFunc)(float, float));

    void generatePlatforms(std::vector<Platform *> &platforms, float cameraY);
};

#endif
