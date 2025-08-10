#!/bin/bash
set -e

# ============================================
#       SCRIPT KONFIGURASI DNS SERVER
# ============================================

DOMAIN_NAME="grwna.sister.acc"
IP_DNS="192.168.100.10"
IP_REVERSE_PROXY="192.168.100.40"

echo "[+] Downloading required files..."
sudo curl -o /var/named/named.ca https://www.internic.net/domain/named.root
sudo mkdir /var/named/data
sudo chown named:named /var/named/data

echo "[+] Creating BIND config file /etc/named.conf..."
sudo tee /etc/named.conf > /dev/null <<EOF
options {
    listen-on port 53 { 127.0.0.1; $IP_DNS; };
    directory       "/var/named";
    dump-file       "/var/named/data/cache_dump.db";
    statistics-file "/var/named/data/named_stats.txt";
    memstatistics-file "/var/named/data/named_mem_stats.txt";
    allow-query     { localhost; 192.168.100.0/24; };
    recursion yes;
};
logging {
        channel default_debug {
                file "data/named.run";
                severity dynamic;
        };
};
zone "." IN { type hint; file "named.ca"; };

zone "$DOMAIN_NAME" IN {
    type master;
    file "db.$DOMAIN_NAME";
};
EOF

echo "[+] Creating domain zone file for $DOMAIN_NAME..."
SERIAL_NUM=$(date +%Y%m%d01)
sudo tee /var/named/db.$DOMAIN_NAME > /dev/null <<EOF
\$TTL 86400
@   IN  SOA     ns1.$DOMAIN_NAME. root.$DOMAIN_NAME. (
        $SERIAL_NUM  ; Serial
        3600        ; Refresh
        1800        ; Retry
        604800      ; Expire
        86400 )     ; Minimum TTL
;
@           IN      NS      ns1.$DOMAIN_NAME.
ns1         IN      A       $IP_DNS
@           IN      A       $IP_REVERSE_PROXY
www         IN      A       $IP_REVERSE_PROXY
EOF

echo "[✓] DNS configuration finished."

echo "[*] Activating DNS service"
sudo systemctl enable named.service
sudo systemctl restart named.service

echo ""
echo "[✓] DNS Server configuration finished!"
