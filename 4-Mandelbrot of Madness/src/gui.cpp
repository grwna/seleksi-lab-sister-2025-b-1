#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "headers/gui/app_state.hpp"
#include "headers/gui/control_window.hpp"
#include "headers/gui/render_window.hpp"

/**
 * @brief Titik masuk utama untuk aplikasi GUI Mandelbrot.
 *
 * Fungsi main ini bertanggung jawab untuk:
 * 1. Menginisialisasi state bersama (SharedState) yang akan digunakan oleh kedua jendela.
 * 2. Memuat resource global seperti font.
 * 3. Membuat instance dari ControlWindow dan RenderWindow.
 * 4. Menjalankan loop aplikasi utama yang menangani event, update, dan drawing
 * untuk kedua jendela secara terus-menerus sampai aplikasi ditutup.
 */
int main() {
    // --- Inisialisasi Utama ---
    std::cout << "Membuka aplikasi GUI Mandelbrot..." << std::endl;

    // Muat font yang akan dibagikan ke jendela kontrol
    sf::Font sharedFont;
    if (!sharedFont.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf")) {
        std::cerr << "Error: Gagal memuat font utama. Pastikan paket ttf-dejavu terinstal." << std::endl;
        return -1;
    }

    // Buat state bersama yang akan menghubungkan kedua jendela
    SharedState sharedState;

    // Buat kedua jendela
    ControlWindow control(sharedState, sharedFont);
    RenderWindow render(sharedState);

    // --- Loop Aplikasi Utama ---
    // Loop akan terus berjalan selama is_running di state bersama adalah true.
    // Menutup salah satu jendela akan mengubah flag ini menjadi false.
    while (sharedState.is_running) {
        // --- Proses Jendela Kontrol ---
        // Tangani semua event yang terjadi di jendela kontrol (klik, ketik)
        control.handleEvents();
        // Gambar ulang jendela kontrol
        control.draw();

        // --- Proses Jendela Render ---
        // Tangani semua event di jendela render (hanya event 'close' saat ini)
        render.handleEvents();
        // Cek apakah perlu mengupdate gambar (berdasarkan flag needs_update)
        render.update();
        // Gambar ulang jendela render
        render.draw();
    }

    std::cout << "Menutup aplikasi." << std::endl;
    return 0;
}
