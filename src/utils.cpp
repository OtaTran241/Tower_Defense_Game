#include "Utils.h"
#include <iostream>

void setBackground(const std::string& folderPath, sf::Sprite& backgroundSprite, float windowW, float windowH) {
    static sf::Texture backgroundTexture;
    std::string filePath = folderPath + "/BG.png";

    //std::string filePath = folderPath + "/BG" + std::to_string(randomBetween(1, 3)) + ".jpg";

    if (!backgroundTexture.loadFromFile(filePath)) {
        std::cout << "Error: Failed to load background image from: " << filePath << std::endl;
        return;
    }
    else
    {
        std::cout << "load background image from: " << filePath << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = windowW / static_cast<float>(textureSize.x);
    float scaleY = (windowH - 160.0f) / static_cast<float>(textureSize.y);
    backgroundSprite.setScale(scaleX, scaleY);

    float backgroundWidth = textureSize.x * scaleX;
    float backgroundHeight = textureSize.y * scaleY;
    backgroundSprite.setPosition((windowW - backgroundWidth) / 2.0f, (windowH + 160.0f - backgroundHeight) / 2.0f);
}

float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

Tower* getTowerAtPosition(const sf::Vector2f& clickPosition, const std::vector<std::unique_ptr<Tower>>& towers) {
    for (const auto& tower : towers) {
        if (!tower) continue;

        if (tower->getGlobalBounds().contains(clickPosition)) {
            return tower.get();
        }
    }
    return nullptr;
}

void createRandomPaths(float windowW, float windowH, int numNodes, int numMovePaths, std::vector<std::unique_ptr<MovePath>>& movePaths, float minDistance) {
    movePaths.clear();

    for (int pathIndex = 0; pathIndex < numMovePaths; ++pathIndex) {
        int grayValue = randomBetween(0, 128);
        sf::Color randomGray(grayValue, grayValue, grayValue);

        auto movePath = std::make_unique<MovePath>(50.0f, randomGray);

        int directionType = randomBetween(1, 2);

        if (directionType == 1) {
            // Hướng từ trên xuống dưới
            float x = static_cast<float>(randomBetween(0, static_cast<int>(windowW))); 
            float y = 160.0f; 
            movePath->addNode(MoveNode(x, y));

            for (int i = 1; i < numNodes; ++i) {
                float stepY = static_cast<float>(randomBetween(50, 150));
                y += stepY;
                if (y > windowH) {
                    y = windowH;
                }

                float stepX = static_cast<float>(randomBetween(0, static_cast<int>(windowW * 0.25f)));
                float xDirection = (randomBetween(0, 1) == 0) ? 1.0f : -1.0f;
                x += stepX * xDirection;

                if (x < 50.0f) x = 50.0f;                 
                if (x > windowW) x = windowW - 20.0f; 

                // Kiểm tra chồng lấn
                bool valid = true;
                for (const auto& otherPath : movePaths) {
                    for (const auto& otherNode : otherPath->getMovePath()) {
                        if (distance(sf::Vector2f(x, y), otherNode.getPosition()) < minDistance) {
                            valid = false;
                            break;
                        }
                    }
                    if (!valid) break;
                }

                if (valid) {
                    movePath->addNode(MoveNode(x, y));
                }
                else {
                    --i;
                }
            }

            if (y < windowH) {
                y = windowH;
                movePath->addNode(MoveNode(x, y));
            }
        }
        else {
            // Hướng từ trái sang phải
            float x = 0.0f; 
            float y = static_cast<float>(randomBetween(160.0f, static_cast<int>(windowH)));
            movePath->addNode(MoveNode(x, y));

            for (int i = 1; i < numNodes; ++i) {
                float stepX = static_cast<float>(randomBetween(50, 150));
                x += stepX;
                if (x > windowW) {
                    x = windowW;
                }

                float stepY = static_cast<float>(randomBetween(160.0f, static_cast<int>(windowH * 0.25f)));
                float yDirection = (randomBetween(0, 1) == 0) ? 1.0f : -1.0f;
                y += stepY * yDirection;

                if (y < 170.0f) y = 170.0f;                     // Không vượt quá biên trên
                if (y > windowH) y = windowH - 50.0f; // Không vượt quá biên dưới

                // Kiểm tra chồng lấn
                bool valid = true;
                for (const auto& otherPath : movePaths) {
                    for (const auto& otherNode : otherPath->getMovePath()) {
                        if (distance(sf::Vector2f(x, y), otherNode.getPosition()) < minDistance) {
                            valid = false;
                            break;
                        }
                    }
                    if (!valid) break;
                }

                if (valid) {
                    movePath->addNode(MoveNode(x, y));
                }
                else {
                    --i; // Nếu không hợp lệ, sinh lại node
                }
            }

            if (x < windowW) {
                x = windowW;
                movePath->addNode(MoveNode(x, y));
            }
        }

        movePaths.push_back(std::move(movePath));
    }
}

bool isClickOnPaths(const sf::Vector2f& mousePosition, const std::vector<std::unique_ptr<MovePath>>& movePaths) {
    for (const auto& movePath : movePaths) {
        const auto& segmentBounds = movePath->getSegmentBounds();
        const auto& moveNodes = movePath->getMovePath();

        for (size_t i = 0; i < segmentBounds.size(); ++i) {
            // Lấy vị trí đầu và cuối của đoạn đường
            sf::Vector2f start = moveNodes[i].getPosition();
            sf::Vector2f end = moveNodes[i + 1].getPosition();

            // Tính vector vuông góc và khoảng cách
            sf::Vector2f direction = end - start;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length == 0.0f) continue;

            sf::Vector2f normal = { -direction.y / length, direction.x / length }; // Vector pháp tuyến

            // Kiểm tra nếu click nằm trong vùng hình chữ nhật mở rộng của đoạn đường
            sf::Vector2f toMouse = mousePosition - start;
            float dotAlong = (toMouse.x * direction.x + toMouse.y * direction.y) / length;
            float dotPerpendicular = std::abs(toMouse.x * normal.x + toMouse.y * normal.y);

            if (dotAlong >= 0 && dotAlong <= length && dotPerpendicular <= segmentBounds[i].y / 2.0f) {
                std::cout << movePath;
                return true;
            }
        }
    }
    return false;
}

int randomBetween(int min, int max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::string loadTextFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "Error loading help file.";
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();
}