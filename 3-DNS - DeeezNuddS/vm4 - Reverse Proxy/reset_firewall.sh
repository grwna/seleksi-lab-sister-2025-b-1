echo "[+] Clearing firewall rules..."
sudo iptables -F
sudo iptables -X
sudo iptables -Z

echo "[+] Setting default policy (accept all)..."
sudo iptables -P INPUT ACCEPT
sudo iptables -P FORWARD ACCEPT
sudo iptables -P OUTPUT ACCEPT
