// File: src/packet.c
#include <string.h>
#include <arpa/inet.h> // For htons, inet_pton
#include <stdio.h>

#include "../include/protocols.h"

// Standard constants (normally defined in net/ethernet.h)
#define ETH_P_ARP 0x0806
#define ETH_P_IP 0x0800

/**
 * Constructs a complete ARP packet in the provided buffer.
 * * @param buffer      : The memory buffer where the packet is written
 * @param source_mac  : Real MAC of the sender (US)
 * @param source_ip   : IP (potentially spoofed) of the sender
 * @param dest_mac    : MAC of the target (or FF:FF:FF:FF:FF:FF for broadcast)
 * @param dest_ip     : IP of the target
 * @param opcode      : ARP_REQUEST (1) or ARP_REPLY (2)
 */
void forge_arp_packet(unsigned char *buffer, 
                      uint8_t *source_mac, char *source_ip, 
                      uint8_t *dest_mac,   char *dest_ip, 
                      uint16_t opcode) {
    
    // 1. Pointers to structures inside the buffer
    struct ethernet_header *eth = (struct ethernet_header *)buffer;
    
    // The ARP header starts just after the Ethernet header (14 bytes later)
    struct arp_header *arp = (struct arp_header *)(buffer + sizeof(struct ethernet_header));

    // ---------------------------------------------------------
    // Filling the Ethernet Header (Layer 2)
    // ---------------------------------------------------------
    memcpy(eth->src_mac, source_mac, 6); // From whom (Me)
    memcpy(eth->dest_mac, dest_mac, 6);  // For whom (Target)
    eth->eth_type = htons(ETH_P_ARP);    // This packet contains ARP

    // ---------------------------------------------------------
    // Filling the ARP Header (Layer 2.5)
    // ---------------------------------------------------------
    arp->hw_type = htons(ARPHRD_ETHER);  // Ethernet Hardware
    arp->proto_type = htons(ETH_P_IP);   // IPv4 Protocol
    arp->hw_len = 6;                     // MAC is 6 bytes
    arp->proto_len = 4;                  // IP is 4 bytes
    arp->opcode = htons(opcode);         // Request or Reply

    // Sender Info (In an attack, this is where we lie!)
    // "I am [Sender IP] and my MAC is [Sender MAC]"
    memcpy(arp->sender_mac, source_mac, 6);
    
    // Convert IP String ("192.168.1.1") to Binary (32 bits)
    if (inet_pton(AF_INET, source_ip, &arp->sender_ip) != 1) {
        perror("Source IP conversion error");
    }

    // Target Info
    // "I am looking for (or replying to) [Target IP] who has MAC [Target MAC]"
    if (opcode == ARP_REQUEST) {
        memset(arp->target_mac, 0, 6); // Target MAC is unknown in a Request
    } else {
        memcpy(arp->target_mac, dest_mac, 6);
    }
    
    if (inet_pton(AF_INET, dest_ip, &arp->target_ip) != 1) {
        perror("Destination IP conversion error");
    }
}
