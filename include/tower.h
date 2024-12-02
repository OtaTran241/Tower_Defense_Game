#pragma once
#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "IEntity.h"
#include "Enemy.h"
#include <queue>

class Tower : public IEntity {
protected:
    sf::RectangleShape shape;
    float range;
    sf::CircleShape rangeCircle;
    float shootCooldown;
    float shootTimer;
    unsigned int goldPrice;
    unsigned int damage;
    unsigned int upgradeCount;
    bool isRangeVisible;
    bool isMegaUpgrade;

    Enemy* findNearestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies);

public:
    Tower(float x, float y, float range, sf::Color color, int goldPrice, int damage, float shootCooldown = 1.0f, int upgradeCount = 5);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void showRange();
    sf::Vector2f getPosition() const override;
    virtual void tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) = 0;
    void upgradeMega();
    bool hasUpgradeMega() const;

    int getGoldPrice() const;

    int getUpgradeCount() const;
    void setUpgradeCount(int newDamage);

    float getShootCooldown() const;
    void setShootCooldown(float cooldown);

    float getRange() const;
    void setRange(float newRange);

    const sf::CircleShape& getRangeCircle() const;
    void updateRangeCircle();

    int getDamage() const;
    void setDamage(int newDamage);

    sf::FloatRect getGlobalBounds() const;
};

class TrackingTower : public Tower {
public:
    TrackingTower(float x, float y);
    void tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) override;
};

class TripleTower : public Tower {
public:
    TripleTower(float x, float y);
    void tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) override;
};

class BulletJumpTower : public Tower {
public:
    BulletJumpTower(float x, float y);
    void tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) override;
};
#endif

