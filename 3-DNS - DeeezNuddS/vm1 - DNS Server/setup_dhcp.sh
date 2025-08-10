#!/bin/bash
set -e

# ============================================
#       SCRIPT KONFIGURASI DHCP SERVER
# ============================================

subnet="192.168.100.0"
MASK="255.255.255.0"
RANGE_START="192.168.100.100"
RANGE_END="192.168.100.150"

IP_DNS="192.168.100.10" 
DOMAIN_NAME="grwna.sister.acc"

echo "[+] Creating DHCP config file /etc/dhcpd.conf..."
sudo tee /etc/dhcpd.conf > /dev/null <<EOF
# dhcpd.conf - Konfigurasi untuk DHCP Server
authoritative;

subnet $subnet netmask $MASK {
  range $RANGE_START $RANGE_END;
  option domain-name-servers $IP_DNS;
  option domain-name "$DOMAIN_NAME";
  option routers 192.168.100.1;
  default-lease-time 600;
  max-lease-time 7200;
}
EOF
echo "[✓] DHCP configuration finished."

echo "[*] Activating DHCP service..."
sudo systemctl enable dhcpd4.service
sudo systemctl restart dhcpd4.service

echo ""
echo "[✓] DHCP Server configuration finished!"