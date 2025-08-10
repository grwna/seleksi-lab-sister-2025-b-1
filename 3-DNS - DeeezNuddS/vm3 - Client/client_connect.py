import requests

DOMAIN_NAME = "grwna.sister.acc"
TARGET_PORT = ""

def set_domain_name():
    global DOMAIN_NAME
    print("--- Set Domain Name ---\n")
    DOMAIN_NAME = input("Enter domain name: ")
    if not DOMAIN_NAME:
        print("[!] Domain name cannot be empty!")
        return

def set_target_port():
    global TARGET_PORT
    print("--- Set Target Port ---\n")
    TARGET_PORT = input("Enter target port: ")
    

def connect_to_server():
    print("--- Connect to Web Server ---\n")
    print(f"\n[*] Connecting to {DOMAIN_NAME}...")
    
    try:
        header = {'X-Target-Port': TARGET_PORT}

        
        res = requests.get("http://"+DOMAIN_NAME, headers=header,timeout=5)
        res.raise_for_status()
        
        print("\n--- [ RESULT ] ---")
        print(res.text)
        print("   --- [  EOF ] ---\n")

    except requests.exceptions.RequestException as e:
        print(f"\n[!] Error accessing server: {e}")
    except Exception as e:
        print(f"\n[!] Unexpected error: {e}")

def main():
    """Fungsi utama program client."""
    while True:
        print()
        print()
        print("==========================================")
        print("     CONNECT TO WEB SERVER - VM 3")
        print("==========================================")
        print("Choose an option:")
        print("  1. Set domain name")
        print("  2. Set target port")
        print("  3. Connect to web server")
        print("  4. Keluar")
        print("------------------------------------------")
        print("Domain Name: ", DOMAIN_NAME)
        print("Target Port: ", TARGET_PORT if TARGET_PORT else "Let target decide which port to connect to")



        choice = input("Masukkan pilihan Anda (1/2/3/4): ")
        
        if choice == '1':
            set_domain_name()
        elif choice == '2':
            set_target_port()
        elif choice == '3':
            connect_to_server()
        elif choice == '4':
            print("Exiting...")
            break
        else:
            print("[!] Invalid choice!, try again.")
            input("Press Enter to continue...")

if __name__ == "__main__":
    main()