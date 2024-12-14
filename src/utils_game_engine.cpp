#include "game_engine.h"
#include "Utils.h"

void initializeGame(GameEngine& engine) {
    int laneCouunt = randomBetween(1, 3);
    engine.towers.clear();
    engine.enemies.clear();
    engine.bullets.clear();
    engine.enemySpawnTimer = 0.0f;
    engine.goldCount = 300 * laneCouunt;
    engine.slotCount = 3 * laneCouunt;
    engine.isPaused = false;

    engine.view.setSize(static_cast<float>(engine.windowW), static_cast<float>(engine.windowH));
    engine.view.setCenter(static_cast<float>(engine.windowW) / 2.0f, static_cast<float>(engine.windowH) / 2.0f);
    engine.window.setView(engine.view);

    engine.windowBorder.setSize(sf::Vector2f(engine.windowW - 2.0f, engine.windowH - 2.0f));
    engine.windowBorder.setPosition(1.0f, 1.0f);
    engine.windowBorder.setOutlineThickness(33.0f);
    engine.windowBorder.setOutlineColor(sf::Color(128, 0, 128));
    engine.windowBorder.setFillColor(sf::Color::Transparent);

    engine.helpPanel.setSize(sf::Vector2f(engine.windowW, engine.windowH - 165.0f));
    engine.helpPanel.setPosition(1.0f, 165.0f);
    engine.helpPanel.setFillColor(sf::Color(148, 144, 144, 2));

    setHelpText(engine);

    setBackground("images/backgrounds", engine.backgroundSprite, engine.windowW, engine.windowH);

    engine.uiPanel.setPosition(0, 0);
    engine.uiPanel.setSize(engine.windowW, 160.0f);
    engine.uiPanel.setUIColor(sf::Color(0, 119, 255), sf::Color::Yellow, 4.0f);

    engine.restartButton.setPosition(engine.windowW * 0.863f, engine.windowH * 0.013f);
    engine.restartButton.setSize(190.0f, 50.0f);
    engine.restartButton.setText("Restart");
    engine.restartButton.setUIColor(sf::Color::Blue, sf::Color::Green, 4.0f);
    engine.restartButton.setOnClick([&engine]() {
        engine.resetGame();
        });

    engine.pauseButton.setPosition(engine.windowW * 0.863f, engine.windowH * 0.073f);
    engine.pauseButton.setSize(190.0f, 50.0f);
    engine.pauseButton.setText("Help/Pause");
    engine.pauseButton.setUIColor(sf::Color::Blue, sf::Color::Green, 4.0f);
    engine.pauseButton.setOnClick([&engine]() {
        engine.pauseGame();
        });

    engine.goldCountLabel.setPosition(engine.windowW * 0.01f, engine.windowH * 0.013f);
    engine.goldCountLabel.setSize(210.0f, 50.0f);
    engine.goldCountLabel.setText("Gold: " + std::to_string(engine.goldCount));
    engine.goldCountLabel.setUIColor(sf::Color(255, 40, 40), sf::Color::Green, 4.0f);

    engine.slotCountLabel.setPosition(engine.windowW * 0.01f, engine.windowH * 0.073f);
    engine.slotCountLabel.setSize(210.0f, 50.0f);
    engine.slotCountLabel.setText("Slot: " + std::to_string(engine.slotCount));
    engine.slotCountLabel.setUIColor(sf::Color(255, 40, 40), sf::Color::Green, 4.0f);

    engine.buildUpgradePanel.setPosition(engine.windowW * 0.168f, engine.windowH * 0.013f);
    engine.buildUpgradePanel.setSize(1020.0f, 122.0f);
    engine.buildUpgradePanel.setUIColor(sf::Color::Black, sf::Color::Green, 4.0f);

    engine.buildLabel.setPosition(engine.windowW * 0.445f, engine.windowH * 0.029f);
    engine.buildLabel.setSize(190.0f, 34.0f);
    engine.buildLabel.setText("(Q):Jump bullet tower -200 (W):Tracking tower -100 (E):Triple tower -130 (R):Delete +50%");
    engine.buildLabel.setUIColor(sf::Color::Transparent, sf::Color::Transparent, 0.0f);

    engine.upgradeLabel.setPosition(engine.windowW * 0.445f, engine.windowH * 0.077f);
    engine.upgradeLabel.setSize(190.0f, 34.0f);
    engine.upgradeLabel.setText("(A):Upgrade range -60 (S):Upgrade speed -70 (D):Upgrade damage -80 (F):Mega upgrade -200%");
    engine.upgradeLabel.setUIColor(sf::Color::Transparent, sf::Color::Transparent, 0.0f);

    engine.upgradeRange = new Upgrade(Upgrade::RANGE, 30.0f, 60);
    engine.upgradeDamage = new Upgrade(Upgrade::DAMAGE, 10.0f, 70);
    engine.upgradeAttackSpeed = new Upgrade(Upgrade::ATTACK_SPEED, 0.1f, 80);

    createRandomPaths(engine.windowW, engine.windowH, randomBetween(7, 13), laneCouunt, engine.movePaths);
}

void setHelpText(GameEngine& engine) {
    if (!engine.font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Can't load font!" << "\n";
    }

    std::string helpContent = loadTextFromFile("helptext.txt");

    std::vector<std::string> sections;
    std::stringstream ss(helpContent);
    std::string line;

    std::string currentSection;

    while (std::getline(ss, line)) {
        if (line.empty()) continue;  
        sections.push_back(line);
    }

    float yOffset = 5.0f; 

    for (const auto& section : sections) {
        sf::Text sectionText;
        sectionText.setFont(engine.font);
        sectionText.setString(section);
        sectionText.setCharacterSize(24);
        sectionText.setFillColor(sf::Color(255, 255, 0));
        sectionText.setPosition(engine.helpPanel.getPosition().x + 20, engine.helpPanel.getPosition().y + yOffset);

        if (section.find("Tower information:") != std::string::npos) {
            sectionText.setFillColor(sf::Color::Red);
        }
        else if (section.find("Enemy information:") != std::string::npos) {
            sectionText.setFillColor(sf::Color::Green);
        }
        else if (section.find("Upgrade information:") != std::string::npos) {
            sectionText.setFillColor(sf::Color(255, 100, 0));
        }
        else if (section.find("Game information:") != std::string::npos) {
            sectionText.setFillColor(sf::Color::Magenta);
        }
        else if (section.find("Developer information:") != std::string::npos) {
            sectionText.setFillColor(sf::Color::Blue);
        }

        engine.helpTexts.push_back(sectionText);

        yOffset += sectionText.getLocalBounds().height + 13.0f;
    }
}

void processCollisionsAndLogic(GameEngine& engine, float deltaTime) {
    if (engine.bullets.empty()) {
        for (auto enemyIt = engine.enemies.begin(); enemyIt != engine.enemies.end();) {
            auto& enemy = *enemyIt;

            if (enemy->hasReachedEnd()) {
                enemyIt = engine.enemies.erase(enemyIt);
                engine.goldCount -= enemy->getGoldDrop();
                std::cout << "Delete enemy\n";
            }
            else {
                ++enemyIt;
            }
        }
    }

    std::vector<std::unique_ptr<Enemy>> spawnedClones;
    for (auto bulletIt = engine.bullets.begin(); bulletIt != engine.bullets.end();) {
        bool shouldErase = false;

        auto bulletPos = (*bulletIt)->getPosition();
        if (bulletPos.x < 0 || bulletPos.x > engine.windowW || bulletPos.y < 0 || bulletPos.y > engine.windowH) {
            shouldErase = true;
        }

        for (auto enemyIt = engine.enemies.begin(); enemyIt != engine.enemies.end();) {
            auto& enemy = *enemyIt;

            if (enemy->hasReachedEnd()) {
                enemyIt = engine.enemies.erase(enemyIt);
                engine.goldCount -= enemy->getGoldDrop();
                std::cout << "Delete enemy\n";
            }
            // Nếu kẻ địch trúng đạn
            else if (distance((*bulletIt)->getPosition(), enemy->getPosition()) <= enemy->getSize()) {
                enemy->takeDamage((*bulletIt)->getDamage());
                shouldErase = true;

                if (enemy->isDead()) {
                    engine.addGold(enemy->getGoldDrop());

                    if (auto* cloneEnemy = dynamic_cast<CloneEnemy*>(enemy.get())) {
                        spawnedClones = std::move(cloneEnemy->getSpawnedClones());
                    }

                    for (auto& bullet : engine.bullets) {
                        if (auto* autoTargetBullet = dynamic_cast<AutoTargetBullet*>(bullet.get())) {
                            if (autoTargetBullet->getTarget() == enemy.get()) {
                                autoTargetBullet->loseTarget();
                            }
                        }
                        else if (auto* jumpBullet = dynamic_cast<JumpBullet*>(bullet.get())) {
                            if (jumpBullet->getTarget() == enemy.get()) {
                                jumpBullet->jump(engine.enemies);
                                shouldErase = false;
                            }
                        }
                    }

                    enemyIt = engine.enemies.erase(enemyIt);
                    std::cout << "Kill enemy\n";

                    for (auto& clone : spawnedClones) {
                        engine.enemies.push_back(std::move(clone));
                        std::cout << "Add mini enemy\n";
                    }
                }
                else {
                    ++enemyIt;
                }

                break;
            }
            else {
                ++enemyIt;
            }
        }

        if (auto* autoTargetBullet = dynamic_cast<AutoTargetBullet*>(bulletIt->get())) {
            if (autoTargetBullet->isTargetLost()) {
                shouldErase = true;
            }
        }
        else if (auto* jumpBullet = dynamic_cast<JumpBullet*>(bulletIt->get())) {
            if (jumpBullet->isDelete()) {
                shouldErase = true;
            }
            else if (shouldErase) {
                jumpBullet->jump(engine.enemies);
                shouldErase = false;
            }
        }

        if (shouldErase) {
            bulletIt = engine.bullets.erase(bulletIt);
        }
        else {
            ++bulletIt;
        }
    }
}

void renderGame(GameEngine& engine) {
    engine.window.clear();

    engine.window.draw(engine.backgroundSprite);

    renderEntities(engine.movePaths, engine.window);
    renderEntities(engine.towers, engine.window);
    renderEntities(engine.enemies, engine.window);
    renderEntities(engine.bullets, engine.window);

    engine.uiPanel.render(engine.window);
    engine.restartButton.render(engine.window);
    engine.pauseButton.render(engine.window);
    engine.goldCountLabel.render(engine.window);
    engine.slotCountLabel.render(engine.window);
    engine.buildUpgradePanel.render(engine.window);

    engine.buildLabel.render(engine.window);
    engine.upgradeLabel.render(engine.window);

    engine.window.draw(engine.windowBorder);

    engine.window.display();
}

void handleEvent(GameEngine& engine) {
    while (engine.window.pollEvent(engine.event)) {
        if (engine.event.type == sf::Event::Closed)
            engine.window.close();

        float minZoom = 1.0f;
        float maxZoom = 2.0f;
        static float currentZoom = 1.0f;

        sf::Vector2i mouseScreenPosition = sf::Mouse::getPosition(engine.window);
        sf::Vector2f mouseWorldPosition = engine.window.mapPixelToCoords(mouseScreenPosition);

        // Lăn chuột
        if (engine.event.type == sf::Event::MouseWheelScrolled) {
            if (!engine.isPaused) {
                float zoomFactor = 1.1f;
                if (engine.event.mouseWheelScroll.delta > 0 && currentZoom > minZoom) {
                    engine.view.zoom(1.0f / zoomFactor);
                    currentZoom /= zoomFactor;
                }
                else if (engine.event.mouseWheelScroll.delta < 0 && currentZoom < maxZoom) {
                    engine.view.zoom(zoomFactor);
                    currentZoom *= zoomFactor;
                }
                engine.window.setView(engine.view);
            }
        }

        // Click chuột
        if (engine.event.type == sf::Event::MouseButtonPressed) {
            // Click chuột trái
            if (engine.event.mouseButton.button == sf::Mouse::Left) {
                Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers);
                if (engine.restartButton.getGlobalBounds().contains(mouseWorldPosition)) {
                    engine.restartButton.handleEvent(engine.event, engine.window);
                }
                else if (engine.pauseButton.getGlobalBounds().contains(mouseWorldPosition)) {
                    engine.pauseButton.handleEvent(engine.event, engine.window);
                }
                else if (Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers)) {
                    if (towerSelected) {
                        towerSelected->showRange();
                    }
                }
            }
            // Click chuột phải
            else if (engine.event.mouseButton.button == sf::Mouse::Right) {

            }
        }

        // Click phím
        if (engine.event.type == sf::Event::KeyPressed) {
            if (!engine.keyStates[engine.event.key.code]) {
                engine.keyStates[engine.event.key.code] = true;

                if (engine.event.key.code == sf::Keyboard::Q) {
                    Tower* bulletJumpTower = new BulletJumpTower(mouseWorldPosition.x, mouseWorldPosition.y);
                    engine.addTowerInPosition(bulletJumpTower, mouseWorldPosition);
                }

                if (engine.event.key.code == sf::Keyboard::W) {
                    Tower* trackingTower = new TrackingTower(mouseWorldPosition.x, mouseWorldPosition.y);
                    engine.addTowerInPosition(trackingTower, mouseWorldPosition);
                }

                if (engine.event.key.code == sf::Keyboard::E) {
                    Tower* tripleTower = new TripleTower(mouseWorldPosition.x, mouseWorldPosition.y);
                    engine.addTowerInPosition(tripleTower, mouseWorldPosition);
                }

                if (engine.event.key.code == sf::Keyboard::R) {
                    Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers);
                    if (towerSelected) {
                        auto it = std::remove_if(engine.towers.begin(), engine.towers.end(),
                            [towerSelected](const std::unique_ptr<Tower>& tower) {
                                return tower.get() == towerSelected;
                            });
                        engine.addGold(towerSelected->getGoldPrice() / 2);
                        engine.slotCount += 1;
                        engine.towers.erase(it, engine.towers.end());
                        std::cout << "Delete tower" << "\n";
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers);

                    if (towerSelected) {
                        engine.applyUpgrade(engine.upgradeRange, towerSelected);
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers);

                    if (towerSelected) {
                        engine.applyUpgrade(engine.upgradeAttackSpeed, towerSelected);
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers);

                    if (towerSelected) {
                        engine.applyUpgrade(engine.upgradeDamage, towerSelected);
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                    Tower* towerSelected = getTowerAtPosition(mouseWorldPosition, engine.towers);

                    if (towerSelected) {
                        if (engine.reduceGold(towerSelected->getGoldPrice() * 2)) {
                            if (!towerSelected->hasUpgradeMega() && towerSelected->getUpgradeCount() == 0) {
                                towerSelected->upgradeMega();
                                std::cout << "Mega upgrade tower" << "\n";
                            }
                            else {
                                engine.addGold(towerSelected->getGoldPrice() * 2);
                                std::cout << "Base stats not yet fully upgraded or already upgrade mega" << "\n";
                            }
                        }
                        else {
                            std::cout << "Not enough gold to upgrade mega" << "\n";
                        }
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    engine.pauseButton.handleEvent(engine.event, engine.window);
                }
            }
        }
        else if (engine.event.type == sf::Event::KeyReleased) {
            engine.keyStates[engine.event.key.code] = false;
        }
    }
}