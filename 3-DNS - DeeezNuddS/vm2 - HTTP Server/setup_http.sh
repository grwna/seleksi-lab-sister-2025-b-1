#!/bin/bash
set -e

# =========================================================
#             SCRIPT KONFIGURASI HTTP SERVER
# =========================================================

DOMAIN_NAME="grwna.sister.acc"
WEB_PORT="8080"
WEB_DIR="/var/www/sister"
FLAG_PORT="8081"
FLAG_DIR="/var/www/flag"

echo "[+] Creating web root directory at $WEB_DIR..."
sudo mkdir -p $WEB_DIR
echo "[+] Creating flag root directory at $FLAG_DIR..."
sudo mkdir -p $FLAG_DIR

echo "[+] Sending index.html to web root..."
sudo cp index.html $WEB_DIR/index.html
echo "[+] Sending flag.html to flag root..."
sudo cp flag.html $FLAG_DIR/flag.html

echo "[+] Setting permissions..."
sudo chown -R http:http /var/www/
echo "[✓] Creating web page successful."

echo "[+] Configuring Nginx for port $WEB_PORT and $FLAG_PORT..."
sudo tee /etc/nginx/nginx.conf > /dev/null <<EOF
user http;
worker_processes  1;

events {
    worker_connections  1024;
}

http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;

    server {
        listen       $WEB_PORT;
        server_name  $DOMAIN_NAME;
        root         $WEB_DIR;
        index        index.html;

        location / {
            try_files \$uri \$uri/ =404;
        }
    }

    server {
        listen       $FLAG_PORT;
        server_name  $DOMAIN_NAME;
        root         $FLAG_DIR;
        index        flag.html;

        location / {
            try_files \$uri \$uri/ =404;
        }
    }
}
EOF
echo "[✓] Nginx configuration finished."

echo "[✓] Activating Nginx..."
sudo systemctl enable nginx.service
sudo systemctl restart nginx.service

echo ""
echo "[✓] HTTP Server configuration finished!"