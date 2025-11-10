#ifndef METEOR_H
#define METEOR_H

#include "physics/objects.h"
#include <GL/glut.h>

#define METEOR_SPRITE_PATH "C:\\Users\\fam\\OneDrive\\Desktop\\GIU\\Semester 7\\Graphics\\Hoppy\\assets\\sprites\\meteor.png"
#define METEOR_ROTATION_SPEED 180.0f

class Meteor : public PhysicsObject
{
private:
    float rotation;
    float meteorSize;
    bool showWarning;
    float warningX;
    float warningTimer;
    static GLuint texture;
    static bool textureLoaded;
    static Meteor *currentDrawingMeteor;
    static void loadTexture();
    static void defaultDrawFunc(float x, float y);

public:
    Meteor(float x, float y, float size, float terminalVelocityX, float terminalVelocityY, float gravity);
    ~Meteor();

    void draw() override;
    void update(float deltaTime) override;
    void setShowWarning(bool show) { showWarning = show; }
    bool getShowWarning() const { return showWarning; }
    float getWarningX() const { return warningX; }
    float getWarningTimer() const { return warningTimer; }
    static void cleanupTexture();
};

#endif
