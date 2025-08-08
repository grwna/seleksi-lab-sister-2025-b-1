# Mandelbrot of Madness

## Mandelbrot Set Generator with CPU & GPU Acceleration

<img width="800"  alt="Image" align="center" src="https://private-user-images.githubusercontent.com/163623355/474342421-3cc523b6-fe23-40fd-ad6d-0ab436c16900.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NTQzODYwMzUsIm5iZiI6MTc1NDM4NTczNSwicGF0aCI6Ii8xNjM2MjMzNTUvNDc0MzQyNDIxLTNjYzUyM2I2LWZlMjMtNDBmZC1hZDZkLTBhYjQzNmMxNjkwMC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjUwODA1JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI1MDgwNVQwOTIyMTVaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1hZmQ2ODI5Zjg0YWRiMTRhMmE2MzA2OGZhZDZiMDNjMmI3NTE2ZWU4Nzg2ODk4N2I5YWQ5MTVhZWUyMmQ4NjZiJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.yY5EpX-FDo-Er0O_udnnWmYYwRkc21SUcQZf3dNs06k" />

Proyek ini di buat sebagai salah satu tugas seleksi asisten Laboratorium Sistem Terdistribusi. Program dapat menghasilkan gambar himpunan Mandelbrot dalam bentuk `.bmp` menggunakan  tiga jenis implementasi: serial, paralel CPU dengan OpenMP, dan akselerasi GPU menggunakan CUDA. Program ini ditulis dalam bahasa C++

<br>


## Author
| Nama              | NIM      | Github                            |
| ----------------- | -------- | --------------------------------- |
| M. Rayhan Farrukh | 13523035 | [grwna](https://github.com/grwna) |

<br>


## 📑 Daftar Isi
- [Fitur](#fitur)
- [Deskripsi Implementasi](#deskripsi-implementasi)
    - [Serial](#serial)
    - [CPU](#paralel-cpu-(openmp))
    - [GPU](#akselerasi-gpu-(cuda))
    - [CLI](#cli)
    - [GUI](#gui)
- [Petunjuk Kompilasi & Menjalankan](#petunjuk-kompilasi-&-menjalanka)
    - [*Prerequisite*](#prerequisite)
    - [Langkah Kompilasi](#kompilasi)
    - [Menjalankan Program](#menjalankan-program)
- [Testing](#testing)
    - [Hasil Benchmarking](#hasil-benchmarking)
    - [Contoh Hasil Gambar](#contoh-hasil-gambar)
    - [Video Demo](#video-demo)
    
<br>


## ✨Fitur
- Generator dengan tiga mode (serial, CPU, GPU)
- Dapat dijalankan melalui CLI atau GUI
- 

<br>


## 📝Deskripsi Implementasi

Berikut adalah penjelasan singkat untuk setiap pendekatan yang digunakan dalam proyek ini.
### 🐌Serial

Implementasi *basic* di mana setiap piksel  gambar dihitung secara berurutan satu per satu oleh satu thread CPU. Bagian ini acuan untuk mengukur percepatan yang didapat dari metode paralel.
### 🐇Paralel CPU (OpenMP)

Implementasi serial diparalelkan menggunakan **OpenMP**. Direktif `#pragma omp parallel for` ditambahkan *outer loop* utama. Ini memungkinkan  CPU membagi pekerjaan, di mana setiap thread akan mengerjakan sekumpulan baris piksel yang berbeda pada gambar secara bersamaan. 
### 🐎Akselerasi GPU (CUDA)

Untuk akselerasi GPU, implementasi serial disesuaikan untuk dapat dijalankan pada GPU. Setiap *thread* akan memproses satu *pixel*, ini dilakukan dengan mengubah fungsi pewarnaan dan perhitungan Mandelbrot menjadi *device* dan fungsi utama menjadi *kernel*.  *Kernel* dijalankan dengan 256 *threads* per blok dan  jumlah blok menyesuaikan agar jumlah *thread* cukup untuk  menjalankan semua *pixel* pada gambar.

### 📟CLI

Mode CLI hanya sederhana, dan sebenarnya tidak ada *interface* sama sekali. Mode CLI hanya digunakan untuk memasukkan input dari file.
### 💻GUI

GUI menampilkan satu *window* untuk mengatur parameter *image*,  dan satu *window* lain untuk menampilkan *image* hasil generasi. Parameter yang dapat diatur adalah mode kalkulasi (serial, cpu, gpu), dimensi gambar, serta *max iterations*.  Untuk mode GUI, tidak ada fitur untuk menyimpan gambar, gunakanlah GUI untuk mengecek *preview* dari gambar, kemudian gunakan parameter tersebut pada mode CLI untuk menyimpan gambar. Juga tidak ada multitasking, jadi ketika *image* di-*render*, GUI akan *freeze*.

<br>

## 📖Petunjuk Kompilasi & Menjalankan

>[!note]
>Program hanya diuji pada *environment* **Linux** melalui **Windows Subsystem for Linux 2** (WSL2). Tidak ada jaminan program akan berjalan pada sistem operasi lain.
### *Prerequisite*

Pastikan perangkat Anda telah memenuhi/menginstall prasyarat berikut:
- GNU Make
- Compiler C++ yang mendukung C++17 dan OpenMP.
- Memiliki **Graphics Card Nvidia**
- Nvidia CUDA Toolkit.
- Library SFML (jika menggunakan GUI).

### Kompilasi

1. **Cek Compute Capability GPU Anda**, jalankan *command* berikut.
```
	Nvidia-smi --query-gpu=compute_cap --format=csv,noheader
```

2. **Sesuaikan `Makefile`**. Buka file `Makefile` dan ubah nilai variabel `ARCH` agar sesuai dengan hasil dari perintah di atas. Contoh: jika hasilnya `8.6`, ubah menjadi `ARCH = -arch=sm_86`.

3. **Jalankan `make`**. Buka terminal di direktori utama proyek dan jalankan perintah berikut untuk mengompilasi semua target:
```
	make all
```

Atau, kompilasi target spesifik:
   
   ```
   make cli       # Hanya compile CLI
   make gui       # Hanya compile GUI
   make benchmark # Hanya compile program benchmark
   ```

*Binary* program akan disimpan pada folder `bin` 
### Menjalankan Program

Setelah kompilasi , Anda dapat menjalankan program dari direktori utama menggunakan `make`.
```
make run_cli
make run_gui
make run_benchmark
```

Untuk mode CLI, Anda bisa menghitung waktu berjalannya program menggunakan *command* `time` 
```
time make run_cli
```

Pada mode CLI juga Anda dapat memasukkan *input* dari file melalui *input redirection*. Format dari file adalah sebagai berikut.
```
WIDTH
HEIGHT
MAX_ITER
MODE
OUT_FILENAME
SAVE/NOSAVE
```

Contoh penggunaan:
```
time make run_cli < test/1.txt
```

<br>

## ⚙️Testing
### Hasil Benchmarking

Berikut adalah tabel perbandingan waktu eksekusi dan _speedup_ untuk beberapa kasus

>[!note] 
>Pengujian dilakukan pada CPU **Intel i7-12700H** dan GPU **Nvidia GeForce RTX 3060 Laptop GPU**.

| Implementasi             | Waktu Eksekusi (detik) | Speedup vs Serial  |
| ------------------------ | ---------------------- | ------------------ |
| **Serial**               | `[Isi waktu di sini]`  | 1.00x              |
| **Paralel CPU (OpenMP)** | `[Isi waktu di sini]`  | `[Hitung speedup]` |
| **GPU (CUDA)**           | `[Isi waktu di sini]`  | `[Hitung speedup]` |



### Contoh Hasil Gambar

Berikut adalah contoh gambar yang dihasilkan oleh program dengan resolusi 1200x800, dan iterasi 500.
<img width="800"  alt="Image" align="center" src="https://private-user-images.githubusercontent.com/163623355/474399112-5593ec7d-b6d5-4511-a74d-5b0f9b1c65e1.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NTQzODMxMjgsIm5iZiI6MTc1NDM4MjgyOCwicGF0aCI6Ii8xNjM2MjMzNTUvNDc0Mzk5MTEyLTU1OTNlYzdkLWI2ZDUtNDUxMS1hNzRkLTViMGY5YjFjNjVlMS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjUwODA1JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI1MDgwNVQwODMzNDhaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT0zNTlhYTZhZmY0N2NkMDY2NTQyZTZjMTYyZTNjODc3NjRlOGRhMmNmMmMyMzc2YmM4ZDliNDFjZjhlYzgxOTFjJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.btDN-movim1IdlJHvAXjqeKaZbbnwWvHd6zUW2ZSNfU" />

### Video Demo
- GUI - https://youtu.be/IWVH-m3sh3Y
- Julia Set -
- Zoom -