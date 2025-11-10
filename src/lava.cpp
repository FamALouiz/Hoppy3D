#include "lava.h"
#include <cmath>

void Lava::defaultDrawFunc(float x, float y)
{
    glBegin(GL_QUADS);
    glColor3f(LAVA_COLOR_R, LAVA_COLOR_G, LAVA_COLOR_B);
    glVertex2f(x - LAVA_WIDTH / 2.0f, y - LAVA_HEIGHT / 2.0f);
    glVertex2f(x + LAVA_WIDTH / 2.0f, y - LAVA_HEIGHT / 2.0f);

    glColor3f(LAVA_COLOR_R * 0.8f, LAVA_COLOR_G * 0.8f, LAVA_COLOR_B);
    glVertex2f(x + LAVA_WIDTH / 2.0f, y + LAVA_HEIGHT / 2.0f);
    glVertex2f(x - LAVA_WIDTH / 2.0f, y + LAVA_HEIGHT / 2.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(LAVA_COLOR_R, LAVA_COLOR_G * 0.5f, LAVA_COLOR_B);
    for (int i = 0; i < 10; i++)
    {
        float xPos = x - LAVA_WIDTH / 2.0f + (LAVA_WIDTH / 10.0f) * i;
        float waveHeight = 0.03f * sin(i * 0.5f + glutGet(GLUT_ELAPSED_TIME) / 300.0f);
        glVertex2f(xPos, y + LAVA_HEIGHT / 2.0f);
        glVertex2f(xPos + LAVA_WIDTH / 20.0f, y + LAVA_HEIGHT / 2.0f + waveHeight);
        glVertex2f(xPos + LAVA_WIDTH / 10.0f, y + LAVA_HEIGHT / 2.0f);
    }
    glEnd();
}

Lava::Lava(float x, float y)
    : PhysicsObject(x, y, LAVA_TERMINAL_VELOCITY, LAVA_TERMINAL_VELOCITY, defaultDrawFunc),
      currentSpeed(LAVA_INITIAL_SPEED), accelerationRate(LAVA_ACCELERATION)
{
    setVelocity(0.0f, LAVA_INITIAL_SPEED);
    setAcceleration(0.0f, 0.0f);
    setCollisionBox(LAVA_WIDTH, LAVA_HEIGHT);
}

Lava::Lava(float x, float y, void (*drawFunc)(float, float))
    : PhysicsObject(x, y, LAVA_TERMINAL_VELOCITY, LAVA_TERMINAL_VELOCITY, drawFunc),
      currentSpeed(LAVA_INITIAL_SPEED), accelerationRate(LAVA_ACCELERATION)
{
    setVelocity(0.0f, LAVA_INITIAL_SPEED);
    setAcceleration(0.0f, 0.0f);
    setCollisionBox(LAVA_WIDTH, LAVA_HEIGHT);
}

Lava::~Lava()
{
}

void Lava::resetSpeed()
{
    currentSpeed = LAVA_INITIAL_SPEED;
    setVelocity(0.0f, LAVA_INITIAL_SPEED);
}
