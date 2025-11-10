#include "generators/meteor_generator.h"
#include "generators/platform_generator.h"
#include <cstdlib>
#include <ctime>

MeteorGenerator::MeteorGenerator(int sections, float spacing, float meteorSize,
                                 float screenLeft, float screenRight,
                                 float terminalVelocityX, float terminalVelocityY, float gravity,
                                 int batchSize)
    : sections(sections), spacing(spacing), meteorSize(meteorSize),
      screenLeft(screenLeft), screenRight(screenRight),
      terminalVelocityX(terminalVelocityX), terminalVelocityY(terminalVelocityY), gravity(gravity),
      batchSize(batchSize)
{
}

float MeteorGenerator::getSectionCenter(int section)
{
    float sectionWidth = (screenRight - screenLeft) / sections;
    return screenLeft + (section + 0.5f) * sectionWidth;
}

void MeteorGenerator::generateMeteors(std::vector<Meteor *> &meteors, float cameraY)
{
    float generationThreshold = cameraY;

    while (lastGeneratedHeight < generationThreshold)
    {
        lastGeneratedHeight += spacing;

        for (int i = 0; i < batchSize; i++)
        {
            int section = rand() % sections;
            float xPos = getSectionCenter(section);

            Meteor *meteor = new Meteor(xPos, lastGeneratedHeight + meteorSize * 2, meteorSize, terminalVelocityX, terminalVelocityY, gravity);
            meteors.push_back(meteor);
        }
    }
}
