import socket

LISTENING_HOST = '0.0.0.0'
LISTENING_PORT = 80
TARGET_HOST = '192.168.100.20'
TARGET_PORT = 8080
BUFF_SIZE = 4096

def parse_header(req):
    try:
        headers = req.decode('utf-8').split('\r\n')
        for header in headers:
            if header.lower().startswith('x-target-port:'):
                port = int(header.split(':')[1].strip())
                print(f"[*] 'X-Target-Port' found. directing request to port {port}.")
                return port
    except Exception:
        pass

    print(f"[*] 'X-Target-Port' not found. defaulting to port {TARGET_PORT}.")
    return TARGET_PORT

if __name__ == "__main__":
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((LISTENING_HOST, LISTENING_PORT))
        server_socket.listen(1)
        print(f"[*] Proxy listening on {LISTENING_HOST}:{LISTENING_PORT}...")
    except PermissionError:
        print("Insufficient permission! try running with sudo")
        exit()

    while True:
        client_socket, addr = server_socket.accept()
        print(f"\n[+] Accepting connection from {addr[0]}:{addr[1]}...")
        target_socket = None

        client_socket.settimeout(5)

        try:
            # terima data dari client dan teruskan ke target
            request = client_socket.recv(BUFF_SIZE)
            if request:
                port = parse_header(request)

                print(f"\n[+] Connecting to {TARGET_HOST}:{port}...")
                target_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                target_socket.settimeout(5)
                target_socket.connect((TARGET_HOST, port))
                target_socket.sendall(request)

            # terima dan teruskan data dari target
            response = target_socket.recv(BUFF_SIZE)
            if response:
                client_socket.sendall(response)

        except ConnectionRefusedError as e:
            print(f"Connection refused to {TARGET_HOST}:{port}.")
        except Exception as e:
            print("Unexpected error", e)
                
        finally:
            print(f"[*] Finished, closing connection from {addr[0]}...")
            client_socket.close()
            target_socket.close()
    
