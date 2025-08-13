// #include "../../lib/imgui.h"
// #include "../../lib/imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "headers/gui/SharedState.hpp"
#include "headers/gui/ControlWindow.hpp"
#include "headers/gui/MandelbrotWindow.hpp"
#include "headers/gui/JuliaWindow.hpp"
#include <memory>

int main() {
    SharedState sharedState;
    ControlWindow controlWindow(sharedState);
    MandelbrotWindow mandelbrotWindow(sharedState);

    std::unique_ptr<JuliaWindow> juliaWindow = nullptr;

    sf::Clock deltaClock;

    while (sharedState.is_running) {
        controlWindow.handleEvents();
        if (mandelbrotWindow.isOpen()) {
            mandelbrotWindow.handleEvents();
        }
        if (juliaWindow && juliaWindow->isOpen()) {
            juliaWindow->handleEvents();
        }

        
        // show julia logic
        if (sharedState.show_julia_window && !juliaWindow) {
            juliaWindow = std::make_unique<JuliaWindow>(sharedState);
        } else if (!sharedState.show_julia_window && juliaWindow) {
            juliaWindow.reset();
        }
        
        ImGui::SFML::Update(controlWindow.getWindow(), deltaClock.restart());

        if (mandelbrotWindow.isOpen()) {
            mandelbrotWindow.update();
        }

        if (juliaWindow && juliaWindow->isOpen()) {
            juliaWindow->update();
        }
        
        // Rendering
        controlWindow.defineUI();

        sf::RenderWindow& cw = controlWindow.getWindow();
        cw.clear();
        ImGui::SFML::Render(cw);
        cw.display();

        if (mandelbrotWindow.isOpen()) {
            mandelbrotWindow.render();
        }
        
        if (juliaWindow && juliaWindow->isOpen()) {
            juliaWindow->render();
        }
        
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    return 0;
}
