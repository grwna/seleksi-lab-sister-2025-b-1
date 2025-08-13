#include "../headers/gui/JuliaWindow.hpp"
#include <chrono>
#include <iostream>

Colors juliaSerial(int width, int height, int max_iterations, const Complex& c, const ColorScheme& scheme);
Colors juliaCPU(int width, int height, int max_iterations, const Complex& c, const ColorScheme& scheme);
Colors juliaGPU(int width, int height, int max_iterations, const Complex& c, const ColorScheme& scheme);
Color getColor(int n, int max_iters, const ColorScheme& scheme);

JuliaWindow::JuliaWindow(SharedState& state)
    : m_sharedState(state), m_window(sf::VideoMode(state.julia_params.width, state.julia_params.height), "Julia Set") {

    m_window.setFramerateLimit(60);
    m_window.setPosition({50, 700});

    if (!m_texture.create(m_sharedState.julia_params.width, m_sharedState.julia_params.height)) {
        cerr << "JuliaWindow: Failed to create texture." << endl;
    }
    m_sprite.setTexture(m_texture);
}

JuliaWindow::~JuliaWindow() {}

bool JuliaWindow::isOpen() const {
    return m_window.isOpen();
}

void JuliaWindow::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_sharedState.show_julia_window = false; 
            m_window.close();
        }
    }
}

void JuliaWindow::update() {
    if (m_sharedState.julia_needs_update) {
        regenerateTexture();
        m_sharedState.julia_needs_update = false;
    }
}

void JuliaWindow::render() {
    m_window.clear(sf::Color::Black);
    m_window.draw(m_sprite);
    m_window.display();
}

void JuliaWindow::regenerateTexture() {
    auto start = chrono::high_resolution_clock::now();
    
    RenderParameters& params = m_sharedState.julia_params;
    unsigned int newWidth = params.width;
    unsigned int newHeight = params.height;

    if (m_window.getSize().x != newWidth || m_window.getSize().y != newHeight) {
        m_window.setSize({newWidth, newHeight});
        if (!m_texture.create(newWidth, newHeight)) {
             cerr << "JuliaWindow: Failed to resize texture." << endl;
             return;
        }
        sf::View view = m_window.getDefaultView();
        view.setSize(static_cast<float>(newWidth), static_cast<float>(newHeight));
        view.setCenter(static_cast<float>(newWidth) / 2.f, static_cast<float>(newHeight) / 2.f);
        m_window.setView(view);
    }

    Colors pixels;
    string modeStr = "Unknown";
    
    switch(m_sharedState.mode) {
        case Mode::SERIAL:
            pixels = juliaSerial(params.width, params.height, params.max_iterations, m_sharedState.julia_c, m_sharedState.color_scheme);
            modeStr = "Serial Julia";
            break;
        case Mode::CPU:
            pixels = juliaCPU(params.width, params.height, params.max_iterations, m_sharedState.julia_c, m_sharedState.color_scheme);
            modeStr = "CPU Julia";
            break;
        case Mode::GPU:
            pixels = juliaGPU(params.width, params.height, params.max_iterations, m_sharedState.julia_c, m_sharedState.color_scheme);
            modeStr = "GPU Julia";
            break;
    }

    // Image creation
    if (!pixels.empty() && params.width > 0 && params.height > 0) {
        sf::Image image;
        image.create(params.width, params.height);
        for (int y = 0; y < params.height; ++y) {
            for (int x = 0; x < params.width; ++x) {
                Color c = pixels[y * params.width + x];
                image.setPixel(x, y, sf::Color(c.r, c.g, c.b));
            }
        }
        m_texture.update(image);
    }
    
    m_sprite.setTexture(m_texture, true);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "(" << modeStr << ", " << params.width << "x" << params.height << ", " << params.max_iterations << " iters)";
    cout << " Work done in: " << duration.count() << " seconds" << endl;
}
