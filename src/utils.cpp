#include <GL/glut.h>

void drawLetter(char c, float x, float y)
{
    glRasterPos2f(x, y);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}
void drawText(const char *text, float x, float y, float scale)
{
    glPushMatrix();

    int len = 0;
    while (text[len] != '\0')
    {
        len++;
    }

    float totalWidth = len * scale * 0.6f;
    float startX = x - totalWidth * 0.5f;

    for (int i = 0; i < len; i++)
    {
        float charX = startX + i * scale * 0.6f;

        drawLetter(text[i], charX, y);
    }

    glPopMatrix();
}
