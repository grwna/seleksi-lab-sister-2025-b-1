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

class Dropdown {
    public:
        Dropdown(sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& size, const vector<string>& options);
        
        // option change
        bool handleEvent(sf::Event& event, sf::RenderWindow& window);
        
        void draw(sf::RenderWindow& window);
        int getSelectedOptionIndex() const;

    private:
        sf::Font& m_font;
        sf::RectangleShape m_mainBox;
        sf::Text m_selectedText;
        vector<sf::RectangleShape> m_optionBoxes;
        vector<sf::Text> m_optionTexts;
        vector<string> m_options;
        bool m_isOpen = false;
        int m_selectedIndex = 0;
    };