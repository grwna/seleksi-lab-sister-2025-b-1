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
    // --- Initialization ---
    SharedState sharedState;
    ControlWindow controlWindow(sharedState);
    MandelbrotWindow mandelbrotWindow(sharedState);

    // Use a smart pointer for the Julia window so it can be created and destroyed easily.
    std::unique_ptr<JuliaWindow> juliaWindow = nullptr;

    sf::Clock deltaClock;

    // --- Main Application Loop ---
    while (sharedState.is_running) {
        // --- Event Handling ---
        controlWindow.handleEvents();
        if (mandelbrotWindow.isOpen()) {
            mandelbrotWindow.handleEvents();
        }
        // Handle events for the Julia window only if it exists.
        if (juliaWindow && juliaWindow->isOpen()) {
            juliaWindow.setPosition();
            juliaWindow->handleEvents();
        }

        // --- Update Logic ---
        
        // Check if we need to create or destroy the Julia window.
        if (sharedState.show_julia_window && !juliaWindow) {
            // If the checkbox is ticked and the window doesn't exist, create it.
            juliaWindow = std::make_unique<JuliaWindow>(sharedState);
        } else if (!sharedState.show_julia_window && juliaWindow) {
            // If the checkbox is unticked and the window exists, destroy it.
            juliaWindow.reset();
        }
        
        ImGui::SFML::Update(controlWindow.getWindow(), deltaClock.restart());

        if (mandelbrotWindow.isOpen()) {
            mandelbrotWindow.update();
        }
        // Update the Julia window only if it exists.
        if (juliaWindow && juliaWindow->isOpen()) {
            juliaWindow->update();
        }
        
        // --- Rendering ---
        controlWindow.defineUI();

        sf::RenderWindow& cw = controlWindow.getWindow();
        cw.clear();
        ImGui::SFML::Render(cw);
        cw.display();

        if (mandelbrotWindow.isOpen()) {
            mandelbrotWindow.render();
        }
        // Render the Julia window only if it exists.
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
