// File: src/network.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>       // For struct ifreq
#include <unistd.h>
#include <arpa/inet.h>    // For inet_ntoa
#include <sys/socket.h>

#include "../include/network.h"

/**
 * Retrieves the MAC address of the given interface.
 * * @param socket : The file descriptor of the open socket (even a dummy socket is enough)
 * @param interface_name : The name of the interface (e.g., "eth0")
 * @param mac_buffer : An array of at least 6 bytes to store the result
 */
int get_local_mac(int sock, char *interface_name, uint8_t *mac_buffer) {
    struct ifreq ifr;

    // Prepare the ifreq structure with the interface name
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    // SIOCGIFHWADDR = SIOC Get Interface Hardware Address
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("[-] Error retrieving MAC address");
        return -1;
    }

    // Copy the 6 bytes of the hardware address into our buffer
    memcpy(mac_buffer, ifr.ifr_hwaddr.sa_data, 6);
    
    return 0;
}

/**
 * Retrieves the IP address of the given interface.
 * * @param socket : The file descriptor of the socket
 * @param interface_name : The name of the interface
 * @param ip_buffer : A char buffer (min 16 bytes) to store the IP as a string
 */
int get_local_ip(int sock, char *interface_name, char *ip_buffer) {
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    // SIOCGIFADDR = SIOC Get Interface Address
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        perror("[-] Error retrieving IP address");
        return -1;
    }

    // Convert the sockaddr structure to a readable string (e.g., "192.168.1.5")
    struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(ip_buffer, inet_ntoa(ipaddr->sin_addr));

    return 0;
}

/**
 * Retrieves the Netmask of the given interface.
 * * @param socket : The file descriptor of the socket
 * @param interface_name : The name of the interface
 * @param netmask_buffer : A char buffer to store the Netmask as a string
 */
int get_netmask(int sock, char *interface_name, char *netmask_buffer) {
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);

    // SIOCGIFNETMASK = SIOC Get Interface Netmask
    if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
        perror("[-] Error retrieving Netmask");
        return -1;
    }

    struct sockaddr_in *nm = (struct sockaddr_in *)&ifr.ifr_netmask;
    strcpy(netmask_buffer, inet_ntoa(nm->sin_addr));

    return 0;
}
