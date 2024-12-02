#pragma once
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Utils.h"      
#include "IEntity.h"
#include "bullet.h"
#include "Tower.h"
#include "Enemy.h"
#include "UI.h"
#include "upgrade.h"
#include "move_node.h"
#include "move_path.h"

class GameEngine {
private:
    unsigned int windowH = 1200;
    unsigned int windowW = 1500;
    int goldCount;
    unsigned int slotCount;
    unsigned int spawnedEnemyCount = 0;
    unsigned int nextEnemyThreshold = 10;

    sf::Font font;
    std::vector<sf::Text> helpTexts;
    sf::RenderWindow window;
    sf::RectangleShape windowBorder;
    sf::RectangleShape helpPanel;
    sf::View view;
    sf::Sprite backgroundSprite;

    std::map<sf::Keyboard::Key, bool> keyStates;

    UI restartButton;
    UI pauseButton;
    UI goldCountLabel;
    UI slotCountLabel;
    UI uiPanel;
    UI buildUpgradePanel;
    UI buildLabel;
    UI upgradeLabel;


    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<MovePath>> movePaths;

    Upgrade* upgradeRange;
    Upgrade* upgradeDamage;
    Upgrade* upgradeAttackSpeed;

    float enemySpawnTimer = 0.0f;
    float enemySpawnInterval = 2.0f;

    void resetGame();
    void pauseGame();
    bool isPaused;

    friend void initializeGame(GameEngine& engine);
    friend void processCollisionsAndLogic(GameEngine& game, float deltaTime);
    friend void renderGame(GameEngine& engine);
    friend void setHelpText(GameEngine& engine);

    void addTowerInPosition(Tower* tower, const sf::Vector2f& position);
    void initializeKeyStates();

public:
    GameEngine();

    void addBullet(Bullet* bullet);
    void addTower(Tower* tower);
    void addEnemy(Enemy* enemy);
    void addGold(int gold);
    bool reduceGold(int gold);
    void applyUpgrade(Upgrade* upgrade, Tower* tower);

    int getGoldCount() const;

    void run();

};
#endif 

