#include "UI.h"
#include <iostream>
#include <stdexcept>

UI::UI() {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Can't load font!" << "\n";
    }
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    shape.setFillColor(sf::Color::Red);
}

void UI::setPosition(float x, float y) {
    shape.setPosition(x, y);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (shape.getSize().x - textBounds.width) / 2 - textBounds.left,
        y + (shape.getSize().y - textBounds.height) / 2 - textBounds.top
    );
}

void UI::setSize(float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    text.setCharacterSize(static_cast<unsigned int>(height * 0.7f));

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        shape.getPosition().x + (width - textBounds.width) / 2 - textBounds.left,
        shape.getPosition().y + (height - textBounds.height) / 2 - textBounds.top
    );
}

void UI::setText(const std::string& buttonText) {
    text.setString(buttonText);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        shape.getPosition().x + (shape.getSize().x - textBounds.width) / 2 - textBounds.left,
        shape.getPosition().y + (shape.getSize().y - textBounds.height) / 2 - textBounds.top
    );
}

void UI::setOnClick(const std::function<void()>& callback) {
    onClick = callback;
}

void UI::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void UI::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (onClick) {
        onClick(); 
    }
}


sf::FloatRect UI::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

void UI::setUIColor(const sf::Color& fillColor, const sf::Color& outlineColor, float outlineThickness) {
    shape.setFillColor(fillColor); 
    shape.setOutlineColor(outlineColor); 
    shape.setOutlineThickness(outlineThickness);  
}