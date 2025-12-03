#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h> // Pour ETH_P_ALL
#include <signal.h>

// Variable globale pour gérer l'arrêt propre (Ctrl+C)
volatile int keep_running = 1;

// Gestionnaire de signal pour arrêter la boucle proprement
void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\nArrêt du programme détecté. Nettoyage...\n");
        keep_running = 0;
    }
}

int main(int argc, char *argv[]) {
    // 1. Vérification des arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface_reseau>\n", argv[0]);
        fprintf(stderr, "Exemple: sudo ./arpspoof eth0\n");
        return EXIT_FAILURE;
    }

    char *interface_name = argv[1];
    int raw_socket;

    // 2. Gestion des signaux (pour quitter proprement avec Ctrl+C)
    signal(SIGINT, handle_signal);

    printf("[*] Démarrage de The Armory: ARPSpoofing Tool\n");

    // 3. Création du Raw Socket
    // AF_PACKET : On travaille au niveau paquet (couche 2)
    // SOCK_RAW   : On veut tout gérer nous-mêmes
    // htons(ETH_P_ALL) : On veut capturer/envoyer tous les protocoles Ethernet
    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_socket < 0) {
        perror("[-] Erreur lors de l'ouverture du socket (Etes-vous root ?)");
        return EXIT_FAILURE;
    }
    printf("[+] Raw Socket ouvert avec succès sur le descripteur %d\n", raw_socket);

    // 4. Récupération de l'index de l'interface (ex: eth0 -> 2)
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

    // 5. Boucle principale d'injection
    printf("[*] Début de la boucle d'injection (Appuyez sur Ctrl+C pour arrêter)...\n");
    
    // Buffer pour contenir le paquet (Taille max Ethernet standard)
    unsigned char buffer[1500]; 

    while (keep_running) {
        // TODO: C'est ici que tu devras appeler tes fonctions de création de paquet
        // Exemple conceptuel :
        // create_arp_packet(buffer, ...);
        // send_packet(raw_socket, buffer, if_index, ...);
        
        printf("[DEBUG] Boucle active... (Simulation d'envoi)\n");
        
        // Pause pour ne pas saturer le CPU et le réseau (flood)
        sleep(2);
    }

    // 6. Fermeture propre
    printf("[*] Fermeture du socket.\n");
    close(raw_socket);
    printf("[*] Fin du programme.\n");

    return EXIT_SUCCESS;
}
