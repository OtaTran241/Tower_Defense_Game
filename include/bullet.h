#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "IEntity.h"
#include "Enemy.h"

class Bullet : public IEntity {
protected:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    int damage;

public:
    Bullet(float x, float y, const sf::Vector2f& velocity, int damage, const sf::Color& fillColor);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    sf::Vector2f getPosition() const override;
    int getDamage() const;
};

class AutoTargetBullet : public Bullet {
private:
    Enemy* target;
    bool targetLost;

public:
    AutoTargetBullet(float x, float y, Enemy* target, int damage, const sf::Color& fillColor = sf::Color::Blue);
    Enemy* getTarget() const;
    void loseTarget();
    bool isTargetLost() const;
    void update(float deltaTime) override;
};

class FastBullet : public Bullet {
public:
    FastBullet(float x, float y, const sf::Vector2f& velocity, int damage, const sf::Color& fillColor = sf::Color::Red);
};

class JumpBullet : public Bullet {
private:
    Enemy* target;
    bool deleteFlat;
    int jumpCount;

    void setPosition(const sf::Vector2f& newPosition);

public:
    JumpBullet(float x, float y, Enemy* target, int damage,int jumpCount = 3, const sf::Color& fillColor = sf::Color(228, 205, 205));
    Enemy* getTarget() const;
    void canDelete();
    bool isDelete() const;
    void jump(std::vector<std::unique_ptr<Enemy>>& enemies);
    void update(float deltaTime) override;
};
#endif
