#include "../headers/gui/control_window.hpp"

ControlWindow::ControlWindow(SharedState& state, sf::Font& font)
    : m_window(sf::VideoMode(480, 320), "Kontrol", sf::Style::Titlebar | sf::Style::Close),
      m_sharedState(state),
      m_font(font) {
    
    m_window.setPosition({50, 50});

    // --- TextBoxes ---
    m_textBoxes.push_back(std::make_unique<TextBox>(m_font, sf::Vector2f(150, 75), sf::Vector2f(200, 40), std::to_string(m_sharedState.params.width)));
    m_textBoxes.push_back(std::make_unique<TextBox>(m_font, sf::Vector2f(150, 145), sf::Vector2f(200, 40), std::to_string(m_sharedState.params.height)));
    m_textBoxes.push_back(std::make_unique<TextBox>(m_font, sf::Vector2f(150, 215), sf::Vector2f(200, 40), std::to_string(m_sharedState.params.max_iterations)));
    
    // --- Tombol Inkremental ---
    const sf::Vector2f buttonSize(40, 40);
    for (int i = 0; i < 3; ++i) {
        float yPos = 75.0f + (i * 70.0f);
        // Tombol -
        m_buttons.emplace_back(buttonSize);
        m_buttons.back().setPosition(360, yPos);
        m_buttons.back().setFillColor(sf::Color::Black);
        m_buttonLabels.emplace_back("-", m_font, 24);
        m_buttonLabels.back().setPosition(372, yPos + 2);

        // Tombol +
        m_buttons.emplace_back(buttonSize);
        m_buttons.back().setPosition(410, yPos);
        m_buttons.back().setFillColor(sf::Color::Black);
        m_buttonLabels.emplace_back("+", m_font, 24);
        m_buttonLabels.back().setPosition(422, yPos + 2);
    }
    
    updateActiveTextBox(); // Set text box pertama sebagai aktif
}

void ControlWindow::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_sharedState.is_running = false;
        }

        // --- Klik Mouse ---
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
            // Cek klik pada text box
            for (size_t i = 0; i < m_textBoxes.size(); ++i) {
                if (m_textBoxes[i]->getShape().getGlobalBounds().contains(mousePos)) {
                    m_activeTextBoxIndex = i;
                    updateActiveTextBox();
                }
            }
            // Cek klik pada tombol +/-
            for (size_t i = 0; i < m_buttons.size(); ++i) {
                if (m_buttons[i].getGlobalBounds().contains(mousePos)) {
                    int boxIndex = i / 2;

                    int direction;
                    if (boxIndex == 2) { // Index max iter
                        direction = (i % 2 == 0) ? -1 : 1;
                    } else {
                        direction = (i % 2 == 0) ? -10 : 10;
                    }

                    int currentValue = m_textBoxes[boxIndex]->getIntValue();
                    m_textBoxes[boxIndex]->setValue(currentValue + direction);
                    updateParameters();
                }
            }
        }

        // Teruskan event ke text box yang aktif
        if (m_textBoxes[m_activeTextBoxIndex]->handleEvent(event)) {
            updateParameters();
        }
    }
}

void ControlWindow::draw() {
    m_window.clear(sf::Color(40, 40, 45));
    
    // Gambar label secara dinamis
    sf::Text title("Parameter", m_font, 20); title.setPosition(20, 20);
    sf::Text widthLabel("Width:", m_font, 16); widthLabel.setPosition(30, 85);
    sf::Text heightLabel("Height:", m_font, 16); heightLabel.setPosition(30, 155);
    sf::Text iterLabel("Iterations:", m_font, 16); iterLabel.setPosition(30, 225);

    m_window.draw(title);
    m_window.draw(widthLabel);
    m_window.draw(heightLabel);
    m_window.draw(iterLabel);

    for (const auto& box : m_textBoxes) {
        box->draw(m_window);
    }
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        m_window.draw(m_buttons[i]);
        m_window.draw(m_buttonLabels[i]);
    }

    m_window.display();
}

bool ControlWindow::isOpen() const {
    return m_window.isOpen();
}

void ControlWindow::updateParameters() {
    m_sharedState.params.width = m_textBoxes[0]->getIntValue();
    m_sharedState.params.height = m_textBoxes[1]->getIntValue();
    m_sharedState.params.max_iterations = m_textBoxes[2]->getIntValue();
    m_sharedState.needs_update = true;
}

void ControlWindow::updateActiveTextBox() {
    for (size_t i = 0; i < m_textBoxes.size(); ++i) {
        m_textBoxes[i]->setActive(i == m_activeTextBoxIndex);
    }
}
