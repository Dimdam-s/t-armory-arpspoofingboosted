// Fichier: src/network.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>       // Pour struct ifreq
#include <unistd.h>
#include <arpa/inet.h>    // Pour inet_ntoa
#include <sys/socket.h>

#include "../include/network.h"

/**
 * Récupère l'adresse MAC de l'interface donnée.
 * * @param socket : Le descripteur de fichier du socket ouvert (même un socket dummy suffit)
 * @param interface_name : Le nom de l'interface (ex: "eth0")
 * @param mac_buffer : Un tableau d'au moins 6 octets pour stocker le résultat
 */
int get_local_mac(int sock, char *interface_name, uint8_t *mac_buffer) {
    struct ifreq ifr;

    // On prépare la structure ifreq avec le nom de l'interface
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    // SIOCGIFHWADDR = SIOC Get Interface Hardware Address
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("[-] Erreur lors de la récupération de la MAC");
        return -1;
    }

    // Copie des 6 octets de l'adresse matérielle dans notre buffer
    memcpy(mac_buffer, ifr.ifr_hwaddr.sa_data, 6);
    
    return 0;
}

/**
 * Récupère l'adresse IP de l'interface donnée.
 * * @param socket : Le descripteur de fichier du socket
 * @param interface_name : Le nom de l'interface
 * @param ip_buffer : Un buffer char (min 16 octets) pour stocker l'IP en string
 */
int get_local_ip(int sock, char *interface_name, char *ip_buffer) {
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    // SIOCGIFADDR = SIOC Get Interface Address
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        perror("[-] Erreur lors de la récupération de l'IP");
        return -1;
    }

    // Conversion de la structure sockaddr en string lisible (ex: "192.168.1.5")
    struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(ip_buffer, inet_ntoa(ipaddr->sin_addr));

    return 0;
}
