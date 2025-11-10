#include "physics/objects.h"
#include "physics/core.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iostream>

CollisionDetector *CollisionDetector::instance = nullptr;

CollisionBox::CollisionBox(float x, float y, float w, float h, _Object *obj)
    : x(x), y(y), width(w), height(h), radius(0.0f), shape(SHAPE_RECTANGLE), owner(obj)
{
}

CollisionBox::CollisionBox(float x, float y, float r, _Object *obj)
    : x(x), y(y), width(0.0f), height(0.0f), radius(r), shape(SHAPE_CIRCLE), owner(obj)
{
}

CollisionBox::CollisionBox(float x, float y, _Object *obj)
    : x(x), y(y), width(0.0f), height(0.0f), radius(0.0f), shape(SHAPE_POINT), owner(obj)
{
}

void CollisionBox::updatePosition(float newX, float newY)
{
    x = newX;
    y = newY;
}

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
    for (CollisionBox *box : collisionBoxes)
    {
        delete box;
    }
    collisionBoxes.clear();
}

CollisionDetector *CollisionDetector::getInstance()
{
    if (instance == nullptr)
    {
        instance = new CollisionDetector();
    }
    return instance;
}

void CollisionDetector::destroy()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

CollisionBox *CollisionDetector::addCollisionBox(float x, float y, float width, float height, _Object *owner)
{
    CollisionBox *box = new CollisionBox(x, y, width, height, owner);
    collisionBoxes.push_back(box);
    return box;
}

CollisionBox *CollisionDetector::addCollisionCircle(float x, float y, float radius, _Object *owner)
{
    CollisionBox *circle = new CollisionBox(x, y, radius, owner);
    collisionBoxes.push_back(circle);
    return circle;
}

CollisionBox *CollisionDetector::addCollisionPoint(float x, float y, _Object *owner)
{
    CollisionBox *point = new CollisionBox(x, y, owner);
    collisionBoxes.push_back(point);
    return point;
}

void CollisionDetector::removeCollisionBox(CollisionBox *box)
{
    auto it = std::find(collisionBoxes.begin(), collisionBoxes.end(), box);
    if (it != collisionBoxes.end())
    {
        delete *it;
        collisionBoxes.erase(it);
    }
}

void CollisionDetector::removeCollisionBoxesByOwner(_Object *owner)
{
    for (auto it = collisionBoxes.begin(); it != collisionBoxes.end();)
    {
        if ((*it)->getOwner() == owner)
        {
            delete *it;
            it = collisionBoxes.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool CollisionDetector::checkCollision(const CollisionBox *a, const CollisionBox *b)
{
    if (a->getShape() == SHAPE_CIRCLE && b->getShape() == SHAPE_CIRCLE)
    {
        return checkCircleCircle(a, b);
    }
    else if (a->getShape() == SHAPE_RECTANGLE && b->getShape() == SHAPE_RECTANGLE)
    {
        return checkRectRect(a, b);
    }
    else if ((a->getShape() == SHAPE_CIRCLE && b->getShape() == SHAPE_RECTANGLE) ||
             (a->getShape() == SHAPE_RECTANGLE && b->getShape() == SHAPE_CIRCLE))
    {
        return checkCircleRect(a, b);
    }
    else if (a->getShape() == SHAPE_POINT && b->getShape() == SHAPE_CIRCLE)
    {
        return checkPointCircle(a, b);
    }
    else if (a->getShape() == SHAPE_CIRCLE && b->getShape() == SHAPE_POINT)
    {
        return checkPointCircle(b, a);
    }
    else if (a->getShape() == SHAPE_POINT && b->getShape() == SHAPE_RECTANGLE)
    {
        return checkPointRect(a, b);
    }
    else if (a->getShape() == SHAPE_RECTANGLE && b->getShape() == SHAPE_POINT)
    {
        return checkPointRect(b, a);
    }

    return false;
}

std::vector<CollisionBox *> CollisionDetector::getCollisions(const CollisionBox *box)
{
    std::vector<CollisionBox *> collisions;
    for (CollisionBox *other : collisionBoxes)
    {
        if (other != box && checkCollision(box, other))
        {
            collisions.push_back(other);
        }
    }

    return collisions;
}

std::vector<_Object *> CollisionDetector::getCollidingObjects(const CollisionBox *box)
{
    std::vector<_Object *> objects;
    std::vector<CollisionBox *> collisions = getCollisions(box);

    for (CollisionBox *collision : collisions)
    {
        if (collision->getOwner() != nullptr)
        {
            objects.push_back(collision->getOwner());
        }
    }

    return objects;
}

void CollisionDetector::clear()
{
    for (CollisionBox *box : collisionBoxes)
    {
        delete box;
    }
    collisionBoxes.clear();
}

int CollisionDetector::getCollisionBoxCount() const
{
    return collisionBoxes.size();
}

bool CollisionDetector::checkCircleCircle(const CollisionBox *a, const CollisionBox *b)
{
    float dx = a->getX() - b->getX();
    float dy = a->getY() - b->getY();
    float distance = std::sqrt(dx * dx + dy * dy);
    float radiusSum = a->getRadius() + b->getRadius();

    return distance < radiusSum;
}

bool CollisionDetector::checkRectRect(const CollisionBox *a, const CollisionBox *b)
{
    float aLeft = a->getX() - a->getWidth() / 2.0f;
    float aRight = a->getX() + a->getWidth() / 2.0f;
    float aTop = a->getY() + a->getHeight() / 2.0f;
    float aBottom = a->getY() - a->getHeight() / 2.0f;

    float bLeft = b->getX() - b->getWidth() / 2.0f;
    float bRight = b->getX() + b->getWidth() / 2.0f;
    float bTop = b->getY() + b->getHeight() / 2.0f;
    float bBottom = b->getY() - b->getHeight() / 2.0f;

    return !(aRight < bLeft || aLeft > bRight || aTop < bBottom || aBottom > bTop);
}

bool CollisionDetector::checkCircleRect(const CollisionBox *a, const CollisionBox *b)
{
    const CollisionBox *circle = (a->getShape() == SHAPE_CIRCLE) ? a : b;
    const CollisionBox *rect = (a->getShape() == SHAPE_RECTANGLE) ? a : b;

    float rectLeft = rect->getX() - rect->getWidth() / 2.0f;
    float rectRight = rect->getX() + rect->getWidth() / 2.0f;
    float rectTop = rect->getY() + rect->getHeight() / 2.0f;
    float rectBottom = rect->getY() - rect->getHeight() / 2.0f;

    float closestX = std::max(rectLeft, std::min(circle->getX(), rectRight));
    float closestY = std::max(rectBottom, std::min(circle->getY(), rectTop));

    float dx = circle->getX() - closestX;
    float dy = circle->getY() - closestY;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < circle->getRadius();
}

bool CollisionDetector::checkPointCircle(const CollisionBox *point, const CollisionBox *circle)
{
    float dx = point->getX() - circle->getX();
    float dy = point->getY() - circle->getY();
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < circle->getRadius();
}

bool CollisionDetector::checkPointRect(const CollisionBox *point, const CollisionBox *rect)
{
    float rectLeft = rect->getX() - rect->getWidth() / 2.0f;
    float rectRight = rect->getX() + rect->getWidth() / 2.0f;
    float rectTop = rect->getY() + rect->getHeight() / 2.0f;
    float rectBottom = rect->getY() - rect->getHeight() / 2.0f;

    return point->getX() >= rectLeft && point->getX() <= rectRight &&
           point->getY() >= rectBottom && point->getY() <= rectTop;
}

_CollisionObject::_CollisionObject(float x, float y, void (*drawFunc)(float, float))
    : _Object(x, y, drawFunc), collisionBox(nullptr)
{
}

_CollisionObject::~_CollisionObject()
{
    if (collisionBox != nullptr)
    {
        CollisionDetector *detector = CollisionDetector::getInstance();
        detector->removeCollisionBox(collisionBox);
        collisionBox = nullptr;
    }
}

void _CollisionObject::setCollisionBox(float width, float height)
{
    if (collisionBox != nullptr)
    {
        CollisionDetector *detector = CollisionDetector::getInstance();
        detector->removeCollisionBox(collisionBox);
    }

    CollisionDetector *detector = CollisionDetector::getInstance();
    collisionBox = detector->addCollisionBox(x, y, width, height, this);
}

void _CollisionObject::setCollisionCircle(float radius)
{
    if (collisionBox != nullptr)
    {
        CollisionDetector *detector = CollisionDetector::getInstance();
        detector->removeCollisionBox(collisionBox);
    }

    CollisionDetector *detector = CollisionDetector::getInstance();
    collisionBox = detector->addCollisionCircle(x, y, radius, this);
}

bool _CollisionObject::isColliding()
{
    if (collisionBox == nullptr)
    {
        return false;
    }

    CollisionDetector *detector = CollisionDetector::getInstance();
    std::vector<CollisionBox *> collisions = detector->getCollisions(collisionBox);
    return !collisions.empty();
}

std::vector<_Object *> _CollisionObject::getCollidingObjects()
{
    if (collisionBox == nullptr)
    {
        return std::vector<_Object *>();
    }

    CollisionDetector *detector = CollisionDetector::getInstance();
    return detector->getCollidingObjects(collisionBox);
}

bool _CollisionObject::isCollidingFromTop(_CollisionObject *other)
{
    if (!collisionBox || !other->getCollisionBox())
        return false;

    CollisionDetector *detector = CollisionDetector::getInstance();
    if (!detector->checkCollision(collisionBox, other->getCollisionBox()))
        return false;

    float myBottom = y - getCollisionHeight() / 2.0f;
    float myTop = y + getCollisionHeight() / 2.0f;
    float myLeft = x - getCollisionWidth() / 2.0f;
    float myRight = x + getCollisionWidth() / 2.0f;

    float otherBottom = other->getY() - other->getCollisionHeight() / 2.0f;
    float otherTop = other->getY() + other->getCollisionHeight() / 2.0f;
    float otherLeft = other->getX() - other->getCollisionWidth() / 2.0f;
    float otherRight = other->getX() + other->getCollisionWidth() / 2.0f;

    float overlapTop = myTop - otherBottom;    // How much we overlap from top
    float overlapBottom = otherTop - myBottom; // How much we overlap from bottom
    float overlapLeft = myRight - otherLeft;   // How much we overlap from left
    float overlapRight = otherRight - myLeft;  // How much we overlap from right

    float minOverlap = std::min({overlapTop, overlapBottom, overlapLeft, overlapRight});

    return minOverlap == overlapBottom && overlapBottom > 0;
}

bool _CollisionObject::isCollidingFromBottom(_CollisionObject *other)
{
    if (!collisionBox || !other->getCollisionBox())
        return false;

    CollisionDetector *detector = CollisionDetector::getInstance();
    if (!detector->checkCollision(collisionBox, other->getCollisionBox()))
        return false;

    float myBottom = y - getCollisionHeight() / 2.0f;
    float myTop = y + getCollisionHeight() / 2.0f;
    float myLeft = x - getCollisionWidth() / 2.0f;
    float myRight = x + getCollisionWidth() / 2.0f;

    float otherBottom = other->getY() - other->getCollisionHeight() / 2.0f;
    float otherTop = other->getY() + other->getCollisionHeight() / 2.0f;
    float otherLeft = other->getX() - other->getCollisionWidth() / 2.0f;
    float otherRight = other->getX() + other->getCollisionWidth() / 2.0f;

    float overlapTop = myTop - otherBottom;    // How much we overlap from top
    float overlapBottom = otherTop - myBottom; // How much we overlap from bottom
    float overlapLeft = myRight - otherLeft;   // How much we overlap from left
    float overlapRight = otherRight - myLeft;  // How much we overlap from right

    float minOverlap = std::min({overlapTop, overlapBottom, overlapLeft, overlapRight});

    return minOverlap == overlapTop && overlapTop > 0;
}

bool _CollisionObject::isCollidingFromRight(_CollisionObject *other)
{
    if (!collisionBox || !other->getCollisionBox())
        return false;

    CollisionDetector *detector = CollisionDetector::getInstance();
    if (!detector->checkCollision(collisionBox, other->getCollisionBox()))
        return false;

    float myBottom = y - getCollisionHeight() / 2.0f;
    float myTop = y + getCollisionHeight() / 2.0f;
    float myLeft = x - getCollisionWidth() / 2.0f;
    float myRight = x + getCollisionWidth() / 2.0f;

    float otherBottom = other->getY() - other->getCollisionHeight() / 2.0f;
    float otherTop = other->getY() + other->getCollisionHeight() / 2.0f;
    float otherLeft = other->getX() - other->getCollisionWidth() / 2.0f;
    float otherRight = other->getX() + other->getCollisionWidth() / 2.0f;

    float overlapTop = myTop - otherBottom;    // How much we overlap from top
    float overlapBottom = otherTop - myBottom; // How much we overlap from bottom
    float overlapLeft = myRight - otherLeft;   // How much we overlap from left
    float overlapRight = otherRight - myLeft;  // How much we overlap from right

    float minOverlap = std::min({overlapTop, overlapBottom, overlapLeft, overlapRight});

    return minOverlap == overlapRight && overlapRight > 0;
}

bool _CollisionObject::isCollidingFromLeft(_CollisionObject *other)
{
    if (!collisionBox || !other->getCollisionBox())
        return false;

    CollisionDetector *detector = CollisionDetector::getInstance();
    if (!detector->checkCollision(collisionBox, other->getCollisionBox()))
        return false;

    float myBottom = y - getCollisionHeight() / 2.0f;
    float myTop = y + getCollisionHeight() / 2.0f;
    float myLeft = x - getCollisionWidth() / 2.0f;
    float myRight = x + getCollisionWidth() / 2.0f;

    float otherBottom = other->getY() - other->getCollisionHeight() / 2.0f;
    float otherTop = other->getY() + other->getCollisionHeight() / 2.0f;
    float otherLeft = other->getX() - other->getCollisionWidth() / 2.0f;
    float otherRight = other->getX() + other->getCollisionWidth() / 2.0f;

    float overlapTop = myTop - otherBottom;    // How much we overlap from top
    float overlapBottom = otherTop - myBottom; // How much we overlap from bottom
    float overlapLeft = myRight - otherLeft;   // How much we overlap from left
    float overlapRight = otherRight - myLeft;  // How much we overlap from right

    float minOverlap = std::min({overlapTop, overlapBottom, overlapLeft, overlapRight});

    return minOverlap == overlapLeft && overlapLeft > 0;
}

PhysicsObject::PhysicsObject(float x, float y, float terminalVelocityX, float terminalVelocityY, void (*drawFunc)(float, float))
    : _CollisionObject(x, y, drawFunc), velocityX(0.0f), velocityY(0.0f), terminalVelocityY(terminalVelocityY), terminalVelocityX(terminalVelocityX), accelerationX(0.0f), accelerationY(0.0f), isGrounded(false), shouldRemove(false)
{
}

void PhysicsObject::update(float deltaTime)
{
    velocityX += accelerationX * deltaTime;
    velocityY += accelerationY * deltaTime;

    if (velocityX > terminalVelocityX)
        velocityX = terminalVelocityX;
    if (velocityX < -terminalVelocityX)
        velocityX = -terminalVelocityX;
    if (velocityY > terminalVelocityY)
        velocityY = terminalVelocityY;
    if (velocityY < -terminalVelocityY)
        velocityY = -terminalVelocityY;

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
    CollisionBox *collisionBox = getCollisionBoxPtr();
    if (collisionBox != nullptr)
    {
        collisionBox->updatePosition(x, y);
    }
}

void PhysicsObject::draw()
{
    if (drawFunction)
    {
        drawFunction(x, y);
    }
}

StaticObject::StaticObject(float x, float y, void (*drawFunc)(float, float))
    : _CollisionObject(x, y, drawFunc)
{
}

void StaticObject::draw()
{
    if (drawFunction)
    {
        drawFunction(x, y);
    }
}