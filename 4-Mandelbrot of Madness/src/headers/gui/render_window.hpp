#pragma once

#include <SFML/Graphics.hpp>
#include "app_state.hpp"
#include "../generator/serial.hpp"
#include "../generator/cpu.hpp"
#include "../generator/gpu.hpp"
#include <iostream>
#include <chrono>

class RenderWindow {
public:
    RenderWindow(SharedState& state);
    void handleEvents();
    void update();
    void draw();
    bool isOpen() const;

private:
    void regenerateTexture();

    sf::RenderWindow m_window;
    SharedState& m_sharedState;

    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Text m_statusText;
    sf::Font m_font;
};
