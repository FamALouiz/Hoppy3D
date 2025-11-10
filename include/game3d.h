#ifndef GAME3D_H
#define GAME3D_H

#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <string>

class Player3D;
class Platform3D;
class Collectible3D;
class AnimatedObject3D;
class Camera3D;

struct Vector3
{
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3 &v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3 &v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    float length() const { return sqrt(x * x + y * y + z * z); }
    Vector3 normalize() const
    {
        float len = length();
        return len > 0 ? Vector3(x / len, y / len, z / len) : Vector3(0, 0, 0);
    }
};

struct Color3
{
    float r, g, b;
    Color3(float r = 1, float g = 1, float b = 1) : r(r), g(g), b(b) {}
};

enum AnimationType
{
    ANIM_ROTATION,
    ANIM_SCALING,
    ANIM_TRANSFORMATION,
    ANIM_COLOR_CHANGE
};

enum GameState
{
    STATE_PLAYING,
    STATE_WON,
    STATE_GAMEOVER
};

enum CameraMode
{
    CAM_FREE,
    CAM_TOP,
    CAM_SIDE,
    CAM_FRONT,
    CAM_THIRD_PERSON
};

class Player3D
{
private:
    Vector3 position;
    Vector3 velocity;
    float rotation;
    float size;
    Color3 color;
    float velocityY;
    bool isJumping;
    float moveSpeed;

public:
    Player3D(Vector3 pos);
    void draw();
    void setVelocity(float vx, float vz);
    void update(float deltaTime, const std::vector<Platform3D *> &platforms);
    void jump();
    void setRotation(float angle) { rotation = angle; }
    Vector3 getPosition() const { return position; }
    float getSize() const { return size; }
    bool checkCollision(Vector3 pos, float radius);
};

class Collectible3D
{
private:
    Vector3 position;
    bool collected;
    float rotationAngle;
    float bobOffset;
    Color3 color;
    int platformId;

public:
    Collectible3D(Vector3 pos, Color3 col, int platId);
    void draw();
    void update(float deltaTime);
    bool checkCollision(Vector3 playerPos, float playerRadius);
    void collect() { collected = true; }
    bool isCollected() const { return collected; }
    int getPlatformId() const { return platformId; }
};

class Platform3D
{
private:
    Vector3 position;
    Vector3 size;
    Color3 color;
    int id;

public:
    Platform3D(Vector3 pos, Vector3 sz, Color3 col, int platformId);
    void draw();
    Vector3 getPosition() const { return position; }
    Vector3 getSize() const { return size; }
    int getId() const { return id; }
    bool isPlayerOn(Vector3 playerPos, float playerRadius);
};

class AnimatedObject3D
{
private:
    Vector3 basePosition;
    Vector3 currentPosition;
    AnimationType animType;
    bool animationActive;
    bool animationEnabled;
    float animationTime;
    float scale;
    Color3 baseColor;
    Color3 currentColor;
    int platformId;
    int objectType;

public:
    AnimatedObject3D(Vector3 pos, AnimationType type, int platId, int objType);
    void draw();
    void update(float deltaTime);
    void enableAnimation() { animationEnabled = true; }
    void toggleAnimation()
    {
        if (animationEnabled)
            animationActive = !animationActive;
    }
    bool isEnabled() const { return animationEnabled; }
    bool isActive() const { return animationActive; }
    int getPlatformId() const { return platformId; }

private:
    void drawSamuraiHelmet();
    void drawNinjaStar();
    void drawPagoda();
    void drawDragon();
};

class Camera3D
{
private:
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    CameraMode mode;
    float angleH;
    float angleV;

public:
    Camera3D();
    void apply();
    void setMode(CameraMode newMode);
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
    void rotate(float dh, float dv);
    void followPlayer(Vector3 playerPos);
    CameraMode getMode() const { return mode; }
};

class Game3D
{
private:
    static Game3D *instance;

    Player3D *player;
    std::vector<Platform3D *> platforms;
    std::vector<Collectible3D *> collectibles;
    std::vector<AnimatedObject3D *> animatedObjects;
    Camera3D *camera;

    GameState gameState;
    float gameTime;
    float maxGameTime;

    int collectedPerPlatform[4];
    int totalPerPlatform[4];

    bool keyStates[256];
    bool specialKeyStates[256];

    Game3D();

public:
    static Game3D *getInstance();
    static void destroy();

    void init();
    void update(float deltaTime);
    void draw();
    void drawHUD();
    void drawGameOver();
    void drawWinScreen();

    void handleKeyPress(unsigned char key);
    void handleKeyRelease(unsigned char key);
    void handleSpecialKeyPress(int key);
    void handleSpecialKeyRelease(int key);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

    Player3D *getPlayer() { return player; }
    Camera3D *getCamera() { return camera; }
    GameState getState() const { return gameState; }

private:
    void createEnvironment();
    void createPlatforms();
    void createCollectibles();
    void createAnimatedObjects();
    void checkCollections();
    void drawGround();
    void drawWalls();
    void drawText(float x, float y, const char *text, void *font = GLUT_BITMAP_HELVETICA_18);
};

#endif
