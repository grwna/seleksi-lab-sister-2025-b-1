#include "../headers/gui/components.hpp"

// ========================================================================================================================
//                                                       TEXTBOX 
// ========================================================================================================================

TextBox::TextBox(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size, const string& initialValue) {
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
    return stoi(m_value);
}

void TextBox::setValue(int value) {
    if (value < 0) value = 0; // Cegah nilai negatif
    m_value = to_string(value);
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

// ========================================================================================================================
//                                                       DROPDOWN 
// ========================================================================================================================

Dropdown::Dropdown(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size, const vector<string>& options)
    : m_font(font), m_options(options) {
    
    m_mainBox.setPosition(position);
    m_mainBox.setSize(size);
    m_mainBox.setFillColor(sf::Color(80, 80, 80));
    m_mainBox.setOutlineThickness(1);
    m_mainBox.setOutlineColor(sf::Color(150, 150, 150));

    m_selectedText.setFont(m_font);
    m_selectedText.setCharacterSize(size.y * 0.6);
    m_selectedText.setFillColor(sf::Color::White);
    if (!m_options.empty()) {
        m_selectedText.setString(m_options[0]);
    }
    m_selectedText.setPosition(position.x + 10, position.y + (size.y - m_selectedText.getGlobalBounds().height) / 2 - 5);

    for (size_t i = 0; i < options.size(); ++i) {
        sf::RectangleShape optionBox(size);
        optionBox.setPosition(position.x, position.y + (i + 1) * size.y);
        optionBox.setFillColor(sf::Color(50, 50, 50));
        optionBox.setOutlineColor(sf::Color(150, 150, 150));
        optionBox.setOutlineThickness(1);
        m_optionBoxes.push_back(optionBox);

        sf::Text optionText(options[i], m_font, size.y * 0.6);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition(optionBox.getPosition().x + 10, optionBox.getPosition().y + (size.y - optionText.getGlobalBounds().height) / 2 - 5);
        m_optionTexts.push_back(optionText);
    }
}

bool Dropdown::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        
        if (m_isOpen) {
            for (size_t i = 0; i < m_optionBoxes.size(); ++i) {
                if (m_optionBoxes[i].getGlobalBounds().contains(mousePos)) {
                    if (m_selectedIndex != i) {
                        m_selectedIndex = i;
                        m_selectedText.setString(m_options[i]);
                        m_isOpen = false;
                        return true;
                    }
                }
            }
        }
        
        if (m_mainBox.getGlobalBounds().contains(mousePos)) {
            m_isOpen = !m_isOpen;
        } else {
            m_isOpen = false;
        }
    }
    return false;
}

void Dropdown::draw(sf::RenderWindow& window) {
    window.draw(m_mainBox);
    window.draw(m_selectedText);
    
    if (m_isOpen) {
        for (const auto& box : m_optionBoxes) {
            window.draw(box);
        }
        for (const auto& text : m_optionTexts) {
            window.draw(text);
        }
    }
}

int Dropdown::getSelectedOptionIndex() const {
    return m_selectedIndex;
}
