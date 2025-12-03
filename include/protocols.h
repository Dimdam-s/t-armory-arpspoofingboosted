// Fichier: include/protocols.h
#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <stdint.h>

// --- En-tête Ethernet (14 octets) ---
struct ethernet_header {
    uint8_t dest_mac[6];    // Adresse MAC de destination
    uint8_t src_mac[6];     // Adresse MAC source
    uint16_t eth_type;      // Type de protocole (ex: 0x0806 pour ARP)
} __attribute__((packed));

// --- En-tête ARP (28 octets pour IPv4) ---
struct arp_header {
    uint16_t hw_type;       // Type matériel (1 pour Ethernet)
    uint16_t proto_type;    // Type protocole (0x0800 pour IPv4)
    uint8_t hw_len;         // Longueur adresse MAC (6)
    uint8_t proto_len;      // Longueur adresse IP (4)
    uint16_t opcode;        // Opération (1=Request, 2=Reply)
    
    uint8_t sender_mac[6];  // MAC de l'émetteur
    uint32_t sender_ip;     // IP de l'émetteur
    
    uint8_t target_mac[6];  // MAC de la cible
    uint32_t target_ip;     // IP de la cible
} __attribute__((packed));

// Prototype de la fonction de forge
void forge_arp_packet(unsigned char *buffer, 
                      uint8_t *source_mac, char *source_ip, 
                      uint8_t *dest_mac,   char *dest_ip, 
                      uint16_t opcode);

#endif
