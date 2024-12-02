#pragma once
#ifndef IENTITY_H
#define IENTITY_H

#include <SFML/Graphics.hpp>

class IEntity {
public:
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual ~IEntity() = default;
};

#endif
