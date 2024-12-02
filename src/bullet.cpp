#include "Bullet.h"
#include "Utils.h"

Bullet::Bullet(float x, float y, const sf::Vector2f& velocity, int damage, const sf::Color& fillColor)
    : velocity(velocity), damage(damage) {
    shape.setRadius(5.0f);
    shape.setFillColor(fillColor);
    shape.setPosition(x, y);
}

void Bullet::update(float deltaTime) {
    shape.move(velocity * deltaTime);
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

int Bullet::getDamage() const {
    return damage;
}

AutoTargetBullet::AutoTargetBullet(float x, float y, Enemy* target, int damage, const sf::Color& fillColor)
    : Bullet(x, y, { 0, 0 }, damage, fillColor), target(target), targetLost(false) {
    if (target) {
        targetLost = false;
    }
    else {
        targetLost = true;
    }
}

Enemy* AutoTargetBullet::getTarget() const {
    if (this == nullptr) {
        return nullptr;
    }
    else
    {
        return target;
    }
}

void AutoTargetBullet::loseTarget() {
    targetLost = true;
}

bool AutoTargetBullet::isTargetLost() const {
    return targetLost;
}

void AutoTargetBullet::update(float deltaTime) {
    if (!targetLost) {
        sf::Vector2f dir = target->getPosition() - shape.getPosition();
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (length > 0.0f) {
            dir /= length;
            velocity = dir * 350.0f;
        }
        shape.move(velocity * deltaTime);
    }
    else {
        velocity = { 0, 0 };
    }
}

FastBullet::FastBullet(float x, float y, const sf::Vector2f& velocity, int damage, const sf::Color& fillColor)
    : Bullet(x, y, velocity, damage, fillColor) {}

JumpBullet::JumpBullet(float x, float y, Enemy* target, int damage,int jumpCount, const sf::Color& fillColor)
    : Bullet(x, y, { 0, 0 }, damage, fillColor), target(target), deleteFlat(false), jumpCount(jumpCount) {
    if (target) {
        deleteFlat = false;
    }
    else {
        deleteFlat = true;
    }
}

Enemy* JumpBullet::getTarget() const {
    if (this == nullptr) {
        return nullptr;
    }
    else
    {
        return target;
    }
}

void JumpBullet::canDelete() {
    deleteFlat = true;
}

bool JumpBullet::isDelete() const {
    return deleteFlat;
}

void JumpBullet::setPosition(const sf::Vector2f& newPosition) {
    shape.setPosition(newPosition); 
}

void JumpBullet::jump(std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (jumpCount > 0) {
        jumpCount -= 1;
        Enemy* nearestEnemy = nullptr;
        float minDistance = 200.0f;

        for (auto& enemy : enemies) {
            auto* nextTarget = dynamic_cast<Enemy*>(enemy.get());
            if (!nextTarget || nextTarget == target) {
                continue;
            }

            float dist = distance(getPosition(), nextTarget->getPosition());
            if (dist <= 200.0f && (!nearestEnemy || dist < minDistance)) {
                nearestEnemy = nextTarget;
                minDistance = dist;
            }
        }

        if (nearestEnemy) {
            sf::Vector2f targetPos = target->getPosition();
            sf::Vector2f nearestEnemyPos = nearestEnemy->getPosition();

            sf::Vector2f direction = nearestEnemyPos - targetPos;

            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0.0f) { 
                direction /= length;
            }

            setPosition(getPosition() + (direction * 15.0f));
            target = nearestEnemy;
        }
        else {
            deleteFlat = true;
        }
    }
    else {
        deleteFlat = true;
    }
}

void JumpBullet::update(float deltaTime) {
    if (!deleteFlat) {
        sf::Vector2f targetPos = target->getPosition();
        sf::Vector2f shapePos = shape.getPosition();

        sf::Vector2f dir = targetPos - shapePos;
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (length > 0.0f) {
            dir /= length;
            velocity = dir * 350.0f;
        }
        shape.move(velocity * deltaTime);
    }
}
