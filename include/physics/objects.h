#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glut.h>
#include <vector>

enum CollisionShape
{
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_POINT
};

class _Object;
class CollisionBox;

class _Object
{
protected:
    float x, y;
    void (*drawFunction)(float, float);

public:
    _Object(float x, float y, void (*drawFunc)(float, float)) : x(x), y(y), drawFunction(drawFunc) {}
    virtual void draw() = 0;
    virtual void update(float deltaTime) = 0;
    virtual ~_Object() {}
    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY)
    {
        x = newX;
        y = newY;
    }
};

class CollisionBox
{
private:
    float x, y;
    float width, height;
    float radius;
    CollisionShape shape;
    _Object *owner;

public:
    CollisionBox(float x, float y, float w, float h, _Object *obj);
    CollisionBox(float x, float y, float r, _Object *obj);
    CollisionBox(float x, float y, _Object *obj);

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getRadius() const { return radius; }
    CollisionShape getShape() const { return shape; }
    _Object *getOwner() const { return owner; }
    void updatePosition(float newX, float newY);
};

class CollisionDetector
{
private:
    static CollisionDetector *instance;
    std::vector<CollisionBox *> collisionBoxes;

    CollisionDetector();
    CollisionDetector(const CollisionDetector &) = delete;
    CollisionDetector &operator=(const CollisionDetector &) = delete;

    bool checkCircleCircle(const CollisionBox *a, const CollisionBox *b);
    bool checkRectRect(const CollisionBox *a, const CollisionBox *b);
    bool checkCircleRect(const CollisionBox *a, const CollisionBox *b);
    bool checkPointCircle(const CollisionBox *point, const CollisionBox *circle);
    bool checkPointRect(const CollisionBox *point, const CollisionBox *rect);

public:
    ~CollisionDetector();

    static CollisionDetector *getInstance();
    static void destroy();

    CollisionBox *addCollisionBox(float x, float y, float width, float height, _Object *owner);
    CollisionBox *addCollisionCircle(float x, float y, float radius, _Object *owner);
    CollisionBox *addCollisionPoint(float x, float y, _Object *owner);

    void removeCollisionBox(CollisionBox *box);
    void removeCollisionBoxesByOwner(_Object *owner);

    bool checkCollision(const CollisionBox *a, const CollisionBox *b);
    std::vector<CollisionBox *> getCollisions(const CollisionBox *box);
    std::vector<_Object *> getCollidingObjects(const CollisionBox *box);

    void clear();
    int getCollisionBoxCount() const;
};

class _CollisionObject : public _Object
{
private:
    CollisionBox *collisionBox;

public:
    _CollisionObject(float x, float y, void (*drawFunc)(float, float));
    virtual ~_CollisionObject();

    CollisionBox *getCollisionBox() const { return collisionBox; }
    void setCollisionBox(float width, float height);
    void setCollisionCircle(float radius);
    bool isColliding();
    float getCollisionWidth() const { return collisionBox ? collisionBox->getWidth() : 0.0f; }
    float getCollisionHeight() const { return collisionBox ? collisionBox->getHeight() : 0.0f; }
    float getCollisionRadius() const { return collisionBox ? collisionBox->getRadius() : 0.0f; }
    CollisionBox *getCollisionBoxPtr() const { return collisionBox; }
    std::vector<_Object *> getCollidingObjects();

    bool isCollidingFromTop(_CollisionObject *other);
    bool isCollidingFromBottom(_CollisionObject *other);
    bool isCollidingFromLeft(_CollisionObject *other);
    bool isCollidingFromRight(_CollisionObject *other);
};

class PhysicsObject : public _CollisionObject
{
private:
    float velocityX, velocityY;
    float terminalVelocityY, terminalVelocityX;
    float accelerationX, accelerationY;
    bool isGrounded;
    bool shouldRemove;

public:
    PhysicsObject(float x, float y, float terminalVelocityX, float terminalVelocityY, void (*drawFunc)(float, float));
    void setVelocity(float vx, float vy)
    {
        velocityX = vx;
        velocityY = vy;
    }
    void setAcceleration(float ax, float ay)
    {
        accelerationX = ax;
        accelerationY = ay;
    }
    void moveLeft(float amount) { x -= amount; }
    void moveRight(float amount) { x += amount; }
    void moveUp(float amount) { y += amount; }
    void moveDown(float amount) { y -= amount; }
    float getVelocityX() const { return velocityX; }
    float getVelocityY() const { return velocityY; }
    float getAccelerationX() const { return accelerationX; }
    float getAccelerationY() const { return accelerationY; }
    float getTerminalVelocityX() const { return terminalVelocityX; }
    float getTerminalVelocityY() const { return terminalVelocityY; }
    bool getIsGrounded() const { return isGrounded; }
    void setGrounded(bool grounded) { isGrounded = grounded; }
    bool getShouldRemove() const { return shouldRemove; }
    void markForRemoval() { shouldRemove = true; }
    void update(float deltaTime);
    void draw();
};

class StaticObject : public _CollisionObject
{
public:
    StaticObject(float x, float y, void (*drawFunc)(float, float));
    void draw();
    void update(float deltaTime) {};
};

#endif