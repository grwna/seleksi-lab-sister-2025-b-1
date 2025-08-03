#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class TextBox {
public:
    TextBox(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size, const string& initialValue);

    bool handleEvent(sf::Event& event); // true if value changes

    void draw(sf::RenderWindow& window);

    void setActive(bool active);

    int getIntValue() const;

    void setValue(int value); // untuk inkremen

    sf::RectangleShape getShape() const;

private:
    void updateTextPosition();

    sf::RectangleShape m_shape;
    
    sf::Text m_text;

    string m_value;

    bool m_isActive = false;
};
