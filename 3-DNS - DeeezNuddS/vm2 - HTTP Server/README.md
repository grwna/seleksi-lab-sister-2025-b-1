# VM 2 - HTTP Server

VM 2 digunakan sebagai HTTP Server. Terdapat dua *webpage* yang dilayani, yaitu pada port 8080 (`index.html`) sebagai halaman utama, dan pada port 8081 (`flag.html`) untuk mengetes bonus firewall.

## Lokasi *Config Files*
- nginx.conf          -> /etc/nginx/nginx.conf

## Konfigurasi/*Command*

```
    sudo truncate -s 0 /etc/machine-id
    sudo hostnamectl set-hostname http-server

    sudo pacman -S nginx

    # Set IP untuk internal network
    sudo nmcli con add type ethernet ifname enp0s3 con-name enp0s3
    sudo nmcli con mod enp0s3 ipv4.method manual 
    sudo nmcli con mod enp0s3 ipv4.addresses 192.168.100.20/24
    sudo nmcli con up enp0s3

    # Setup HTTP server melalui: setup_http.sh
```

