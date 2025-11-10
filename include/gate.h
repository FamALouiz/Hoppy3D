#ifndef GATE_H
#define GATE_H

#include "platform.h"
#include <GL/glut.h>

#define GATE_WIDTH 0.5f
#define GATE_HEIGHT 0.8f
#define GATE_COLOR_R 0.8f
#define GATE_COLOR_G 0.6f
#define GATE_COLOR_B 0.2f
#define DOOR_OPEN_DURATION 1.0f

class Gate : public Platform
{
private:
    static Gate *currentDrawingGate;
    bool isOpening;
    bool isOpen;
    float openProgress;

    static void defaultDrawFunc(float x, float y);

public:
    Gate(float x, float y);
    Gate(float x, float y, void (*drawFunc)(float, float));
    ~Gate();

    void startOpening() { isOpening = true; }
    bool getIsOpening() const { return isOpening; }
    bool getIsOpen() const { return isOpen; }
    float getOpenProgress() const { return openProgress; }
    void update(float deltaTime);
};

#endif
