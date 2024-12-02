#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <SFML/System.hpp>
#include <cmath>
#include <random>
#include "tower.h"
#include "enemy.h"
#include "move_path.h"
#include <fstream>
#include <sstream>

//template <typename T>
//void addEntity(std::vector<std::unique_ptr<T>>& container, T* entity) {
//    container.emplace_back(entity);
//}

template <typename EnemyType>
Enemy* spawnEnemyOnMovePath(MovePath* movePath) {
    sf::Vector2f firstNodePosition = movePath->getFirstNodePosition();
    Enemy* enemy = new EnemyType(firstNodePosition.x, firstNodePosition.y);
    enemy->setPath(movePath->getMovePath(), 0);
    return enemy;
}

template <typename T>
void renderEntities(const std::vector<std::unique_ptr<T>>& container, sf::RenderWindow& window) {
    for (const auto& entity : container) {
        entity->render(window);
    }
}

template <typename T>
void updateEntities(const std::vector<std::unique_ptr<T>>& container, float deltaTime) {
    for (const auto& entity : container) {
        entity->update(deltaTime);
    }
}

void setBackground(const std::string& folderPath, sf::Sprite& backgroundSprite,float windowW, float windowH);

float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

std::string loadTextFromFile(const std::string& filePath);

Tower* getTowerAtPosition(const sf::Vector2f& clickPosition, const std::vector<std::unique_ptr<Tower>>& towers);

void createRandomPaths(float windowW, float windowH, int numNodes, int numMovePaths, std::vector<std::unique_ptr<MovePath>>& movePaths, float minDistance = 100.0f);

int randomBetween(int min, int max);

bool isClickOnPaths(const sf::Vector2f& mousePosition, const std::vector<std::unique_ptr<MovePath>>& movePaths);

#endif


