#include "generators/platform_generator.h"
#include <cstdlib>
#include <ctime>

PlatformGenerator::PlatformGenerator(int sections, float spacing, float platformWidth, float platformHeight,
                                     float screenLeft, float screenRight, float generationAhead,
                                     void (*drawFunc)(float, float))
    : sections(sections), spacing(spacing), platformWidth(platformWidth), platformHeight(platformHeight),
      screenLeft(screenLeft), screenRight(screenRight), generationAhead(generationAhead),
      platformDrawFunc(drawFunc), lastGeneratedHeight(SCREEN_BOTTOM)
{
}

float PlatformGenerator::getSectionCenter(int section)
{
    float sectionWidth = (screenRight - screenLeft) / sections;
    return screenLeft + (section + 0.5f) * sectionWidth;
}

void PlatformGenerator::generatePlatforms(std::vector<Platform *> &platforms, float cameraY)
{
    float generationThreshold = cameraY + generationAhead;

    while (lastGeneratedHeight < generationThreshold)
    {
        lastGeneratedHeight += spacing;

        int section = rand() % sections;
        if (section == lastSection)
            section = (section + 1) % sections;
        float xPos = getSectionCenter(section);

        int tiles = PLATFORM_MIN_TILES + rand() % (PLATFORM_MAX_TILES - PLATFORM_MIN_TILES + 1);
        Platform *platform = new Platform(xPos, lastGeneratedHeight, tiles);
        platforms.push_back(platform);

        lastSection = section;
    }
}
