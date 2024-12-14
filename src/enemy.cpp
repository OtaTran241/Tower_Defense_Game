#include "Enemy.h"

Enemy::Enemy(float x, float y, float speed, sf::Color color, int health, int goldDrop, int currentNodeIndex, float radius)
    : speed(speed), health(health), color(color), goldDrop(goldDrop), currentNodeIndex(currentNodeIndex), reachedEnd(false), radius(radius) {
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setPosition(x, y);
    direction = { 0.0f, 0.0f };
}

void Enemy::update(float deltaTime) {
    if (isDamaged && damageTimer.getElapsedTime().asSeconds() >= 0.1f) {
        setColor(color);  
        isDamaged = false;     
    }

    if (reachedEnd || movePath.empty()) return; 

    sf::Vector2f target = movePath[currentNodeIndex].getPosition();
    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2f movement = direction * speed * deltaTime;

    if (std::sqrt(std::pow(target.x - currentPos.x, 2) + std::pow(target.y - currentPos.y, 2)) <= speed * deltaTime) {
        shape.setPosition(target);
        currentNodeIndex++;

        if (currentNodeIndex >= static_cast<int>(movePath.size())) {
            reachedEnd = true;
        }
        else {
            direction = movePath[currentNodeIndex].getPosition() - shape.getPosition();
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) direction /= length;
        }
    }
    else {
        shape.move(movement);
    }
}

void Enemy::setSize(float radius) {
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
}

float Enemy::getSize() const {
    return radius;
}

void Enemy::setColor(const sf::Color& color) {
    shape.setFillColor(color);
}

bool Enemy::hasReachedEnd() const {
    return reachedEnd;
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Enemy::getDirection() const {
    return direction;
}

void Enemy::takeDamage(int damage) {
    getDamageAnimation();
    health -= damage;
}

void Enemy::getDamageAnimation() {
    if (!isDamaged) {
        setColor(sf::Color(246, 98, 62));  
        isDamaged = true;   
        damageTimer.restart();   
    }
}


std::vector<MoveNode> Enemy::getPath() const {
    return movePath;
}

void Enemy::setPath(const std::vector<MoveNode>& path, int currentNodeIndex) {
    movePath = path;
    this->currentNodeIndex = currentNodeIndex;
    if (!movePath.empty()) {
        direction = movePath[currentNodeIndex].getPosition() - shape.getPosition(); 
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y); 
        if (length != 0) direction /= length; 
    }
}

int Enemy::getGoldDrop() const {
    return goldDrop;
}

bool Enemy::isDead() {
    return health <= 0;
}

FastEnemy::FastEnemy(float x, float y)
    : Enemy(x, y, 150.0f, sf::Color::Yellow, 100, 50) {}

SlowEnemy::SlowEnemy(float x, float y)
    : Enemy(x, y, 80.0f, sf::Color::Green, 150, 100) {}

CloneEnemy::CloneEnemy(float x, float y)
    : Enemy(x, y, 70.0f, sf::Color(139, 69, 19), 200, 200) {}

void CloneEnemy::update(float deltaTime) {
    Enemy::update(deltaTime);
}

std::vector<std::unique_ptr<Enemy>> CloneEnemy::clone(sf::Vector2f position) {
    std::vector<std::unique_ptr<Enemy>> clones;

    auto clone1 = std::make_unique<Enemy>(position.x + 20.0f, position.y, 90.0f, sf::Color(139, 60, 19), 50, 85);
    clone1->setSize(7.0f);
    clone1->setPath(getPath(), currentNodeIndex);
    auto clone2 = std::make_unique<Enemy>(position.x, position.y + 20.0f, 90.0f, sf::Color(139, 60, 19), 50, 85);
    clone2->setSize(7.0f);
    clone2->setPath(getPath(), currentNodeIndex);

    clones.push_back(std::move(clone1)); 
    clones.push_back(std::move(clone2)); 

    return clones;
}

std::vector<std::unique_ptr<Enemy>>& CloneEnemy::getSpawnedClones() {
    return spawnedClones;
}

bool CloneEnemy::isDead() {
    if (health <= healthThresholdClone) {
        goldDrop = 30;
        spawnedClones = clone(getPosition());
        return true;
    }
    else {
        return false;
    }
}