// File: include/protocols.h
#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <stdint.h>

#define ARPHRD_ETHER 1
#define ARP_REQUEST 1
#define ARP_REPLY 2

// --- Ethernet Header (14 bytes) ---
struct ethernet_header {
    uint8_t dest_mac[6];    // Destination MAC address
    uint8_t src_mac[6];     // Source MAC address
    uint16_t eth_type;      // Protocol type (e.g., 0x0806 for ARP)
} __attribute__((packed));

// --- ARP Header (28 bytes for IPv4) ---
struct arp_header {
    uint16_t hw_type;       // Hardware type (1 for Ethernet)
    uint16_t proto_type;    // Protocol type (0x0800 for IPv4)
    uint8_t hw_len;         // MAC address length (6)
    uint8_t proto_len;      // IP address length (4)
    uint16_t opcode;        // Operation (1=Request, 2=Reply)
    
    uint8_t sender_mac[6];  // Sender MAC
    uint32_t sender_ip;     // Sender IP
    
    uint8_t target_mac[6];  // Target MAC
    uint32_t target_ip;     // Target IP
} __attribute__((packed));

// Forge function prototype
void forge_arp_packet(unsigned char *buffer, 
                      uint8_t *source_mac, char *source_ip, 
                      uint8_t *dest_mac,   char *dest_ip, 
                      uint16_t opcode);

#endif
