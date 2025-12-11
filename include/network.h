// File: include/network.h
#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h> // For uint8_t

// Retrieves the MAC address of the local interface
// Returns 0 on success, -1 on error
int get_local_mac(int socket, char *interface_name, uint8_t *mac_buffer);

// Retrieves the IP address of the local interface (text format)
// Returns 0 on success, -1 on error
int get_local_ip(int socket, char *interface_name, char *ip_buffer);

// Retrieves the Netmask of the local interface
// Returns 0 on success, -1 on error
int get_netmask(int socket, char *interface_name, char *netmask_buffer);

#endif
