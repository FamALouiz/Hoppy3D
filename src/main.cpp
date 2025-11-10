#include <GL/glut.h>
#include "game3d.h"

Game3D *game = nullptr;

void display()
{
    if (game)
    {
        game->draw();
    }
}

void update(int value)
{
    if (game)
    {
        game->update(0.016f); // ~60 FPS
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    if (game)
    {
        game->handleKeyPress(key);
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    if (game)
    {
        game->handleKeyRelease(key);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    if (game)
    {
        game->handleSpecialKeyPress(key);
    }
    glutPostRedisplay();
}

void specialKeysUp(int key, int x, int y)
{
    if (game)
    {
        game->handleSpecialKeyRelease(key);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (game)
    {
        game->handleMouse(button, state, x, y);
    }
}

void mouseMotion(int x, int y)
{
    if (game)
    {
        game->handleMouseMotion(x, y);
    }
    glutPostRedisplay();
}

void passiveMouseMotion(int x, int y)
{
    if (game)
    {
        game->handleMouseMotion(x, y);
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ancient Warriors - 3D Collectibles Game");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Set up lighting
    GLfloat lightPos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Set background color
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Initialize game
    game = Game3D::getInstance();
    game->init();

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMouseMotion);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, update, 0);

    // Start main loop
    glutMainLoop();

    // Cleanup
    Game3D::destroy();

    return 0;
}
