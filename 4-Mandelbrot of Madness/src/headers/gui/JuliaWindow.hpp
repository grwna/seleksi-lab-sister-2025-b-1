#pragma once

#include <SFML/Graphics.hpp>
#include "SharedState.hpp"
#include <string>
#include <iostream>

class JuliaWindow {
public:
    JuliaWindow(SharedState& state);
    ~JuliaWindow();
    void handleEvents();
    void update();
    void render();
    bool isOpen() const;

private:
    void regenerateTexture();

    SharedState& m_sharedState;
    sf::RenderWindow m_window;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};