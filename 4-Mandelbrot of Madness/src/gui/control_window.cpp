#include "../headers/gui/control_window.hpp"

ControlWindow::ControlWindow(SharedState& state, sf::Font& font)
    : m_window(sf::VideoMode(600, 400), "Control", sf::Style::Titlebar | sf::Style::Close),
      m_sharedState(state),
      m_font(font) {
    
     m_window.setPosition({50, 50});

    // --- (BARU) Dropdown untuk Mode ---
    vector<string> modes = {"Serial", "CPU", "GPU"};
    m_modeDropdown = make_unique<Dropdown>(m_font, sf::Vector2f(150, 75), sf::Vector2f(200, 40), modes);

    // --- (DIUBAH) Posisi TextBoxes disesuaikan ---
    m_textBoxes.push_back(make_unique<TextBox>(m_font, sf::Vector2f(150, 145), sf::Vector2f(200, 40), to_string(m_sharedState.params.width)));
    m_textBoxes.push_back(make_unique<TextBox>(m_font, sf::Vector2f(150, 215), sf::Vector2f(200, 40), to_string(m_sharedState.params.height)));
    m_textBoxes.push_back(make_unique<TextBox>(m_font, sf::Vector2f(150, 285), sf::Vector2f(200, 40), to_string(m_sharedState.params.max_iterations)));
    
    m_stepBoxes.push_back(std::make_unique<TextBox>(m_font, sf::Vector2f(460, 145), sf::Vector2f(80, 40), "10"));
    m_stepBoxes.push_back(std::make_unique<TextBox>(m_font, sf::Vector2f(460, 215), sf::Vector2f(80, 40), "10"));
    m_stepBoxes.push_back(std::make_unique<TextBox>(m_font, sf::Vector2f(460, 285), sf::Vector2f(80, 40), "1"));

    // --- (DIUBAH) Posisi Tombol Inkremental disesuaikan ---
    const sf::Vector2f buttonSize(40, 40);
    for (int i = 0; i < 3; ++i) {
        float yPos = 145.0f + (i * 70.0f);
        m_buttons.emplace_back(buttonSize);
        m_buttons.back().setPosition(360, yPos);
        m_buttons.back().setFillColor(sf::Color::Black);
        m_buttonLabels.emplace_back("-", m_font, 24);
        m_buttonLabels.back().setPosition(372, yPos + 2);

        m_buttons.emplace_back(buttonSize);
        m_buttons.back().setPosition(410, yPos);
        m_buttons.back().setFillColor(sf::Color::Black);
        m_buttonLabels.emplace_back("+", m_font, 24);
        m_buttonLabels.back().setPosition(422, yPos + 2);
    }
    
    updateActiveTextBox();
}

void ControlWindow::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_sharedState.is_running = false;
            return;
        }

        // 1. Perubahan pada dropdown SELALU memicu render ulang.
        if (m_modeDropdown->handleEvent(event, m_window)) {
            updateParameters();
        }

        // 2. Perubahan pada kotak langkah (step boxes) TIDAK memicu render ulang.
        // Kita hanya meneruskan event agar teksnya bisa diedit.
        for (auto& box : m_stepBoxes) {
            box->handleEvent(event);
        }

        // 3. Perubahan pada kotak parameter utama (width, height, etc) SELALU memicu render ulang.
        // Cek hanya pada kotak teks yang aktif.
        if (m_activeTextBoxIndex < m_textBoxes.size()) {
            if (m_textBoxes[m_activeTextBoxIndex]->handleEvent(event)) {
                updateParameters();
            }
        }
        
        // 4. Logika untuk klik mouse
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));

            // Atur status aktif untuk semua kotak teks
            m_activeTextBoxIndex = 99; // Nonaktifkan semua kotak utama
            for (auto& box : m_stepBoxes) {
                box->setActive(box->getShape().getGlobalBounds().contains(mousePos));
            }
            for (size_t i = 0; i < m_textBoxes.size(); ++i) {
                if (m_textBoxes[i]->getShape().getGlobalBounds().contains(mousePos)) {
                    m_activeTextBoxIndex = i; // Aktifkan kotak utama yang diklik
                    // Nonaktifkan semua kotak langkah
                    for (auto& stepBox : m_stepBoxes) {
                        stepBox->setActive(false);
                    }
                }
            }
            updateActiveTextBox();

            // Cek klik pada tombol +/- (yang SELALU memicu render ulang)
            for (size_t i = 0; i < m_buttons.size(); ++i) {
                if (m_buttons[i].getGlobalBounds().contains(mousePos)) {
                    int boxIndex = i / 2;
                    int step = m_stepBoxes[boxIndex]->getIntValue();
                    int direction = (i % 2 == 0) ? -step : step;
                    
                    int currentValue = m_textBoxes[boxIndex]->getIntValue();
                    m_textBoxes[boxIndex]->setValue(currentValue + direction);
                    updateParameters();
                }
            }
        }
    }
}

void ControlWindow::draw() {
    m_window.clear(sf::Color(40, 40, 45));
    
    // Gambar label secara dinamis
    sf::Text title("Parameter", m_font, 24); title.setPosition(20, 20);
    sf::Text modeLabel("Mode:", m_font, 16); modeLabel.setPosition(30, 85);
    sf::Text widthLabel("Width:", m_font, 16); widthLabel.setPosition(30, 155);
    sf::Text heightLabel("Height:", m_font, 16); heightLabel.setPosition(30, 225);
    sf::Text iterLabel("Iterations:", m_font, 16); iterLabel.setPosition(30, 300);

    m_window.draw(title);
    m_window.draw(modeLabel);
    m_window.draw(widthLabel);
    m_window.draw(heightLabel);
    m_window.draw(iterLabel);

    for (const auto& box : m_textBoxes) {
        box->draw(m_window);
    }

    for (const auto& box : m_stepBoxes) {
        box->draw(m_window);
    }

    for (size_t i = 0; i < m_buttons.size(); ++i) {
        m_window.draw(m_buttons[i]);
        m_window.draw(m_buttonLabels[i]);
    }

    m_modeDropdown->draw(m_window);
    m_window.display();
}

bool ControlWindow::isOpen() const {
    return m_window.isOpen();
}

void ControlWindow::updateParameters() {
    int selectedModeIndex = m_modeDropdown->getSelectedOptionIndex();
    switch(selectedModeIndex) {
        case 0: m_sharedState.params.mode = Mode::SERIAL; break;
        case 1: m_sharedState.params.mode = Mode::CPU; break;
        case 2: m_sharedState.params.mode = Mode::GPU; break;
    }

    
    int width = m_textBoxes[0]->getIntValue();
    int height = m_textBoxes[1]->getIntValue();
    int iterations = m_textBoxes[2]->getIntValue();

    if (width < 1) width = 1;
    if (height < 1) height = 1;
    
    m_sharedState.params.width = width;
    m_sharedState.params.height = height;
    m_sharedState.params.max_iterations = iterations;

    m_sharedState.needs_update = true;
}

void ControlWindow::updateActiveTextBox() {
    for (size_t i = 0; i < m_textBoxes.size(); ++i) {
        m_textBoxes[i]->setActive(i == m_activeTextBoxIndex);
    }
}
