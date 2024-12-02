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

void GameEngine::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            float minZoom = 1.0f;
            float maxZoom = 2.0f;
            static float currentZoom = 1.0f;

            sf::Vector2i mouseScreenPosition = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPosition = window.mapPixelToCoords(mouseScreenPosition);

            // Lăn chuột
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (!isPaused) {
                    float zoomFactor = 1.1f;
                    if (event.mouseWheelScroll.delta > 0 && currentZoom > minZoom) {
                        view.zoom(1.0f / zoomFactor);
                        currentZoom /= zoomFactor;
                    }
                    else if (event.mouseWheelScroll.delta < 0 && currentZoom < maxZoom) {
                        view.zoom(zoomFactor);
                        currentZoom *= zoomFactor;
                    }
                    window.setView(view);
                }
            }

            // Click chuột
            if (event.type == sf::Event::MouseButtonPressed) {
                // Click chuột trái
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, towers);
                    if (restartButton.getGlobalBounds().contains(mouseWorldPosition)) {
                        restartButton.handleEvent(event, window);
                    }
                    else if (pauseButton.getGlobalBounds().contains(mouseWorldPosition)) {
                        pauseButton.handleEvent(event, window);
                    }
                    else if (Tower * towerSelected = getTowerAtPosition(mouseWorldPosition, towers)) {
                        if (towerSelected) {
                            towerSelected->showRange();
                        }
                    }
                }
                // Click chuột phải
                else if (event.mouseButton.button == sf::Mouse::Right) {

                }
            }

            // Click phím
            if (event.type == sf::Event::KeyPressed) {
                if (!keyStates[event.key.code]) {
                    keyStates[event.key.code] = true;

                    if (event.key.code == sf::Keyboard::Q) {
                        Tower* bulletJumpTower = new BulletJumpTower(mouseWorldPosition.x, mouseWorldPosition.y);
                        addTowerInPosition(bulletJumpTower, mouseWorldPosition);
                    }

                    if (event.key.code == sf::Keyboard::W) {
                        Tower* trackingTower = new TrackingTower(mouseWorldPosition.x, mouseWorldPosition.y);
                        addTowerInPosition(trackingTower, mouseWorldPosition);
                    }

                    if (event.key.code == sf::Keyboard::E) {
                        Tower* tripleTower = new TripleTower(mouseWorldPosition.x, mouseWorldPosition.y);
                        addTowerInPosition(tripleTower, mouseWorldPosition);
                    }

                    if (event.key.code == sf::Keyboard::R) {
                        Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, towers);
                        if (towerSelected) {
                            auto it = std::remove_if(towers.begin(), towers.end(),
                                [towerSelected](const std::unique_ptr<Tower>& tower) {
                                    return tower.get() == towerSelected;
                                });
                            addGold(towerSelected->getGoldPrice() / 2);
                            slotCount += 1;
                            towers.erase(it, towers.end());
                            std::cout << "Delete tower" << "\n";
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, towers);

                        if (towerSelected) {
                            applyUpgrade(upgradeRange, towerSelected);
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, towers);

                        if (towerSelected) {
                            applyUpgrade(upgradeAttackSpeed, towerSelected);
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                        Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, towers);

                        if (towerSelected) {
                            applyUpgrade(upgradeDamage, towerSelected);
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                        Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, towers);

                        if (towerSelected) {
                            if (reduceGold(towerSelected->getGoldPrice() * 2)) {
                                if (!towerSelected->hasUpgradeMega() && towerSelected->getUpgradeCount() == 0) {
                                    towerSelected->upgradeMega();
                                    std::cout << "Mega upgrade tower" << "\n";
                                }
                                else {
                                    addGold(towerSelected->getGoldPrice() * 2);
                                    std::cout << "Base stats not yet fully upgraded or already upgrade mega" << "\n";
                                }
                            }
                            else {
                                std::cout << "Not enough gold to upgrade mega" << "\n";
                            }
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                        pauseButton.handleEvent(event, window);
                    }
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                keyStates[event.key.code] = false;
            }
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

        // Sinh kẻ địch tự động
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= enemySpawnInterval) {
            enemySpawnTimer = randomBetween(0,1);
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

        // Tháp bắn đạn
        for (auto& tower : towers) {
            tower->tryShoot(enemies, bullets);
        }

        // Kiểm tra đạn và kẻ địch
        processCollisionsAndLogic(*this, deltaTime);

        if (goldCount < 0) {
            restartButton.handleEvent(event, window);
        }

        updateEntities(towers, deltaTime);
        updateEntities(enemies, deltaTime);
        updateEntities(bullets, deltaTime);

        goldCountLabel.setText("Gold: " + std::to_string(goldCount));
        slotCountLabel.setText("Slot: " + std::to_string(slotCount));

        renderGame(*this);
    }
}