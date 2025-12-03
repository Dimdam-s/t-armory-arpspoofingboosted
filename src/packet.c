// Fichier: src/packet.c
#include <string.h>
#include <arpa/inet.h> // Pour htons, inet_pton
#include <stdio.h>

#include "../include/protocols.h"

// Constantes standards (définies normalement dans net/ethernet.h)
#define ETH_P_ARP 0x0806
#define ARPHRD_ETHER 1
#define ETH_P_IP 0x0800
#define ARP_REQUEST 1
#define ARP_REPLY 2

/**
 * Construit un paquet ARP complet dans le buffer fourni.
 * * @param buffer      : Le tampon mémoire où écrire le paquet
 * @param source_mac  : MAC réelle de l'envoyeur (NOUS)
 * @param source_ip   : IP (potentiellement falsifiée) de l'envoyeur
 * @param dest_mac    : MAC de la cible (ou FF:FF:FF:FF:FF:FF pour broadcast)
 * @param dest_ip     : IP de la cible
 * @param opcode      : ARP_REQUEST (1) ou ARP_REPLY (2)
 */
void forge_arp_packet(unsigned char *buffer, 
                      uint8_t *source_mac, char *source_ip, 
                      uint8_t *dest_mac,   char *dest_ip, 
                      uint16_t opcode) {
    
    // 1. Pointeurs vers les structures à l'intérieur du buffer
    struct ethernet_header *eth = (struct ethernet_header *)buffer;
    
    // L'en-tête ARP commence juste après l'en-tête Ethernet (14 octets plus loin)
    struct arp_header *arp = (struct arp_header *)(buffer + sizeof(struct ethernet_header));

    // ---------------------------------------------------------
    // Remplissage de l'Ethernet Header (Couche 2)
    // ---------------------------------------------------------
    memcpy(eth->src_mac, source_mac, 6); // De qui ça vient (Moi)
    memcpy(eth->dest_mac, dest_mac, 6);  // Pour qui (Cible)
    eth->eth_type = htons(ETH_P_ARP);    // Ce paquet contient de l'ARP

    // ---------------------------------------------------------
    // Remplissage de l'ARP Header (Couche 2.5)
    // ---------------------------------------------------------
    arp->hw_type = htons(ARPHRD_ETHER);  // Matériel Ethernet
    arp->proto_type = htons(ETH_P_IP);   // Protocole IPv4
    arp->hw_len = 6;                     // MAC fait 6 octets
    arp->proto_len = 4;                  // IP fait 4 octets
    arp->opcode = htons(opcode);         // Request ou Reply

    // Sender Info (Dans une attaque, c'est ici qu'on ment !)
    // "Je suis [Sender IP] et ma MAC est [Sender MAC]"
    memcpy(arp->sender_mac, source_mac, 6);
    
    // Conversion IP String ("192.168.1.1") vers Binaire (32 bits)
    if (inet_pton(AF_INET, source_ip, &arp->sender_ip) != 1) {
        perror("Erreur conversion Source IP");
    }

    // Target Info
    // "Je cherche (ou je réponds à) [Target IP] qui a la MAC [Target MAC]"
    memcpy(arp->target_mac, dest_mac, 6);
    
    if (inet_pton(AF_INET, dest_ip, &arp->target_ip) != 1) {
        perror("Erreur conversion Destination IP");
    }
}
