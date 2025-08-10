import client_network
import client_connect
import os


def cls():
    """Membersihkan layar terminal."""
    os.system('cls' if os.name == 'nt' else 'clear')

def main():
    """Fungsi utama program client."""
    while True:
        cls()
        print()
        print()
        print("==========================================")
        print("         CLIENT PROGRAM - VM 3")
        print("==========================================")
        print("Choose an option:")
        print("  1. Konfigurasi Jaringan (Set IP/DHCP)")
        print("  2. Akses Web Server (via Domain Name)")
        print("  3. Keluar")
        print("------------------------------------------")
        
        choice = input("(1/2/3): ")
        
        if choice == '1':
            cls()
            client_network.main()
        elif choice == '2':
            cls()
            client_connect.main()
        elif choice == '3':
            print("Exiting...")
            exit()
        else:
            print("[!] Invalid choice!, try again.")
            input("Press Enter to continue...")

if __name__=="__main__":
    main()