# VM 3 - Client

VM 3 digunakan sebagai Client, yang akan mengakses *webpage*. Untuk melakukan tes terhadap *firewall*, program client dibuat dapat memilih *target port* yang dituju pada HTTP server. Port ini disimpan di *header* dan akan di translasi oleh *reverse proxy* untuk ditujukan ke HTTP server.

## Konfigurasi/*Command*
```
    sudo truncate -s 0 /etc/machine-id
    sudo hostnamectl set-hostname client

    sudo pacman -S python

    # untuk testing bonus firewall
    sudo pacman -S netcat    
    sudo pacman -S python-requests

    sudo nmcli con add type ethernet ifname enp0s3 con-name enp0s3
    # Set ip manual atau dengan DHCP melalui program 
```
Jalankan program melalui `main.py`.