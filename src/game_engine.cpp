#include "Game_Engine.h"

GameEngine::GameEngine() : window(sf::VideoMode(windowW / 1.5f, windowH / 1.5f), "Tower Defense Game") {
    initializeGame(*this);

    std::cout << std::string(15, '-') << "\n";
    std::cout << "Game start!" << "\n";
    std::cout << std::string(15, '-') << "\n";
}

void GameEngine::addBullet(Bullet* bullet) {
    bullets.emplace_back(bullet);
}

void GameEngine::addTower(Tower* tower) {
    towers.emplace_back(tower);
}

void GameEngine::addEnemy(Enemy* enemy) {
    enemies.emplace_back(enemy);
}

void GameEngine::addGold(int gold) {
    if (goldCount + gold > 9999) {
        goldCount = 9999;
    }
    else {
        goldCount += gold;
    }
}

bool GameEngine::reduceGold(int gold) {
    if (goldCount >= gold) {
        goldCount -= gold;
        return true;
    }
    else
    {
        return false;
    }
}

int GameEngine::getGoldCount() const {
    return goldCount;
}

void GameEngine::applyUpgrade(Upgrade* upgrade, Tower* tower) {
    if (reduceGold(upgrade->getCost())) {
        if (upgrade->apply(tower)) {
            std::cout << "Upgrade " << upgrade->getTypeName() << " successfully" << "\n";
        }
        else {
            std::cout << "Maxed Upgrade"  << "\n";
        }
    }
    else {
        std::cout << "Not enough gold to upgrade " << upgrade->getTypeName() << "\n";
    }
}

void GameEngine::pauseGame() {
    isPaused = !isPaused;
    if (isPaused) {
        pauseButton.setText("Continue");
        std::cout << "Game paused!" << "\n";
    }
    else {
        pauseButton.setText("Help/Pause");
        std::cout << "Game continued!" << "\n";
    }
}

void GameEngine::resetGame() {
    initializeGame(*this);

    std::cout << std::string(15, '-') << "\n";
    std::cout << "Game reset!" << "\n";
    std::cout << std::string(15, '-') << "\n";
}

void GameEngine::addTowerInPosition(Tower* tower, const sf::Vector2f& position) {
    if (isClickOnPaths(position, movePaths) || getTowerAtPosition(position, towers)) {
        std::cout << "Can't add tower on move path or another tower" << std::endl;
        delete tower;
    }
    else {
        if (reduceGold(tower->getGoldPrice()) && slotCount > 0) {
            addTower(tower);
            slotCount -= 1;
            std::cout << "Add tower" << "\n";
        }
        else {
            std::cout << "Not enough gold or slot to add tower" << "\n";
            delete tower; 
        }
    }
}

void GameEngine::initializeKeyStates() {
    keyStates[sf::Keyboard::R] = false;
    keyStates[sf::Keyboard::Q] = false;
    keyStates[sf::Keyboard::W] = false;
    keyStates[sf::Keyboard::E] = false;
    keyStates[sf::Keyboard::A] = false;
    keyStates[sf::Keyboard::S] = false;
    keyStates[sf::Keyboard::D] = false;
    keyStates[sf::Keyboard::F] = false;
    keyStates[sf::Keyboard::Escape] = false;
}

void GameEngine::spawnEnemyThread() {
    isRunning = true;
    enemySpawnThread = std::thread([this]() {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            if (!isPaused) {
                std::lock_guard<std::mutex> lock(entityMutex);
                int random = randomBetween(0, 2);
                if (random == 0) {
                    addEnemy(spawnEnemyOnMovePath<FastEnemy>(movePaths[randomBetween(0, movePaths.size() - 1)].get()));
                    std::cout << "Add fast enemy" << "\n";
                }
                else if (random == 1) {
                    addEnemy(spawnEnemyOnMovePath<SlowEnemy>(movePaths[randomBetween(0, movePaths.size() - 1)].get()));
                    std::cout << "Add slow enemy" << "\n";
                }
                else if (random == 2) {
                    addEnemy(spawnEnemyOnMovePath<CloneEnemy>(movePaths[randomBetween(0, movePaths.size() - 1)].get()));
                    std::cout << "Add clone enemy" << "\n";
                }

                spawnedEnemyCount++;

                if (spawnedEnemyCount >= nextEnemyThreshold) {
                    slotCount += 1;
                    std::cout << "Slot increased! Total slots: " << slotCount << "\n";
                    nextEnemyThreshold = static_cast<int>(nextEnemyThreshold * 1.8);
                    std::cout << "Next threshold: " << nextEnemyThreshold << " enemies" << "\n";
                }
            }
        }
        });
}

void GameEngine::stopSpawnEnemyThread() {
    isRunning = false; 
    if (enemySpawnThread.joinable()) {
        enemySpawnThread.join();
    }
}

void GameEngine::startUpdateEntitiesThread() {
    isRunning = true;
    updateEntitiesThread = std::thread([this]() {
        sf::Clock clock;
        while (isRunning) {
            float deltaTime = clock.restart().asSeconds();

            {
                std::lock_guard<std::mutex> lock(entityMutex); 
                updateEntities(towers, deltaTime);
                updateEntities(enemies, deltaTime);
                updateEntities(bullets, deltaTime);

                goldCountLabel.setText("Gold: " + std::to_string(goldCount));
                slotCountLabel.setText("Slot: " + std::to_string(slotCount));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
        }
        });
}

void GameEngine::stopUpdateEntitiesThread() {
    isRunning = false;
    if (updateEntitiesThread.joinable()) {
        updateEntitiesThread.join(); 
    }
}

GameEngine::~GameEngine() {
    stopUpdateEntitiesThread();
    stopSpawnEnemyThread();
}

void GameEngine::run() {
    sf::Clock clock;
    spawnEnemyThread();
    startUpdateEntitiesThread();

    while (window.isOpen()) {
        {
            std::lock_guard<std::mutex> lock(entityMutex);
            handleEvent(*this);
        }

        if (isPaused) {
            window.draw(helpPanel);
            for (const auto& text : helpTexts) {
                window.draw(text);
            }
            pauseButton.render(window);
            window.display();
            continue;
        }

        float deltaTime = clock.restart().asSeconds();

        {
            std::lock_guard<std::mutex> lock(entityMutex);
            processCollisionsAndLogic(*this, deltaTime);
        }

        // Tháp bắn đạn
        for (auto& tower : towers) {
            std::lock_guard<std::mutex> lock(entityMutex);
            tower->tryShoot(enemies, bullets);
        }

        // Kiểm tra đạn và kẻ địch
        {
            std::lock_guard<std::mutex> lock(entityMutex);
            processCollisionsAndLogic(*this, deltaTime);
        }

        if (goldCount < 0) {
            restartButton.handleEvent(event, window);
        }

        renderGame(*this);
    }
    stopSpawnEnemyThread();
    stopUpdateEntitiesThread();
}