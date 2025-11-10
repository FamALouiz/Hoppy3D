#include "game3d.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player3D::Player3D(Vector3 pos) : position(pos), velocity(0, 0, 0), rotation(0), size(0.3f),
                                  velocityY(0), isJumping(false), moveSpeed(3.5f)
{
    color = Color3(0.2f, 0.1f, 0.05f);
}

void Player3D::draw()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation, 0, 1, 0);

    // Body (torso) - rectangular prism
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(0, size * 0.8f, 0);
    glScalef(size * 0.6f, size * 0.9f, size * 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Head - sphere
    glPushMatrix();
    glColor3f(0.9f, 0.8f, 0.7f); // Skin color
    glTranslatef(0, size * 1.7f, 0);
    glutSolidSphere(size * 0.35, 20, 20);
    glPopMatrix();

    // Headband - torus
    glPushMatrix();
    glColor3f(0.8f, 0.0f, 0.0f);
    glTranslatef(0, size * 1.8f, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.05, size * 0.35, 10, 20);
    glPopMatrix();

    // Left arm
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(-size * 0.4f, size * 1.0f, 0);
    glScalef(size * 0.2f, size * 0.7f, size * 0.2f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Right arm
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(size * 0.4f, size * 1.0f, 0);
    glScalef(size * 0.2f, size * 0.7f, size * 0.2f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Left leg
    glPushMatrix();
    glColor3f(0.15f, 0.15f, 0.15f);
    glTranslatef(-size * 0.2f, size * 0.25f, 0);
    glScalef(size * 0.25f, size * 0.6f, size * 0.25f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Right leg
    glPushMatrix();
    glColor3f(0.15f, 0.15f, 0.15f);
    glTranslatef(size * 0.2f, size * 0.25f, 0);
    glScalef(size * 0.25f, size * 0.6f, size * 0.25f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Sword on back
    glPushMatrix();
    glTranslatef(0, size * 1.2f, -size * 0.3f); // Position behind the back
    glRotatef(-30, 1, 0, 0);                    // Tilt for diagonal placement

    // Sword blade (cylinder)
    glPushMatrix();
    glColor3f(0.7f, 0.7f, 0.75f); // Silver blade
    glTranslatef(0, size * 0.5f, 0);
    glRotatef(-90, 1, 0, 0);
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, size * 0.05f, size * 0.05f, size * 1.0f, 10, 10);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // Sword tip (cone)
    glPushMatrix();
    glColor3f(0.7f, 0.7f, 0.75f);
    glTranslatef(0, size * 1.5f, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(size * 0.05f, size * 0.2f, 10, 10);
    glPopMatrix();

    // Sword handle (cylinder)
    glPushMatrix();
    glColor3f(0.2f, 0.1f, 0.05f); // Dark brown handle
    glTranslatef(0, size * 0.25f, 0);
    glRotatef(-90, 1, 0, 0);
    quad = gluNewQuadric();
    gluCylinder(quad, size * 0.08f, size * 0.08f, size * 0.25f, 10, 10);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // Sword guard (flattened cube)
    glPushMatrix();
    glColor3f(0.6f, 0.5f, 0.2f); // Gold guard
    glTranslatef(0, size * 0.5f, 0);
    glScalef(size * 0.4f, size * 0.05f, size * 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

void Player3D::setVelocity(float vx, float vz)
{
    velocity.x = vx;
    velocity.z = vz;

    if (vx != 0 || vz != 0)
    {
        rotation = atan2(vx, vz) * 180.0f / M_PI;
    }
}

void Player3D::update(float deltaTime, const std::vector<Platform3D *> &platforms)
{
    Vector3 newPos = position;
    newPos.x += velocity.x * deltaTime;
    newPos.z += velocity.z * deltaTime;

    if (newPos.x < -4.7f)
        newPos.x = -4.7f;
    if (newPos.x > 4.7f)
        newPos.x = 4.7f;
    if (newPos.z < -4.7f)
        newPos.z = -4.7f;
    if (newPos.z > 4.7f)
        newPos.z = 4.7f;

    bool horizontalCollision = false;
    for (auto platform : platforms)
    {
        Vector3 pPos = platform->getPosition();
        Vector3 pSize = platform->getSize();

        float minX = pPos.x - pSize.x / 2 - size;
        float maxX = pPos.x + pSize.x / 2 + size;
        float minZ = pPos.z - pSize.z / 2 - size;
        float maxZ = pPos.z + pSize.z / 2 + size;

        float platTop = pPos.y + pSize.y / 2 + 0.045f;
        float platBottom = pPos.y - pSize.y / 2;

        if (newPos.x > minX && newPos.x < maxX &&
            newPos.z > minZ && newPos.z < maxZ)
        {
            if (position.y < platTop - 0.1f && position.y + size > platBottom)
            {
                horizontalCollision = true;
                break;
            }
        }
    }

    if (!horizontalCollision)
    {
        position.x = newPos.x;
        position.z = newPos.z;
    }

    const float GRAVITY = -15.0f;
    velocityY += GRAVITY * deltaTime;
    position.y += velocityY * deltaTime;

    // Ground
    if (position.y <= 0)
    {
        position.y = 0;
        velocityY = 0;
        isJumping = false;
    }

    for (auto platform : platforms)
    {
        Vector3 pPos = platform->getPosition();
        Vector3 pSize = platform->getSize();

        float minX = pPos.x - pSize.x / 2;
        float maxX = pPos.x + pSize.x / 2;
        float minZ = pPos.z - pSize.z / 2;
        float maxZ = pPos.z + pSize.z / 2;

        float platTop = pPos.y + pSize.y / 2 + 0.045f;
        float platBottom = pPos.y - pSize.y / 2;

        if (position.x > minX && position.x < maxX &&
            position.z > minZ && position.z < maxZ)
        {
            if (velocityY <= 0 && position.y >= platTop - 0.2f && position.y <= platTop + 0.5f)
            {
                position.y = platTop;
                velocityY = 0;
                isJumping = false;
            }
            else if (velocityY > 0 && position.y + size * 2 >= platBottom && position.y < platBottom)
            {
                position.y = platBottom - size * 2 - 0.01f;
                velocityY = 0;
            }
        }
    }
}

void Player3D::jump()
{
    if (!isJumping)
    {
        velocityY = 7.0f;
        isJumping = true;
    }
}

bool Player3D::checkCollision(Vector3 pos, float radius)
{
    float dist = (position - pos).length();
    return dist < (size + radius);
}

Collectible3D::Collectible3D(Vector3 pos, Color3 col, int platId)
    : position(pos), collected(false), rotationAngle(0), bobOffset(0),
      color(col), platformId(platId)
{
}

void Collectible3D::draw()
{
    if (collected)
        return;

    glPushMatrix();
    glTranslatef(position.x, position.y + bobOffset, position.z);
    glRotatef(rotationAngle, 0, 1, 0);

    glPushMatrix();
    glColor3f(color.r, color.g, color.b);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.05, 0.15, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(color.r * 1.2f, color.g * 1.2f, color.b * 1.2f);
    glutSolidSphere(0.12, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(color.r * 0.8f, color.g * 0.8f, color.b * 0.8f);
    glRotatef(45, 0, 1, 0);
    glutSolidCube(0.1);
    glPopMatrix();

    glPopMatrix();
}

void Collectible3D::update(float deltaTime)
{
    if (collected)
        return;

    rotationAngle += 90.0f * deltaTime;
    if (rotationAngle > 360)
        rotationAngle -= 360;

    bobOffset = sin(rotationAngle * M_PI / 180.0f) * 0.1f;
}

bool Collectible3D::checkCollision(Vector3 playerPos, float playerRadius)
{
    if (collected)
        return false;

    float dist = (position - playerPos).length();
    return dist < (0.2f + playerRadius);
}

Platform3D::Platform3D(Vector3 pos, Vector3 sz, Color3 col, int platformId)
    : position(pos), size(sz), color(col), id(platformId)
{
}

void Platform3D::draw()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    glPushMatrix();
    glColor3f(color.r, color.g, color.b);
    glScalef(size.x, size.y, size.z);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(color.r * 0.7f, color.g * 0.7f, color.b * 0.7f);
    glTranslatef(0, size.y / 2 + 0.02f, 0);
    glScalef(size.x + 0.1f, 0.05f, size.z + 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

bool Platform3D::isPlayerOn(Vector3 playerPos, float playerRadius)
{
    float minX = position.x - size.x / 2;
    float maxX = position.x + size.x / 2;
    float minZ = position.z - size.z / 2;
    float maxZ = position.z + size.z / 2;

    return playerPos.x > minX && playerPos.x < maxX &&
           playerPos.z > minZ && playerPos.z < maxZ;
}

AnimatedObject3D::AnimatedObject3D(Vector3 pos, AnimationType type, int platId, int objType)
    : basePosition(pos), currentPosition(pos), animType(type),
      animationActive(false), animationEnabled(false), animationTime(0),
      scale(1.0f), platformId(platId), objectType(objType)
{

    baseColor = Color3(0.8f, 0.6f, 0.2f);
    currentColor = baseColor;
}

void AnimatedObject3D::draw()
{
    glPushMatrix();
    glTranslatef(currentPosition.x, currentPosition.y, currentPosition.z);
    glScalef(scale, scale, scale);

    if (animType == ANIM_COLOR_CHANGE)
    {
        glColor3f(currentColor.r, currentColor.g, currentColor.b);
    }

    switch (objectType)
    {
    case 0:
        drawSamuraiHelmet();
        break;
    case 1:
        drawNinjaStar();
        break;
    case 2:
        drawPagoda();
        break;
    case 3:
        drawDragon();
        break;
    }

    glPopMatrix();
}

void AnimatedObject3D::drawSamuraiHelmet()
{
    // Helmet dome (sphere)
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.35f);
    glTranslatef(0, 0.3f, 0);
    glScalef(1.0f, 0.8f, 1.0f);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    // Face guard (cube)
    glPushMatrix();
    glColor3f(0.25f, 0.25f, 0.3f);
    glTranslatef(0, 0.1f, 0.35f);
    glScalef(0.6f, 0.5f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Horns (cones)
    glPushMatrix();
    glColor3f(0.7f, 0.6f, 0.2f);
    glTranslatef(-0.3f, 0.6f, 0);
    glRotatef(-30, 0, 0, 1);
    glutSolidCone(0.08, 0.3, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.7f, 0.6f, 0.2f);
    glTranslatef(0.3f, 0.6f, 0);
    glRotatef(30, 0, 0, 1);
    glutSolidCone(0.08, 0.3, 10, 10);
    glPopMatrix();

    // Crest (torus)
    glPushMatrix();
    glColor3f(0.8f, 0.1f, 0.1f);
    glTranslatef(0, 0.55f, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.05, 0.15, 10, 20);
    glPopMatrix();
}

void AnimatedObject3D::drawNinjaStar()
{
    // Center sphere
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.55f);
    glutSolidSphere(0.15, 20, 20);
    glPopMatrix();

    // 8 blades (cubes)
    for (int i = 0; i < 8; i++)
    {
        float angle = i * 45.0f;
        glPushMatrix();
        glRotatef(angle, 0, 1, 0);
        glTranslatef(0, 0, 0.25f);
        glScalef(0.15f, 0.05f, 0.3f);
        glColor3f(0.6f, 0.6f, 0.65f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}

void AnimatedObject3D::drawPagoda()
{
    // Base platform
    glPushMatrix();
    glColor3f(0.4f, 0.2f, 0.1f);
    glTranslatef(0, 0, 0);
    glScalef(0.8f, 0.1f, 0.8f);
    glutSolidCube(1.0);
    glPopMatrix();

    // First tier
    glPushMatrix();
    glColor3f(0.5f, 0.25f, 0.15f);
    glTranslatef(0, 0.15f, 0);
    glScalef(0.6f, 0.15f, 0.6f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Roof 1 (cone)
    glPushMatrix();
    glColor3f(0.7f, 0.2f, 0.2f);
    glTranslatef(0, 0.25f, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.45, 0.2, 20, 10);
    glPopMatrix();

    // Second tier
    glPushMatrix();
    glColor3f(0.5f, 0.25f, 0.15f);
    glTranslatef(0, 0.45f, 0);
    glScalef(0.4f, 0.1f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Roof 2 (cone)
    glPushMatrix();
    glColor3f(0.7f, 0.2f, 0.2f);
    glTranslatef(0, 0.52f, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.3, 0.15, 20, 10);
    glPopMatrix();

    // Top sphere
    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.2f);
    glTranslatef(0, 0.7f, 0);
    glutSolidSphere(0.08, 20, 20);
    glPopMatrix();
}

void AnimatedObject3D::drawDragon()
{
    // Get current color for this object (will change if color animation is active)
    float r = (animType == ANIM_COLOR_CHANGE) ? currentColor.r : 0.8f;
    float g = (animType == ANIM_COLOR_CHANGE) ? currentColor.g : 0.2f;
    float b = (animType == ANIM_COLOR_CHANGE) ? currentColor.b : 0.2f;

    // Body (sphere)
    glPushMatrix();
    glColor3f(r, g, b);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Head (sphere)
    glPushMatrix();
    glColor3f(r * 1.06f, g * 1.25f, b * 1.25f);
    glTranslatef(0, 0.15f, 0.35f);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    // Snout (cone)
    glPushMatrix();
    glColor3f(r * 1.13f, g * 1.5f, b * 1.5f);
    glTranslatef(0, 0.15f, 0.5f);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(0.1, 0.15, 10, 10);
    glPopMatrix();

    // Wings (flattened cubes)
    glPushMatrix();
    glColor3f(r * 0.88f, g * 0.75f, b * 0.75f);
    glTranslatef(-0.35f, 0.1f, 0);
    glRotatef(30, 0, 0, 1);
    glScalef(0.4f, 0.05f, 0.3f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3f(r * 0.88f, g * 0.75f, b * 0.75f);
    glTranslatef(0.35f, 0.1f, 0);
    glRotatef(-30, 0, 0, 1);
    glScalef(0.4f, 0.05f, 0.3f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Tail (cone)
    glPushMatrix();
    glColor3f(r * 0.94f, g, b);
    glTranslatef(0, 0, -0.3f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.1, 0.4, 10, 10);
    glPopMatrix();
}

void AnimatedObject3D::update(float deltaTime)
{
    if (!animationActive)
        return;

    animationTime += deltaTime;

    switch (animType)
    {
    case ANIM_ROTATION:
        // Continuous rotation around Y axis
        currentPosition = basePosition;
        break;

    case ANIM_SCALING:
        // Pulsing scale
        scale = 1.0f + 0.3f * sin(animationTime * 2.0f);
        break;

    case ANIM_TRANSFORMATION:
        // Up and down movement
        currentPosition = basePosition;
        currentPosition.y += 0.3f * sin(animationTime * 2.0f);
        break;

    case ANIM_COLOR_CHANGE:
        // Color cycling
        currentColor.r = baseColor.r + 0.2f * sin(animationTime);
        currentColor.g = baseColor.g + 0.2f * sin(animationTime + 2.0f);
        currentColor.b = baseColor.b + 0.2f * sin(animationTime + 4.0f);
        break;
    }
}

Camera3D::Camera3D() : mode(CAM_THIRD_PERSON), angleH(0), angleV(-20)
{
    position = Vector3(0, 5, 10);
    lookAt = Vector3(0, 0, 0);
    up = Vector3(0, 1, 0);
}

void Camera3D::apply()
{
    if (mode == CAM_FREE)
    {
        float radH = angleH * M_PI / 180.0f;
        float radV = angleV * M_PI / 180.0f;

        lookAt.x = position.x + cos(radV) * sin(radH);
        lookAt.y = position.y + sin(radV);
        lookAt.z = position.z + cos(radV) * cos(radH);

        up = Vector3(0, 1, 0);
    }
    else if (mode == CAM_THIRD_PERSON)
    {
        up = Vector3(0, 1, 0);
    }
    else if (mode == CAM_TOP)
    {
        position = Vector3(0, 15, 0.1f);
        lookAt = Vector3(0, 0, 0);
        up = Vector3(0, 0, -1);
    }
    else if (mode == CAM_SIDE)
    {
        position = Vector3(12, 3, 0);
        lookAt = Vector3(0, 1, 0);
        up = Vector3(0, 1, 0);
    }
    else if (mode == CAM_FRONT)
    {
        position = Vector3(0, 3, -12);
        lookAt = Vector3(0, 1, 0);
        up = Vector3(0, 1, 0);
    }

    gluLookAt(position.x, position.y, position.z,
              lookAt.x, lookAt.y, lookAt.z,
              up.x, up.y, up.z);
}

void Camera3D::setMode(CameraMode newMode)
{
    mode = newMode;
    if (mode == CAM_FREE)
    {
        position = Vector3(0, 5, 10);
        angleH = 0;
        angleV = -20;
        up = Vector3(0, 1, 0);
    }
    else if (mode == CAM_THIRD_PERSON)
    {
        angleH = 0;
        angleV = -20;
    }
}

void Camera3D::moveForward(float amount)
{
    if (mode != CAM_FREE)
        return;

    float radH = angleH * M_PI / 180.0f;
    float radV = angleV * M_PI / 180.0f;

    position.x += amount * cos(radV) * sin(radH);
    position.y += amount * sin(radV);
    position.z += amount * cos(radV) * cos(radH);
}

void Camera3D::moveRight(float amount)
{
    if (mode != CAM_FREE)
        return;

    float radH = (angleH + 90) * M_PI / 180.0f;

    position.x += amount * sin(radH);
    position.z += amount * cos(radH);
}

void Camera3D::moveUp(float amount)
{
    if (mode != CAM_FREE)
        return;
    position.y += amount;
}

void Camera3D::rotate(float dh, float dv)
{
    if (mode != CAM_FREE)
        return;

    angleH += dh;
    angleV += dv;

    if (angleV > 89)
        angleV = 89;
    if (angleV < -89)
        angleV = -89;
}

void Camera3D::followPlayer(Vector3 playerPos)
{
    if (mode == CAM_THIRD_PERSON)
    {
        // Position camera behind and above the player
        float distance = 5.0f;
        float height = 3.0f;

        position.x = playerPos.x;
        position.y = playerPos.y + height;
        position.z = playerPos.z + distance;

        lookAt.x = playerPos.x;
        lookAt.y = playerPos.y + 1.0f;
        lookAt.z = playerPos.z;
    }
}

Game3D *Game3D::instance = nullptr;

Game3D::Game3D() : player(nullptr), camera(nullptr), gameState(STATE_PLAYING),
                   gameTime(0), maxGameTime(120.0f)
{
    for (int i = 0; i < 4; i++)
    {
        collectedPerPlatform[i] = 0;
        totalPerPlatform[i] = 0;
    }

    for (int i = 0; i < 256; i++)
    {
        keyStates[i] = false;
        specialKeyStates[i] = false;
    }
}

Game3D *Game3D::getInstance()
{
    if (!instance)
    {
        instance = new Game3D();
    }
    return instance;
}

void Game3D::destroy()
{
    if (instance)
    {
        delete instance->player;
        for (auto p : instance->platforms)
            delete p;
        for (auto c : instance->collectibles)
            delete c;
        for (auto o : instance->animatedObjects)
            delete o;
        delete instance->camera;
        delete instance;
        instance = nullptr;
    }
}

void Game3D::init()
{
    srand(time(NULL));

    player = new Player3D(Vector3(0, 0, 0));
    camera = new Camera3D();

    createPlatforms();
    createCollectibles();
    createAnimatedObjects();

    gameState = STATE_PLAYING;
    gameTime = 0;
}

void Game3D::createPlatforms()
{
    // 4 distinct platforms in different positions
    platforms.push_back(new Platform3D(Vector3(-2.5f, 0.2f, -2.5f), Vector3(1.5f, 0.4f, 1.5f),
                                       Color3(0.6f, 0.3f, 0.2f), 0));
    platforms.push_back(new Platform3D(Vector3(2.5f, 0.3f, -2.5f), Vector3(1.8f, 0.6f, 1.8f),
                                       Color3(0.3f, 0.5f, 0.6f), 1));
    platforms.push_back(new Platform3D(Vector3(-2.5f, 0.25f, 2.5f), Vector3(1.6f, 0.5f, 1.6f),
                                       Color3(0.5f, 0.6f, 0.3f), 2));
    platforms.push_back(new Platform3D(Vector3(2.5f, 0.35f, 2.5f), Vector3(2.0f, 0.7f, 2.0f),
                                       Color3(0.7f, 0.4f, 0.5f), 3));
}

void Game3D::createCollectibles()
{
    // At least 3 collectibles per platform (12 total minimum)
    Color3 colors[4] = {
        Color3(1.0f, 0.84f, 0.0f),   // Gold
        Color3(0.75f, 0.75f, 0.75f), // Silver
        Color3(0.8f, 0.5f, 0.2f),    // Bronze
        Color3(0.9f, 0.1f, 0.1f)     // Ruby
    };

    for (int p = 0; p < 4; p++)
    {
        Vector3 platPos = platforms[p]->getPosition();
        Vector3 platSize = platforms[p]->getSize();

        // Create 4 collectibles per platform
        for (int i = 0; i < 4; i++)
        {
            float offsetX = (i % 2 == 0 ? -0.3f : 0.3f);
            float offsetZ = (i < 2 ? -0.3f : 0.3f);

            Vector3 pos(platPos.x + offsetX,
                        platPos.y + platSize.y / 2 + 0.3f,
                        platPos.z + offsetZ);

            collectibles.push_back(new Collectible3D(pos, colors[p], p));
            totalPerPlatform[p]++;
        }
    }
}

void Game3D::createAnimatedObjects()
{
    // Create 4 animated objects, one per platform with different animation
    for (int i = 0; i < 4; i++)
    {
        Vector3 platPos = platforms[i]->getPosition();
        Vector3 platSize = platforms[i]->getSize();
        Vector3 objPos(platPos.x, platPos.y + platSize.y / 2 + 0.6f, platPos.z);

        AnimationType animType = static_cast<AnimationType>(i);
        animatedObjects.push_back(new AnimatedObject3D(objPos, animType, i, i));
    }
}

void Game3D::update(float deltaTime)
{
    if (gameState == STATE_PLAYING || gameState == STATE_WON)
    {
        // Calculate velocity based on key states
        float vx = 0.0f;
        float vz = 0.0f;
        float moveSpeed = 3.5f;

        // WASD keys
        if (keyStates['w'] || keyStates['W'])
            vz -= moveSpeed;
        if (keyStates['s'] || keyStates['S'])
            vz += moveSpeed;
        if (keyStates['a'] || keyStates['A'])
            vx -= moveSpeed;
        if (keyStates['d'] || keyStates['D'])
            vx += moveSpeed;

        // Arrow keys
        if (specialKeyStates[GLUT_KEY_UP])
            vz -= moveSpeed;
        if (specialKeyStates[GLUT_KEY_DOWN])
            vz += moveSpeed;
        if (specialKeyStates[GLUT_KEY_LEFT])
            vx -= moveSpeed;
        if (specialKeyStates[GLUT_KEY_RIGHT])
            vx += moveSpeed;

        // Set player velocity
        player->setVelocity(vx, vz);

        // Update player physics
        player->update(deltaTime, platforms);

        // Update camera to follow player if in third person mode
        camera->followPlayer(player->getPosition());
    }

    if (gameState == STATE_PLAYING || gameState == STATE_WON)
    {
        gameTime += deltaTime;

        // Check time limit
        if (gameTime >= maxGameTime)
        {
            gameState = STATE_GAMEOVER;
            return;
        }

        // Update animated objects
        for (auto obj : animatedObjects)
        {
            obj->update(deltaTime);
        }
    }

    if (gameState == STATE_PLAYING)
    {
        // Update collectibles
        for (auto c : collectibles)
        {
            c->update(deltaTime);
        }

        // Check collections
        checkCollections();
    }
}

void Game3D::checkCollections()
{
    Vector3 playerPos = player->getPosition();
    float playerRadius = player->getSize();

    for (auto c : collectibles)
    {
        if (!c->isCollected() && c->checkCollision(playerPos, playerRadius))
        {
            c->collect();
            collectedPerPlatform[c->getPlatformId()]++;

            // Check if all collectibles on this platform are collected
            int platId = c->getPlatformId();
            if (collectedPerPlatform[platId] == totalPerPlatform[platId])
            {
                animatedObjects[platId]->enableAnimation();
                animatedObjects[platId]->toggleAnimation(); // Start animation

                // Check if all platforms are complete
                bool allComplete = true;
                for (int i = 0; i < 4; i++)
                {
                    if (collectedPerPlatform[i] < totalPerPlatform[i])
                    {
                        allComplete = false;
                        break;
                    }
                }

                if (allComplete)
                {
                    gameState = STATE_WON;
                }
            }
        }
    }
}

void Game3D::drawGround()
{
    glPushMatrix();
    glColor3f(0.2f, 0.25f, 0.15f); // Dark green
    glTranslatef(0, -0.01f, 0);
    glScalef(10.0f, 0.02f, 10.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Grid pattern for visual appeal
    glPushMatrix();
    glColor3f(0.15f, 0.2f, 0.1f);
    glTranslatef(0, 0, 0);
    for (int i = -5; i <= 5; i++)
    {
        glPushMatrix();
        glTranslatef(i, 0, 0);
        glScalef(0.02f, 0.01f, 10.0f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0, i);
        glScalef(10.0f, 0.01f, 0.02f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void Game3D::drawWalls()
{
    float wallHeight = 2.0f;
    float wallThickness = 0.2f;

    CameraMode camMode = camera->getMode();

    if (camMode != CAM_FRONT)
    {
        glPushMatrix();
        glColor3f(0.4f, 0.3f, 0.25f);
        glTranslatef(0, wallHeight / 2, -5);
        glScalef(10.0f, wallHeight, wallThickness);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Left wall
    glPushMatrix();
    glColor3f(0.4f, 0.3f, 0.25f);
    glTranslatef(-5, wallHeight / 2, 0);
    glScalef(wallThickness, wallHeight, 10.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    if (camMode != CAM_SIDE)
    {
        glPushMatrix();
        glColor3f(0.4f, 0.3f, 0.25f);
        glTranslatef(5, wallHeight / 2, 0);
        glScalef(wallThickness, wallHeight, 10.0f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    // Front wall
    glPushMatrix();
    glColor3f(0.4f, 0.3f, 0.25f);
    glTranslatef(0, wallHeight / 2, 5);
    glScalef(10.0f, wallHeight, wallThickness);
    glutSolidCube(1.0);
    glPopMatrix();
}

void Game3D::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (gameState == STATE_GAMEOVER)
    {
        drawGameOver();
        return;
    }

    // Apply camera
    camera->apply();

    // Draw environment
    drawGround();
    drawWalls();

    // Draw platforms
    for (auto p : platforms)
    {
        p->draw();
    }

    // Draw collectibles
    for (auto c : collectibles)
    {
        c->draw();
    }

    // Draw animated objects with rotation if needed
    for (auto obj : animatedObjects)
    {
        if (obj->isActive() && obj->getPlatformId() == 0)
        { // Rotation animation
            glPushMatrix();
            Vector3 pos = animatedObjects[0]->getPlatformId(); // Get base position
            auto platform = platforms[0];
            Vector3 platPos = platform->getPosition();
            Vector3 platSize = platform->getSize();

            glTranslatef(platPos.x, platPos.y + platSize.y / 2 + 0.6f, platPos.z);
            glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.05f, 0, 1, 0);
            glTranslatef(-platPos.x, -(platPos.y + platSize.y / 2 + 0.6f), -platPos.z);
            obj->draw();
            glPopMatrix();
        }
        else
        {
            obj->draw();
        }
    }

    // Draw player
    player->draw();

    // Draw HUD
    drawHUD();

    // Draw win message if won
    if (gameState == STATE_WON)
    {
        drawWinScreen();
    }

    glutSwapBuffers();
}

void Game3D::drawHUD()
{
    // Switch to 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // Draw timer
    glColor3f(1.0f, 1.0f, 1.0f);
    std::stringstream ss;
    ss << "Time: " << std::fixed << std::setprecision(1) << (maxGameTime - gameTime) << "s";
    drawText(10, 580, ss.str().c_str());

    // Draw collectibles count
    int totalCollected = 0;
    int totalCount = 0;
    for (int i = 0; i < 4; i++)
    {
        totalCollected += collectedPerPlatform[i];
        totalCount += totalPerPlatform[i];
    }

    ss.str("");
    ss << "Collectibles: " << totalCollected << "/" << totalCount;
    drawText(10, 560, ss.str().c_str());

    // Draw platform status
    const char *platNames[] = {"Red Platform", "Blue Platform", "Green Platform", "Purple Platform"};
    for (int i = 0; i < 4; i++)
    {
        ss.str("");
        ss << platNames[i] << ": " << collectedPerPlatform[i] << "/" << totalPerPlatform[i];
        if (animatedObjects[i]->isEnabled())
        {
            ss << " [COMPLETE]";
        }
        drawText(10, 530 - i * 20, ss.str().c_str());
    }

    // Draw controls help
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(10, 120, "Controls:", GLUT_BITMAP_HELVETICA_12);
    drawText(10, 100, "WASD/Arrow Keys: Move | SPACE: Jump", GLUT_BITMAP_HELVETICA_12);
    drawText(10, 80, "1-4: Toggle Animations (after collecting all items on platform)", GLUT_BITMAP_HELVETICA_12);

    glColor3f(0.7f, 0.9f, 1.0f);
    drawText(10, 55, "Camera: P=3rd Person | T=Top | V=Side | F=Front | C=Free Camera", GLUT_BITMAP_HELVETICA_12);
    drawText(10, 35, "Free Camera: I/K=Forward/Back | J/L=Left/Right | U/O=Up/Down", GLUT_BITMAP_HELVETICA_12);
    drawText(10, 15, "Mouse: Look around (in Free Camera mode only)", GLUT_BITMAP_HELVETICA_12);

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Game3D::drawText(float x, float y, const char *text, void *font)
{
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void Game3D::drawWinScreen()
{
    // Draw semi-transparent overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    // Draw "YOU WIN!" message
    glColor3f(1.0f, 0.84f, 0.0f);
    drawText(300, 350, "*** YOU WIN! ***", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(250, 300, "All collectibles gathered!", GLUT_BITMAP_HELVETICA_18);
    drawText(280, 270, "You can keep playing!", GLUT_BITMAP_HELVETICA_18);

    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Game3D::drawGameOver()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Simple game over scene
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    // Background
    glColor3f(0.1f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    // Text
    glColor3f(1.0f, 0.0f, 0.0f);
    drawText(320, 350, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(300, 300, "Time's up!", GLUT_BITMAP_HELVETICA_18);

    std::stringstream ss;
    int totalCollected = 0;
    for (int i = 0; i < 4; i++)
    {
        totalCollected += collectedPerPlatform[i];
    }
    ss << "Collectibles gathered: " << totalCollected << "/16";
    drawText(280, 270, ss.str().c_str(), GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(300, 200, "Press ESC to exit", GLUT_BITMAP_HELVETICA_12);

    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void Game3D::handleKeyPress(unsigned char key)
{
    if (key < 256)
        keyStates[key] = true;

    float camSpeed = 0.3f;

    if (gameState != STATE_PLAYING && gameState != STATE_WON)
        return;

    switch (key)
    {
    case 'w':
    case 'W':
    case 's':
    case 'S':
    case 'a':
    case 'A':
    case 'd':
    case 'D':
        // Handled in update()
        break;

    // Jump
    case ' ':
        player->jump();
        break;

    // Camera movement (free camera)
    case 'i':
    case 'I':
        camera->moveForward(camSpeed);
        break;
    case 'k':
    case 'K':
        camera->moveForward(-camSpeed);
        break;
    case 'j':
    case 'J':
        camera->moveRight(camSpeed);
        break;
    case 'l':
    case 'L':
        camera->moveRight(-camSpeed);
        break;
    case 'u':
    case 'U':
        camera->moveUp(camSpeed);
        break;
    case 'o':
    case 'O':
        camera->moveUp(-camSpeed);
        break;

    // Camera views
    case 't':
    case 'T':
        camera->setMode(CAM_TOP);
        break;
    case 'f':
    case 'F':
        camera->setMode(CAM_FRONT);
        break;
    case 'c':
    case 'C':
        camera->setMode(CAM_FREE);
        break;
    case 'v':
    case 'V': // Side view
        camera->setMode(CAM_SIDE);
        break;
    case 'p':
    case 'P': // Third person view
        camera->setMode(CAM_THIRD_PERSON);
        break;

    // Toggle animations (only if enabled)
    case '1':
        if (animatedObjects[0]->isEnabled())
            animatedObjects[0]->toggleAnimation();
        break;
    case '2':
        if (animatedObjects[1]->isEnabled())
            animatedObjects[1]->toggleAnimation();
        break;
    case '3':
        if (animatedObjects[2]->isEnabled())
            animatedObjects[2]->toggleAnimation();
        break;
    case '4':
        if (animatedObjects[3]->isEnabled())
            animatedObjects[3]->toggleAnimation();
        break;

    case 27: // ESC
        exit(0);
        break;
    }
}

void Game3D::handleKeyRelease(unsigned char key)
{
    // Mark key as released
    if (key < 256)
        keyStates[key] = false;
}

void Game3D::handleSpecialKeyPress(int key)
{
    // Mark special key as pressed
    if (key < 256)
        specialKeyStates[key] = true;
}

void Game3D::handleSpecialKeyRelease(int key)
{
    // Mark special key as released
    if (key < 256)
        specialKeyStates[key] = false;
}

void Game3D::handleMouse(int button, int state, int x, int y)
{
    // Can be used for additional controls if needed
}

void Game3D::handleMouseMotion(int x, int y)
{
    // Only rotate camera in free mode
    if (camera->getMode() != CAM_FREE)
        return;

    static int lastX = 400;
    static int lastY = 300;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
        return;
    }

    float dx = (x - lastX) * 0.3f;
    float dy = (y - lastY) * 0.3f;

    camera->rotate(-dx, -dy);

    lastX = x;
    lastY = y;
}
