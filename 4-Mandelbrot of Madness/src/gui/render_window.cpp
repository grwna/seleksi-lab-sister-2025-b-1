#include "../headers/gui/render_window.hpp"


RenderWindow::RenderWindow(SharedState& state)
    : m_window(sf::VideoMode(state.params.width, state.params.height), "Mandelbrot Render", sf::Style::Titlebar | sf::Style::Close),
      m_sharedState(state) {
    
    // Posisikan jendela render di sebelah jendela kontrol
    m_window.setPosition({500, 50}); 

    // Inisialisasi font untuk teks status
    // Ini adalah fallback jika font utama tidak bisa di-pass
    if (!m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        cerr << "RenderWindow: Gagal memuat font." << endl;
    }
}

void RenderWindow::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_sharedState.is_running = false;
        }
    }
}

void RenderWindow::update() {
    // Cek flag dari SharedState
    if (m_sharedState.needs_update) {
        // Resize jendela jika perlu
        if (m_window.getSize().x != m_sharedState.params.width || m_window.getSize().y != m_sharedState.params.height) {
            m_window.setSize({(unsigned int)m_sharedState.params.width, (unsigned int)m_sharedState.params.height});
        }
        
        regenerateTexture();
        
        // Set flag kembali ke false setelah update
        m_sharedState.needs_update = false;
    }
}

void RenderWindow::draw() {
    m_window.clear(sf::Color::Black);
    
    // Gambar sprite jika tekstur sudah ada, jika tidak, gambar teks status
    if (m_texture.getSize().x > 0) {
        m_window.draw(m_sprite);
    }
    
    m_window.display();
}

bool RenderWindow::isOpen() const {
    return m_window.isOpen();
}

void RenderWindow::regenerateTexture() {
    auto start = chrono::high_resolution_clock::now();

    vector<Color> pixels;

    if (m_sharedState.params.width == 0 || m_sharedState.params.height == 0){
        cout << "Width/Heigth must be > 0" << endl;
        if (m_sharedState.params.width == 0 ) m_sharedState.params.width = 1;
        if (m_sharedState.params.height == 0 ) m_sharedState.params.height = 1;
        return;
    }

    if (m_sharedState.params.mode == Mode::SERIAL) {
        pixels = mandelbrotSerial(m_sharedState.params.width, m_sharedState.params.height, m_sharedState.params.max_iterations);
    }
    // (Tambahkan else if untuk mode CPU dan GPU di sini)

    // Buat gambar dari data piksel
    sf::Image image;
    // Pastikan parameter valid sebelum membuat gambar
    if (m_sharedState.params.width > 0 && m_sharedState.params.height > 0) {
        image.create(m_sharedState.params.width, m_sharedState.params.height);
        for (int y = 0; y < m_sharedState.params.height; ++y) {
            for (int x = 0; x < m_sharedState.params.width; ++x) {
                Color c = pixels[y * m_sharedState.params.width + x];
                image.setPixel(x, y, sf::Color(c.r, c.g, c.b));
            }
        }
        
        m_texture.loadFromImage(image);
        m_sprite.setTexture(m_texture, true); // true = reset texture rect
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "(" << m_sharedState.params.width << ", " << m_sharedState.params.height << ", " << m_sharedState.params.max_iterations << ")";
    cout  << " Work done in: " << duration.count() << " seconds" << endl;
}
