#pragma once
#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class UI {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    std::function<void()> onClick; 

public:
    UI();

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setText(const std::string& buttonText);
    void setOnClick(const std::function<void()>& callback);
    void render(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    sf::FloatRect getGlobalBounds() const;
    void setUIColor(const sf::Color& fillColor, const sf::Color& outlineColor, float outlineThickness = 1.0f);
};

#endif 


