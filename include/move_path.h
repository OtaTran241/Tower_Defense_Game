#pragma once
#include <SFML/Graphics.hpp>
#include "Move_Node.h"
#include <vector>

class MovePath {
private:
    std::vector<MoveNode> movePath;
    std::vector<sf::RectangleShape> segments;
    std::vector<sf::Vector2f> segmentBounds;
    float width;
    sf::Color color;

    void createSegments(); 

public:
    MovePath(float width = 20.0f, sf::Color color = sf::Color::White);
    void addNode(const MoveNode& node); 
    void render(sf::RenderWindow& window); 
    const std::vector<MoveNode>& getMovePath() const;
    const std::vector<sf::Vector2f>& getSegmentBounds() const;
    void clearMovePath();
    sf::Vector2f getFirstNodePosition() const;
};
