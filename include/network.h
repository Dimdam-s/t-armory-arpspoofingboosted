// Fichier: include/network.h
#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h> // Pour uint8_t

// Récupère l'adresse MAC de l'interface locale
// Retourne 0 en cas de succès, -1 en cas d'erreur
int get_local_mac(int socket, char *interface_name, uint8_t *mac_buffer);

// Récupère l'adresse IP de l'interface locale (format texte)
// Retourne 0 en cas de succès, -1 en cas d'erreur
int get_local_ip(int socket, char *interface_name, char *ip_buffer);

#endif
