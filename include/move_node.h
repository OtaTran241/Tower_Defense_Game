#pragma once
#include <SFML/Graphics.hpp>

class MoveNode {
private:
    sf::Vector2f position;

public:
    MoveNode(float x, float y);
    sf::Vector2f getPosition() const;
};
