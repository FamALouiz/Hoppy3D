#include "gate.h"
#include <cmath>

Gate *Gate::currentDrawingGate = nullptr;

void Gate::defaultDrawFunc(float x, float y)
{
    if (!currentDrawingGate)
        return;

    float openProgress = currentDrawingGate->openProgress;
    bool isOpen = currentDrawingGate->isOpen;

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.3f, 0.2f);
    glVertex2f(x - GATE_WIDTH / 2.0f - 0.05f, y - GATE_HEIGHT / 2.0f);
    glVertex2f(x - GATE_WIDTH / 2.0f, y - GATE_HEIGHT / 2.0f);
    glVertex2f(x - GATE_WIDTH / 2.0f, y + GATE_HEIGHT / 2.0f);
    glVertex2f(x - GATE_WIDTH / 2.0f - 0.05f, y + GATE_HEIGHT / 2.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.3f, 0.2f);
    glVertex2f(x + GATE_WIDTH / 2.0f, y - GATE_HEIGHT / 2.0f);
    glVertex2f(x + GATE_WIDTH / 2.0f + 0.05f, y - GATE_HEIGHT / 2.0f);
    glVertex2f(x + GATE_WIDTH / 2.0f + 0.05f, y + GATE_HEIGHT / 2.0f);
    glVertex2f(x + GATE_WIDTH / 2.0f, y + GATE_HEIGHT / 2.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.3f, 0.2f);
    glVertex2f(x - GATE_WIDTH / 2.0f - 0.05f, y + GATE_HEIGHT / 2.0f);
    glVertex2f(x + GATE_WIDTH / 2.0f + 0.05f, y + GATE_HEIGHT / 2.0f);
    glVertex2f(x + GATE_WIDTH / 2.0f + 0.05f, y + GATE_HEIGHT / 2.0f + 0.05f);
    glVertex2f(x - GATE_WIDTH / 2.0f - 0.05f, y + GATE_HEIGHT / 2.0f + 0.05f);
    glEnd();

    float leftDoorOffset = -GATE_WIDTH / 2.0f * openProgress;
    float rightDoorOffset = GATE_WIDTH / 2.0f * openProgress;

    if (!isOpen || openProgress < 1.0f)
    {
        glBegin(GL_QUADS);
        glColor3f(GATE_COLOR_R, GATE_COLOR_G, GATE_COLOR_B);
        glVertex2f(x - GATE_WIDTH / 2.0f + leftDoorOffset, y - GATE_HEIGHT / 2.0f);
        glVertex2f(x + leftDoorOffset, y - GATE_HEIGHT / 2.0f);
        glVertex2f(x + leftDoorOffset, y + GATE_HEIGHT / 2.0f);
        glVertex2f(x - GATE_WIDTH / 2.0f + leftDoorOffset, y + GATE_HEIGHT / 2.0f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.2f, 0.1f);
        glVertex2f(x - GATE_WIDTH / 2.0f + 0.05f + leftDoorOffset, y);
        glVertex2f(x - 0.05f + leftDoorOffset, y);
        glVertex2f(x - 0.05f + leftDoorOffset, y + GATE_HEIGHT / 2.0f - 0.1f);
        glVertex2f(x - GATE_WIDTH / 2.0f + 0.05f + leftDoorOffset, y + GATE_HEIGHT / 2.0f - 0.1f);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glColor3f(1.0f, 0.84f, 0.0f);
        glVertex2f(x - GATE_WIDTH / 4.0f - 0.03f + leftDoorOffset, y + 0.1f);
        glVertex2f(x - GATE_WIDTH / 4.0f + 0.03f + leftDoorOffset, y + 0.1f);
        glVertex2f(x - GATE_WIDTH / 4.0f + 0.03f + leftDoorOffset, y + 0.18f);
        glVertex2f(x - GATE_WIDTH / 4.0f - 0.03f + leftDoorOffset, y + 0.18f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(GATE_COLOR_R, GATE_COLOR_G, GATE_COLOR_B);
        glVertex2f(x + rightDoorOffset, y - GATE_HEIGHT / 2.0f);
        glVertex2f(x + GATE_WIDTH / 2.0f + rightDoorOffset, y - GATE_HEIGHT / 2.0f);
        glVertex2f(x + GATE_WIDTH / 2.0f + rightDoorOffset, y + GATE_HEIGHT / 2.0f);
        glVertex2f(x + rightDoorOffset, y + GATE_HEIGHT / 2.0f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.2f, 0.1f);
        glVertex2f(x + 0.05f + rightDoorOffset, y);
        glVertex2f(x + GATE_WIDTH / 2.0f - 0.05f + rightDoorOffset, y);
        glVertex2f(x + GATE_WIDTH / 2.0f - 0.05f + rightDoorOffset, y + GATE_HEIGHT / 2.0f - 0.1f);
        glVertex2f(x + 0.05f + rightDoorOffset, y + GATE_HEIGHT / 2.0f - 0.1f);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glColor3f(1.0f, 0.84f, 0.0f);
        glVertex2f(x + GATE_WIDTH / 4.0f - 0.03f + rightDoorOffset, y + 0.1f);
        glVertex2f(x + GATE_WIDTH / 4.0f + 0.03f + rightDoorOffset, y + 0.1f);
        glVertex2f(x + GATE_WIDTH / 4.0f + 0.03f + rightDoorOffset, y + 0.18f);
        glVertex2f(x + GATE_WIDTH / 4.0f - 0.03f + rightDoorOffset, y + 0.18f);
        glEnd();
    }

    if (isOpen)
    {
        const int segments = 8;
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 1.0f, 0.8f, 0.6f);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++)
        {
            float angle = 2.0f * 3.14159f * i / segments;
            float cx = x + GATE_WIDTH * 0.3f * cos(angle);
            float cy = y + GATE_HEIGHT * 0.3f * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();
    }
}

Gate::Gate(float x, float y)
    : Platform(x, y, GATE_WIDTH, GATE_HEIGHT, defaultDrawFunc), isOpening(false), isOpen(false), openProgress(0.0f)
{
}

Gate::Gate(float x, float y, void (*drawFunc)(float, float))
    : Platform(x, y, GATE_WIDTH, GATE_HEIGHT, drawFunc), isOpening(false), isOpen(false), openProgress(0.0f)
{
}

Gate::~Gate()
{
}

void Gate::update(float deltaTime)
{
    if (isOpening && !isOpen)
    {
        openProgress += deltaTime / DOOR_OPEN_DURATION;
        if (openProgress >= 1.0f)
        {
            openProgress = 1.0f;
            isOpen = true;
        }
    }

    currentDrawingGate = this;
}
