#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "app_state.hpp"
#include "components.hpp"

class ControlWindow {
public:
    ControlWindow(SharedState& state, sf::Font& font);
    void handleEvents();
    void draw();
    bool isOpen() const;

private:
    void updateParameters();
    void updateActiveTextBox(); // BARU: Helper untuk mengupdate text box aktif

    sf::RenderWindow m_window;
    SharedState& m_sharedState;
    sf::Font& m_font;

    vector<unique_ptr<TextBox>> m_textBoxes;
    vector<unique_ptr<TextBox>> m_stepBoxes;
    vector<sf::RectangleShape> m_buttons; // BARU: Untuk tombol +/-
    vector<sf::Text> m_buttonLabels;      // BARU: Untuk label tombol
    unique_ptr<Dropdown> m_modeDropdown;

    size_t m_activeTextBoxIndex = 0; // BARU: Melacak text box yang aktif
};
