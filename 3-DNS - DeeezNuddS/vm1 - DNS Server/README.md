# VM 1 - DNS Server

VM 1 digunakan sebagai DNS Server sekaligus DHCP Server.

## Lokasi *Config Files*
- db.grwna.sister.acc -> /var/named/db.grwna.sister.acc
- dhcpd.conf          -> /etc/dhcpd.conf 
- named.conf          -> /etc/named.conf

## Konfigurasi/*Command*

```
    sudo truncate -s 0 /etc/machine-id
    sudo hostnamectl set-hostname dns-server

    sudo pacman -S bind
    sudo pacman -S dhcp

    # Set IP untuk internal network
    sudo nmcli con add type ethernet ifname enp0s3 con-name enp0s3
    sudo nmcli con mod enp0s3 ipv4.method manual 
    sudo nmcli con mod enp0s3 ipv4.addresses 192.168.100.10/24
    sudo nmcli con up enp0s3

    # Setup server DNS dan DHCP melalui skrip setup_dns.sh dan setup_dhcp.sh
```

