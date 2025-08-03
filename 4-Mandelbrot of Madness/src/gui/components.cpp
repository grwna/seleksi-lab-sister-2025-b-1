#include "../headers/gui/components.hpp"

TextBox::TextBox(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& initialValue) {
    m_value = initialValue;
    
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(50, 50, 50));
    m_shape.setOutlineThickness(2);
    m_shape.setOutlineColor(sf::Color(100, 100, 100));

    m_text.setFont(font);
    m_text.setString(m_value);
    m_text.setCharacterSize(20);
    m_text.setFillColor(sf::Color::White);
    updateTextPosition();
}

bool TextBox::handleEvent(sf::Event& event) {
    bool valueChanged = false;
    if (m_isActive && event.type == sf::Event::TextEntered) {
        // handle backspace
        if (event.text.unicode == '\b') { 
            if (!m_value.empty()) {
                m_value.pop_back();
                valueChanged = true;
            }
        // only numbers
        } else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            m_value += static_cast<char>(event.text.unicode);
            valueChanged = true;
        }

        if (valueChanged) {
            m_text.setString(m_value);
            updateTextPosition();
        }
    }
    return valueChanged;
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
    window.draw(m_text);
}

void TextBox::setActive(bool active) {
    m_isActive = active;
    m_shape.setOutlineColor(m_isActive ? sf::Color::White : sf::Color(100, 100, 100));
}

int TextBox::getIntValue() const {
    if (m_value.empty()) {
        return 0;
    }
    return std::stoi(m_value);
}

void TextBox::setValue(int value) {
    if (value < 0) value = 0; // Cegah nilai negatif
    m_value = std::to_string(value);
    m_text.setString(m_value);
    updateTextPosition();
}

sf::RectangleShape TextBox::getShape() const {
    return m_shape;
}

void TextBox::updateTextPosition() {
    m_text.setPosition(
        m_shape.getPosition().x + 10,
        m_shape.getPosition().y + (m_shape.getSize().y - m_text.getGlobalBounds().height) / 2 - 5
    );
}
