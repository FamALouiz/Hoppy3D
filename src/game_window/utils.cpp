#include "game_window/utils.h"
#include "game_window/game_window.h"
#include <GL/glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw a simple triangle as a test
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex2f(-0.5f, -0.5f);

    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex2f(0.5f, -0.5f);

    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    GameWindow *window = GameWindow::getInstance();

    switch (key)
    {
    case 'f':
    case 'F':
        window->toggleFullscreen();
        break;

    default:
        printf("Key pressed: %c (ASCII: %d) at (%d, %d)\n", key, key, x, y);
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_F11:
        printf("F11 pressed - Toggling fullscreen...\n");
        GameWindow::getInstance()->toggleFullscreen();
        break;

    case GLUT_KEY_UP:
        printf("Up arrow pressed\n");
        break;

    case GLUT_KEY_DOWN:
        printf("Down arrow pressed\n");
        break;

    case GLUT_KEY_LEFT:
        printf("Left arrow pressed\n");
        break;

    case GLUT_KEY_RIGHT:
        printf("Right arrow pressed\n");
        break;
    }
}