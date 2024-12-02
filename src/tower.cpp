#include "Tower.h"
#include "Utils.h"

Tower::Tower(float x, float y, float range, sf::Color color, int goldPrice, int damage, float shootCooldown,int upgradeCount)
    : range(range), shootCooldown(shootCooldown), shootTimer(0.0f), goldPrice(goldPrice), damage(damage), upgradeCount(upgradeCount), isRangeVisible(false), isMegaUpgrade(false) {
    shape.setSize({ 30.0f, 30.0f });
    shape.setFillColor(color);
    shape.setPosition(x - 15.0f, y - 15.0f);

    rangeCircle.setRadius(range);
    rangeCircle.setFillColor(sf::Color(color.r, color.g, color.b, 20));
    rangeCircle.setOutlineColor(color);
    rangeCircle.setOutlineThickness(1.0f);
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(x, y);
}

void Tower::update(float deltaTime) {
    shootTimer += deltaTime;
}

void Tower::showRange() {
    isRangeVisible = !isRangeVisible;
}

void Tower::render(sf::RenderWindow& window) {
    if (isRangeVisible) {
        window.draw(rangeCircle);
    }
    window.draw(shape);
}

sf::Vector2f Tower::getPosition() const {
    return shape.getPosition();
}

sf::FloatRect Tower::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

int Tower::getUpgradeCount() const {
    return upgradeCount;
}

void Tower::setUpgradeCount(int newUpgradeCount) {
    upgradeCount = newUpgradeCount;
}

int Tower::getGoldPrice() const {
    return goldPrice;
}

float Tower::getShootCooldown() const {
    return shootCooldown;
}

void Tower::setShootCooldown(float cooldown) {
    shootCooldown = cooldown;
}

float Tower::getRange() const {
    return range;
}

void Tower::setRange(float newRange) {
    range = newRange;
    updateRangeCircle();
}

int Tower::getDamage() const {
    return damage;
}

void Tower::setDamage(int newDamage) {
    damage = newDamage;
}

const sf::CircleShape& Tower::getRangeCircle() const {
    return rangeCircle;
}

void Tower::updateRangeCircle() {
    rangeCircle.setRadius(range);
    rangeCircle.setOrigin(range, range);
}

void Tower::upgradeMega() {
    isMegaUpgrade = true;
}

bool Tower::hasUpgradeMega() const {
    return isMegaUpgrade;
}

Enemy* Tower::findNearestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* nearestEnemy = nullptr;
    float minDistance = range;

    for (auto& enemy : enemies) {
        auto target = dynamic_cast<Enemy*>(enemy.get());
        if (!target) continue;

        float dist = distance(getPosition(), target->getPosition());
        if (dist <= range && (!nearestEnemy || dist < minDistance)) {
            nearestEnemy = target;
            minDistance = dist;
        }
    }
    return nearestEnemy;
}

TrackingTower::TrackingTower(float x, float y)
    : Tower(x, y, 200.0f, sf::Color::Blue, 100, 25) {
    damage = 25;
}

void TrackingTower::tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) {
    if (shootTimer < shootCooldown)
        return;
    if (isMegaUpgrade) {
        shootTimer = 0.0f;

        std::vector<Enemy*> nearestEnemies;
        std::vector<std::pair<float, Enemy*>> enemiesInRange;

        for (auto& enemy : enemies) {
            auto target = dynamic_cast<Enemy*>(enemy.get());
            if (!target) continue;

            float dist = distance(getPosition(), target->getPosition());
            if (dist <= range) {
                enemiesInRange.emplace_back(dist, target);
            }
        }

        std::sort(enemiesInRange.begin(), enemiesInRange.end(),
            [](const std::pair<float, Enemy*>& a, const std::pair<float, Enemy*>& b) {
                return a.first < b.first;
            });

        for (size_t i = 0; i < std::min<size_t>(3, enemiesInRange.size()); ++i) {
            nearestEnemies.push_back(enemiesInRange[i].second);
        }

        for (Enemy* enemy : nearestEnemies) {
            bullets.push_back(std::move(std::make_unique<AutoTargetBullet>(shape.getPosition().x, shape.getPosition().y, enemy, damage)));
        }
    }
    else {
        Enemy* nearestEnemy = findNearestEnemy(enemies);

        if (nearestEnemy) {
            shootTimer = 0.0f;

            bullets.push_back(std::move(std::make_unique<AutoTargetBullet>(shape.getPosition().x, shape.getPosition().y, nearestEnemy, damage)));
        }
    }
}

TripleTower::TripleTower(float x, float y)
    : Tower(x, y, 150.0f, sf::Color::Red, 130, 50) {
    damage = 20;
}

void TripleTower::tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) {
    if (shootTimer < shootCooldown)
        return;
    if (isMegaUpgrade) {
        Enemy* nearestEnemy = findNearestEnemy(enemies);

        if (nearestEnemy) {
            shootTimer = 0.0f;
            sf::Vector2f basePosition = shape.getPosition();
            sf::Vector2f dir = nearestEnemy->getPosition() - basePosition;
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= length;

            float angleStep = 10.0f * (3.14159f / 180.0f); 
            std::vector<sf::Vector2f> directions;

            for (int i = -2; i <= 2; ++i) {
                float angle = i * angleStep;
                sf::Vector2f rotatedDir(
                    dir.x * std::cos(angle) - dir.y * std::sin(angle),
                    dir.x * std::sin(angle) + dir.y * std::cos(angle)
                );
                directions.push_back(rotatedDir);
            }

            for (const auto& direction : directions) {
                bullets.push_back(std::move(std::make_unique<FastBullet>(basePosition.x, basePosition.y, direction * 600.0f, damage)));
            }
        }
    }
    else {
        Enemy* nearestEnemy = findNearestEnemy(enemies);

        if (nearestEnemy) {
            shootTimer = 0.0f;
            sf::Vector2f basePosition = shape.getPosition() + sf::Vector2f(0, 0);
            sf::Vector2f dir = nearestEnemy->getPosition() - basePosition;
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= length;

            float angle = 20.0f * (3.14159f / 180.0f);
            sf::Vector2f leftDir(
                dir.x * std::cos(angle) - dir.y * std::sin(angle),
                dir.x * std::sin(angle) + dir.y * std::cos(angle)
            );
            sf::Vector2f rightDir(
                dir.x * std::cos(-angle) - dir.y * std::sin(-angle),
                dir.x * std::sin(-angle) + dir.y * std::cos(-angle)
            );

            bullets.push_back(std::move(std::make_unique<FastBullet>(basePosition.x, basePosition.y, dir * 600.0f, damage)));
            bullets.push_back(std::move(std::make_unique<FastBullet>(basePosition.x, basePosition.y, leftDir * 600.0f, damage)));
            bullets.push_back(std::move(std::make_unique<FastBullet>(basePosition.x, basePosition.y, rightDir * 600.0f, damage)));
        }
    }
}

BulletJumpTower::BulletJumpTower(float x, float y)
    : Tower(x, y, 180.0f, sf::Color(228, 205, 205), 150, 25, 0.8f) {
    damage = 10;
}

void BulletJumpTower::tryShoot(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Bullet>>& bullets) {
    if (shootTimer < shootCooldown)
        return;

    Enemy* nearestEnemy = findNearestEnemy(enemies);

    if (nearestEnemy) {
        shootTimer = 0.0f;
        int jump = 2;

        if (isMegaUpgrade) {
            jump = 4;
        }

        bullets.push_back(std::move(std::make_unique<JumpBullet>(shape.getPosition().x, shape.getPosition().y, nearestEnemy, damage, jump)));
    }
}