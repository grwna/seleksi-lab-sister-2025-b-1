#!/bin/bash
set -e

# =========================================================
# SCRIPT KONFIGURASI OTOMATIS - FIREWALL (IPTABLES) DI VM 4
# =========================================================

HOST_ONLY_IF="enp0s8"
HOST_IP="192.168.56.1"

echo "[+] Clearing firewall rules..."
sudo iptables -F
sudo iptables -X
sudo iptables -Z

echo "[+] Setting default policy (deny all)..."
sudo iptables -P INPUT DROP
sudo iptables -P FORWARD DROP
sudo iptables -P OUTPUT DROP

echo "[+] Mengizinkan trafik dari koneksi yang sudah ada..."
sudo iptables -A INPUT -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT

echo "[+] Allowing localhost traffic..."
sudo iptables -A INPUT -i lo -j ACCEPT

# BONUS 1
echo "[+] Allowing inbound traffic to port 80 (HTTP)..."
sudo iptables -A INPUT -p tcp --dport 80 -j ACCEPT

# BONUS 2
# Block range  192.168.100.128 - 192.168.100.255 
BLOCKED_RANGE="192.168.100.128/25"
echo "[+]  Blocking IP in range $BLOCKED_RANGE..."
sudo iptables -A INPUT -s $BLOCKED_RANGE -j DROP

# BONUS 3
echo "[+] Allowing outbound traffic to port 8080..."
sudo iptables -A OUTPUT -p tcp --dport 8080 -j ACCEPT

echo "[+] Allowing inbound SSH from host machine ($HOST_IP)..."
sudo iptables -A INPUT -i $HOST_ONLY_IF -p tcp -s $HOST_IP --dport 22 -j ACCEPT

echo "[+] Allowing outbound SSH responses to host machine..."
sudo iptables -A OUTPUT -o $HOST_ONLY_IF -p tcp --sport 22 -j ACCEPT

echo ""
echo "[✓] Konfigurasi firewall di VM 4 telah selesai!"