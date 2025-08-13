#pragma once

#include <SFML/Graphics.hpp>
#include "SharedState.hpp"
#include "../../lib/imgui.h"
#include "../../lib/imgui-SFML.h"

class ControlWindow {
public:
    ControlWindow(SharedState& state);

    ~ControlWindow();

    void handleEvents();

    void defineUI();

    bool isOpen() const;

    sf::RenderWindow& getWindow();

private:
    SharedState& m_sharedState;
    sf::RenderWindow m_window;
};
