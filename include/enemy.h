#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "IEntity.h"
#include "move_node.h"

class Enemy : public IEntity {
protected:
    sf::Color color;
    sf::CircleShape shape;
    float speed;
    float radius;
    sf::Vector2f direction;
    int health;
    unsigned int goldDrop;

    std::vector<MoveNode> movePath;
    int currentNodeIndex;         
    bool reachedEnd;

    sf::Clock damageTimer; 
    bool isDamaged = false;


public:
    Enemy(float x, float y, float speed, sf::Color color, int health, int goldDrop, int currentNodeIndex = 0, float radius = 10.0f);

    virtual bool isDead();
    int getGoldDrop() const;
    bool hasReachedEnd() const;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    sf::Vector2f getPosition() const override;
    sf::Vector2f getDirection() const;
    void takeDamage(int damage);
    void setColor(const sf::Color& color);
    void getDamageAnimation();
    std::vector<MoveNode> getPath() const;
    void setPath(const std::vector<MoveNode>& movePath, int currentNodeIndex = 0);
    void setSize(float radius);
    float getSize() const;
};

class FastEnemy : public Enemy {
public:
    FastEnemy(float x, float y);
};

class SlowEnemy : public Enemy {
public:
    SlowEnemy(float x, float y);
};

class CloneEnemy : public Enemy {
private:
    int healthThresholdClone = 50;
    std::vector<std::unique_ptr<Enemy>> spawnedClones;

    void update(float deltaTime) override;

public:
    CloneEnemy(float x, float y);
    bool isDead() override;
    std::vector<std::unique_ptr<Enemy>> clone(sf::Vector2f position);
    std::vector<std::unique_ptr<Enemy>>& getSpawnedClones();
};
#endif
