## Cara Menggunakan

Note: Pastikan node sudah terinstall di perangkat anda
Jalankan perintah `npm install` untuk menginstall dependensi aplikasi 
Lalu `node app.js`, buka url `http://localhost:3000`

Note: 
Untuk menjalankan lokal perlu membuat file .env pada directory yang sama dengan README ini. <br>

Isi file .env adalah sebagai berikut
- DATABASE_URL - dari layanan database (saya menggunakan Neon)
- GOOGLE_CLIENT_ID - Dari OAuth Google
- GOOGLE_CLIENT_SECRET - Dari OAuth Google
- HUGGINGFACE_API_KEY - Access Token HuggingFace
- JWT_SECRET - random string saja
- BASE_URL - defaultnya `http://localhost:3000`