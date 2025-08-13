#include "../headers/gui/MandelbrotWindow.hpp"

#include <chrono>
#include <iostream>


Colors mandelbrotSerial(int width, int height, int max_iters, const Bounds& bounds, const ColorScheme& scheme);
Colors mandelbrotCPU(int width, int height, int max_iters, const Bounds& bounds, const ColorScheme& scheme);
Colors mandelbrotGPU(int width, int height, int max_iters, const Bounds& bounds, const ColorScheme& scheme);
Color getColor(int n, int max_iters, const ColorScheme& scheme);


MandelbrotWindow::MandelbrotWindow(SharedState& state)
    : m_sharedState(state),
      m_window(sf::VideoMode(state.mandelbrot_params.width, state.mandelbrot_params.height), "Mandelbrot Set") {

    m_window.setFramerateLimit(60);
    m_window.setPosition({500, 50});
    if (!m_texture.create(m_sharedState.mandelbrot_params.width, m_sharedState.mandelbrot_params.height)) {
        cerr << "MandelbrotWindow: Failed to create texture." << endl;
    }
    m_sprite.setTexture(m_texture);
}

MandelbrotWindow::~MandelbrotWindow() {}

bool MandelbrotWindow::isOpen() const {
    return m_window.isOpen();
}

void MandelbrotWindow::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_sharedState.is_running = false;
            m_window.close();
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            zoom(event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                m_is_panning = true;
                m_pan_start_pos = {event.mouseButton.x, event.mouseButton.y};
            }
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                m_is_panning = false;
            }
        }
        // pan or julia?
        if (event.type == sf::Event::MouseMoved) {
            if (m_is_panning) {
                pan({event.mouseMove.x, event.mouseMove.y});
                m_pan_start_pos = {event.mouseMove.x, event.mouseMove.y};
            }
            else if (m_sharedState.show_julia_window) {
                updateJuliaConstant(event.mouseMove.x, event.mouseMove.y);
            }
        }
    }
}

void MandelbrotWindow::updateJuliaConstant(int mouseX, int mouseY) {
    sf::Vector2u windowSize = m_window.getSize();
    if (mouseX < 0 || mouseX >= windowSize.x || mouseY < 0 || mouseY >= windowSize.y) return;
    float real = m_bounds.x_min + ((float)mouseX / windowSize.x) * (m_bounds.x_max - m_bounds.x_min);
    float imag = m_bounds.y_min + ((float)mouseY / windowSize.y) * (m_bounds.y_max - m_bounds.y_min);
    m_sharedState.julia_c = Complex(real, imag);
    m_sharedState.julia_needs_update = true;
}

void MandelbrotWindow::zoom(float delta, int mouseX, int mouseY) {
    const float zoom_factor = 1.1f;
    sf::Vector2u windowSize = m_window.getSize();

    double mouse_real = m_bounds.x_min + ((double)mouseX / windowSize.x) * (m_bounds.x_max - m_bounds.x_min);
    double mouse_imag = m_bounds.y_min + ((double)mouseY / windowSize.y) * (m_bounds.y_max - m_bounds.y_min);

    double new_x_range, new_y_range;

    if (delta > 0) {
        new_x_range = (m_bounds.x_max - m_bounds.x_min) / zoom_factor;
        new_y_range = (m_bounds.y_max - m_bounds.y_min) / zoom_factor;
        m_sharedState.mandelbrot_params.zoom *= zoom_factor;
    } else {
        new_x_range = (m_bounds.x_max - m_bounds.x_min) * zoom_factor;
        new_y_range = (m_bounds.y_max - m_bounds.y_min) * zoom_factor;
        m_sharedState.mandelbrot_params.zoom /= zoom_factor;
    }

    double ratio_x = ((double)mouseX / windowSize.x);
    double ratio_y = ((double)mouseY / windowSize.y);
    m_bounds.x_min = mouse_real - new_x_range * ratio_x;
    m_bounds.x_max = mouse_real + new_x_range * (1.0 - ratio_x);
    m_bounds.y_min = mouse_imag - new_y_range * ratio_y;
    m_bounds.y_max = mouse_imag + new_y_range * (1.0 - ratio_y);

    m_sharedState.mandelbrot_needs_update = true;
}

void MandelbrotWindow::pan(const sf::Vector2i& pan_end) {
    sf::Vector2u windowSize = m_window.getSize();
    double dx = (double)(pan_end.x - m_pan_start_pos.x) / windowSize.x * (m_bounds.x_max - m_bounds.x_min);
    double dy = (double)(pan_end.y - m_pan_start_pos.y) / windowSize.y * (m_bounds.y_max - m_bounds.y_min);

    m_bounds.x_min -= dx;
    m_bounds.x_max -= dx;
    m_bounds.y_min -= dy;
    m_bounds.y_max -= dy;

    m_sharedState.mandelbrot_needs_update = true;
}

void MandelbrotWindow::update() {
    if (m_sharedState.mandelbrot_needs_update) {
        regenerateTexture();
        m_sharedState.mandelbrot_needs_update = false;
    }
}

void MandelbrotWindow::render() {
    m_window.clear(sf::Color::Black);
    m_window.draw(m_sprite);
    m_window.display();
}

void MandelbrotWindow::regenerateTexture() {
    auto start = chrono::high_resolution_clock::now();
    RenderParameters& params = m_sharedState.mandelbrot_params;
    if (m_window.getSize().x != params.width || m_window.getSize().y != params.height) {
        m_window.setSize({params.width, params.height});
        if (!m_texture.create(params.width, params.height)) return;
        sf::View view = m_window.getDefaultView();
        view.setSize(static_cast<float>(params.width), static_cast<float>(params.height));
        view.setCenter(static_cast<float>(params.width) / 2.f, static_cast<float>(params.height) / 2.f);
        m_window.setView(view);
    }
    Colors pixels;
    string modeStr = "Unknown";
    switch(m_sharedState.mode) {
        case Mode::SERIAL:
            pixels = mandelbrotSerial(params.width, params.height, params.max_iterations, m_bounds, m_sharedState.color_scheme);
            modeStr = "Serial Mandelbrot";
            break;
        case Mode::CPU:
            pixels = mandelbrotCPU(params.width, params.height, params.max_iterations, m_bounds, m_sharedState.color_scheme);
            modeStr = "CPU Mandelbrot";
            break;
        case Mode::GPU:
            pixels = mandelbrotGPU(params.width, params.height, params.max_iterations, m_bounds, m_sharedState.color_scheme);
            modeStr = "GPU Mandelbrot";
            break;
    }
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