# VM 4 - Reverse Proxy

VM 4 digunakan sebagai *reverse proxy* ke HTTP server. VM 4 juga dilengkapi dengan *firewall* untuk mengatur *traffic* yang dibolehkan.

## Konfigurasi/*Command*
```
    sudo truncate -s 0 /etc/machine-id
    sudo hostnamectl set-hostname reverse-proxy

    sudo pacman -S python

    # untuk testing firewall
    sudo pacman -S vsftpd
    sudo systemctl enable --now vsftpd

    sudo nmcli con add type ethernet ifname enp0s3 con-name enp0s3
    sudo nmcli con mod enp0s3 ipv4.method manual 
    sudo nmcli con mod enp0s3 ipv4.addresses 192.168.100.40/24
    sudo nmcli con up enp0s3

    # setup firewall melalui setup_firewall.sh
```

## Firewall
Spesifikasi *firewall* adalah sebagai berikut.
- Hanya memperbolehkan http traffic 
- Blokir akses client pada ip range tertentu (ip dibebaskan)
- Hanya memperbolehkan traffic dengan target port 8080

>[!note]
>**ASUMSI**
>Interpretasi saya, hanya memperbolehkan *http traffic* berarti hanya memperbolehkan koneksi ke VM 4 melalui port HTTP (80), sedangkan *target port* merujuk pada ?>port pada HTTP Server (VM 2). Sehingga yang saya lakukan adalah blokir semua, lalu *allow* inbound ke port 80 dan *allow* outbound ke *destination port* 8080.

*Firewall* tidak aktif dengan sendirinya, kita perlu mengaktifkannya dengan skrip `setup_firewall.sh`. Selain itu *firewall* juga tidak disimpan permanen, jika VM dimatikan *rules firewall* akan reset jadi `setup_firewall.sh` harus dijalankan ulang setiap kali menghidupkan VM. Gunakan `reset_firewall.sh` untuk mematikan *rules firewall*.


Untuk melakukan *testing* terhadap *firewall* berikut hal yang dapat dilakukan berdasarkan konfigurasi yang sudah ada <br>

- **Hanya bolehkan HTTP *traffic*** <br>
Port yang berjalan pada VM 4 hanyalah 80 (http), 22 (ssh), dan 21 (ftp). <br>
Coba gunakan `nc -zv -w 10 grwna.sister.acc <port>` atau `nc -zv -w 10 192.168.100.40 <port>` dengan port 21 atau 22 pada VM 1, 2 atau 3, jika hasil menunjukkan `open` maka *firewall* gagal atau tidak aktif, jika melewati 10 detik dan terjadi *timeout* maka *firewall* hidup. <br>
Perlu diperhatikan bahwa *firewall* tidak memblokir *traffic* SSH dari *host machine* agar sambungan SSH tidak terputus.

- **Blokir *client* pada *ip range* tertentu** <br>
IP yang diblokir berada di 192.168.100.128/25, yaitu 192.168.100.128 hingga 192.168.100.255. Ubah IP *client* menjadi salah satu IP pada *range* tersebut lalu coba akses web.

- **Hanya bolehkan *target port* 8080** <br>
Gunakan program pada VM 3 untuk mencoba mengakses web pada *port* 8081
