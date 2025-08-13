#pragma once

#include <SFML/Graphics.hpp>
#include "SharedState.hpp"
#include "../../lib/imgui.h"
#include "../generator/serial.hpp"
#include "../generator/cpu.hpp"
#include "../generator/gpu.hpp"
#include <iostream>
#include <chrono>

class MandelbrotWindow {
public:
    MandelbrotWindow(SharedState& state);
    ~MandelbrotWindow();
    void handleEvents();
    void update();
    void render();
    bool isOpen() const;

private:
    void updateJuliaConstant(int mouseX, int mouseY);
    void regenerateTexture();

    void zoom(float delta, int mouseX, int mouseY);
    void pan(const sf::Vector2i& pan_end);

    SharedState& m_sharedState;
    sf::RenderWindow m_window;
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    Bounds m_bounds = {-2.0, 1.0, -1.5, 1.5};

    bool m_is_panning = false;
    sf::Vector2i m_pan_start_pos;
};