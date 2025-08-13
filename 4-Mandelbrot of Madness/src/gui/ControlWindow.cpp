#include "../headers/gui/ControlWindow.hpp"


void trigger_update(SharedState& state, FractalType type) {
    if (type == FractalType::MANDELBROT) {
        if (state.mandelbrot_params.width < 1) state.mandelbrot_params.width = 1;
        if (state.mandelbrot_params.height < 1) state.mandelbrot_params.height = 1;
        if (state.mandelbrot_params.max_iterations < 1) state.mandelbrot_params.max_iterations = 1;
        
        state.mandelbrot_needs_update = true;
    } else {
        if (state.julia_params.width < 1) state.julia_params.width = 1;
        if (state.julia_params.height < 1) state.julia_params.height = 1;
        if (state.julia_params.max_iterations < 1) state.julia_params.max_iterations = 1;

        state.julia_needs_update = true;
    }
}

ControlWindow::ControlWindow(SharedState& state) 
    : m_sharedState(state), m_window(sf::VideoMode(400, 600), "Control Panel")  {

    m_window.setFramerateLimit(60);
    ImGui::SFML::Init(m_window);

    m_window.setPosition({50, 50});

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}

ControlWindow::~ControlWindow() {
    ImGui::SFML::Shutdown();
}

sf::RenderWindow& ControlWindow::getWindow() {
    return m_window;
}

void ControlWindow::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(m_window, event);
        if (event.type == sf::Event::Closed) {
            m_sharedState.is_running = false;
        }
    }
}

bool ControlWindow::isOpen() const {
    return m_window.isOpen();
}

void ControlWindow::defineUI() {
    ImGui::SetNextWindowSize(ImVec2(390, 690));
    ImGui::SetNextWindowPos(ImVec2(5, 5));
    ImGui::Begin("Fractal Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowPos({0, 0});
    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);


    ImGui::Text("Global Settings");
    ImGui::Separator();
    
    float label_width = 55;
    float input_width = 80;

    ImGui::Text("Mode");
    ImGui::SameLine(label_width);
    ImGui::PushItemWidth(-1); 
    const char* modes[] = { "Serial", "CPU (OpenMP)", "GPU (CUDA)" };
    int current_mode_index = static_cast<int>(m_sharedState.mode);
    if (ImGui::Combo("|", &current_mode_index, modes, IM_ARRAYSIZE(modes))) {
        m_sharedState.mode = static_cast<Mode>(current_mode_index);
        m_sharedState.mandelbrot_needs_update = true;
        m_sharedState.julia_needs_update = true;
    }
    ImGui::PopItemWidth();
    
    
    ImGui::Separator();

    ImGui::Text("Mandelbrot Settings");
    bool mandelbrot_changed = false;
    

    ImGui::Text("Width");
    ImGui::SameLine(label_width);
    ImGui::PushItemWidth(input_width);
    if (ImGui::InputInt("##MandelWidth", &m_sharedState.mandelbrot_params.width, 10, 100)) mandelbrot_changed = true;
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::SliderInt("##MandelWidthSlider", &m_sharedState.mandelbrot_params.width, 1, 3840)) mandelbrot_changed = true;
    ImGui::PopItemWidth();

    ImGui::Text("Height");
    ImGui::SameLine(label_width);
    ImGui::PushItemWidth(input_width);
    if (ImGui::InputInt("##MandelHeight", &m_sharedState.mandelbrot_params.height, 10, 100)) mandelbrot_changed = true;
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::SliderInt("##MandelHeightSlider", &m_sharedState.mandelbrot_params.height, 1, 2160)) mandelbrot_changed = true;
    ImGui::PopItemWidth();

    ImGui::Text("Iters");
    ImGui::SameLine(label_width);
    ImGui::PushItemWidth(input_width);
    if (ImGui::InputInt("##MandelIters", &m_sharedState.mandelbrot_params.max_iterations, 10, 100)) mandelbrot_changed = true;
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::SliderInt("##MandelItersSlider", &m_sharedState.mandelbrot_params.max_iterations, 1, 3000)) mandelbrot_changed = true;
    ImGui::PopItemWidth();

    ImGui::Text("Zoom");
    ImGui::SameLine(label_width);
    ImGui::PushItemWidth(-1);
    ImGui::InputDouble("##Zoom", &m_sharedState.mandelbrot_params.zoom, 0.0, 0.0, "%.2fx", ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();

    ImGui::Separator();
    ImGui::Text("Color Scheme");
    bool color_changed = false;

    if (ImGui::ColorEdit3("Background", m_sharedState.color_scheme.background)) {
        color_changed = true;
    }

    ImGui::Text("Pattern Color Coefficients (R, G, B)");
    if (ImGui::SliderFloat3("Red Channel", m_sharedState.color_scheme.pattern_r, 0.0f, 20.0f)) {
        color_changed = true;
    }
    if (ImGui::SliderFloat3("Green Channel", m_sharedState.color_scheme.pattern_g, 0.0f, 20.0f)) {
        color_changed = true;
    }
    if (ImGui::SliderFloat3("Blue Channel", m_sharedState.color_scheme.pattern_b, 0.0f, 20.0f)) {
        color_changed = true;
    }

    if (color_changed) {
        m_sharedState.mandelbrot_needs_update = true;
        m_sharedState.julia_needs_update = true;
    }

    if (mandelbrot_changed) {
        trigger_update(m_sharedState, FractalType::MANDELBROT);
    }
    ImGui::Separator();
    ImGui::Checkbox("Show Julia", &m_sharedState.show_julia_window);
    if (m_sharedState.show_julia_window) {
        ImGui::Text("Julia Settings");
        bool julia_changed = false;

        ImGui::Text("Width");
        ImGui::SameLine(label_width);
        ImGui::PushItemWidth(input_width);
        if (ImGui::InputInt("##JuliaWidth", &m_sharedState.julia_params.width, 10, 100)) julia_changed = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##JuliaWidthSlider", &m_sharedState.julia_params.width, 1, 3840)) julia_changed = true;
        ImGui::PopItemWidth();

        ImGui::Text("Height");
        ImGui::SameLine(label_width);
        ImGui::PushItemWidth(input_width);
        if (ImGui::InputInt("##JuliaHeight", &m_sharedState.julia_params.height, 10, 100)) julia_changed = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##JuliaHeightSlider", &m_sharedState.julia_params.height, 1, 2160)) julia_changed = true;
        ImGui::PopItemWidth();

        ImGui::Text("Iters");
        ImGui::SameLine(label_width);
        ImGui::PushItemWidth(input_width);
        if (ImGui::InputInt("##JuliaIters", &m_sharedState.julia_params.max_iterations, 10, 100)) julia_changed = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##JuliaItersSlider", &m_sharedState.julia_params.max_iterations, 1, 3000)) julia_changed = true;
        ImGui::PopItemWidth();

        ImGui::Text("Constant (c)");
        ImGui::Text("Re: %.4f, Im: %.4f", m_sharedState.julia_c.real(), m_sharedState.julia_c.imag());

        if (julia_changed) {
            trigger_update(m_sharedState, FractalType::JULIA);
        }
    }

    ImGui::End();
}
