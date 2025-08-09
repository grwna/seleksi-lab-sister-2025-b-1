import os
import sys

IF = "enp0s3"
DNS_ADDR = "192.168.100.10" # VM 1

def run_command(perintah):
    print(f"[*] Running command: {perintah}")
    if os.system(perintah) != 0:
        print(f"[!] Command failed: {perintah}. Exiting...")
        sys.exit(1)

def set_ip_dhcp():
    print("\n[+] Settting up DHCP...")

    run_command(f"sudo nmcli con mod {IF} ipv4.method auto")
    run_command(f"sudo nmcli con mod {IF} ipv4.dns ''")
    run_command(f"sudo nmcli con up {IF}")

    print("\n[✓] DHCP configuration finished. Check IP with 'ip a'.")

def set_ip_manual():
    print("\n[+] Starting manual IP configuration...")
    
    ip_client = input(" > Enter IP for current device (eg. 192.168.100.30): ")
    prefix_client = input(" > Enter subnet prefix (eg. 24): ")
    print()

    ip_dns =  input(" > Enter IP for dns server (eg. 192.168.100.10): ")
    print()

    
    if not ip_client or not prefix_client:
        print("[!] Client's IP and prefix cannot be emtpy!.")
        return
    
    if not ip_dns:
        print("[!] DHCP IP is empty. Using default DNS address")
        ip_dns = DNS_ADDR

    full_addr_client = f"{ip_client}/{prefix_client}"
    full_addr_dns = f"{ip_dns}"

    
    print(f"\n[+] Setting IP: {full_addr_client}...")
    
    run_command(f"sudo nmcli con mod {IF} ipv4.addresses {full_addr_client}")
    run_command(f"sudo nmcli con mod {IF} ipv4.method manual")
    run_command(f"sudo nmcli con mod {IF} ipv4.dns {full_addr_dns}")
    run_command(f"sudo nmcli con up {IF}")
    
    print("\n[✓] Manual configuration finished. Check IP with 'ip a'.")
    

def main():
    while True:
        print("\n--- Client Network Configuration ---")
        print("1. Set IP with DHCP")
        print("2. Set IP Manually")
        print("3. Exit")
        
        choice = input("(1/2/3): ")
        
        if choice == '1':
            set_ip_dhcp()
            break
        elif choice == '2':
            set_ip_manual()
            break
        elif choice == '3':
            print("Exiting...")
            break
        else:
            print("[!] Invalid choice. Please try again!")

if __name__ == "__main__":
    main()