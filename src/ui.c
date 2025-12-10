// Fichier: src/ui.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "../include/ui.h"

void clear_screen(void) {
    printf("\033[2J\033[H");
}

void display_logo(void) {
    printf(BRIGHT_CYAN);
    printf("\n");
    printf("  ████████╗██╗  ██╗███████╗     █████╗ ██████╗ ███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗\n");
    printf("  ╚══██╔══╝██║  ██║██╔════╝    ██╔══██╗██╔══██╗████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝\n");
    printf("     ██║   ███████║█████╗      ███████║██████╔╝██╔████╔██║██║   ██║██████╔╝ ╚████╔╝ \n");
    printf("     ██║   ██╔══██║██╔══╝      ██╔══██║██╔══██╗██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝  \n");
    printf("     ██║   ██║  ██║███████╗    ██║  ██║██║  ██║██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║   \n");
    printf("     ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   \n");
    printf(RESET);
    
    printf(BRIGHT_RED);
    printf("                      ╔═══════════════════════════════════════╗\n");
    printf("                      ║   " BOLD "ARP SPOOFING ATTACK FRAMEWORK" BRIGHT_RED "   ║\n");
    printf("                      ╚═══════════════════════════════════════╝\n");
    printf(RESET);
    
    printf(DIM YELLOW);
    printf("                              [ Network Penetration Tool ]\n");
    printf("                              [ Version: 2.0 - Boosted ]\n");
    printf(RESET);
    printf("\n");
}

void print_separator(void) {
    printf(CYAN "═══════════════════════════════════════════════════════════════════════════════\n" RESET);
}

void print_status(const char *type, const char *message) {
    if (strcmp(type, "success") == 0) {
        printf(BRIGHT_GREEN "[✓] " RESET "%s\n", message);
    } else if (strcmp(type, "info") == 0) {
        printf(BRIGHT_CYAN "[*] " RESET "%s\n", message);
    } else if (strcmp(type, "warning") == 0) {
        printf(BRIGHT_YELLOW "[!] " RESET "%s\n", message);
    } else if (strcmp(type, "error") == 0) {
        printf(BRIGHT_RED "[✗] " RESET "%s\n", message);
    } else if (strcmp(type, "attack") == 0) {
        printf(BRIGHT_RED "[⚔] " RESET "%s\n", message);
    }
}

void display_menu(void) {
    print_separator();
    printf(BOLD CYAN "                              MAIN MENU\n" RESET);
    print_separator();
    printf("\n");
    printf(BRIGHT_GREEN "  [1]" RESET " Manual Attack Configuration\n");
    printf(BRIGHT_GREEN "  [2]" RESET " Automatic Mode (Network Scan)\n");
    printf(BRIGHT_GREEN "  [3]" RESET " Show Network Interfaces\n");
    printf(BRIGHT_GREEN "  [4]" RESET " About / Help\n");
    printf(BRIGHT_RED "  [0]" RESET " Exit\n");
    printf("\n");
    print_separator();
    printf(BOLD "Your choice: " RESET);
}

void display_startup_animation(void) {
    const char *frames[] = {
        "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"
    };
    
    printf(BRIGHT_CYAN);
    printf("\n[");
    for (int i = 0; i < 30; i++) {
        printf("%s", frames[i % 10]);
        fflush(stdout);
        usleep(80000);
        printf("\b");
    }
    printf("✓]\n" RESET);
}

void display_config(const char *interface, const char *target_ip, const char *spoof_ip, const uint8_t *mac) {
    printf("\n");
    print_separator();
    printf(BOLD MAGENTA "                        ATTACK CONFIGURATION\n" RESET);
    print_separator();
    printf("\n");
    printf(CYAN "  Network Interface : " RESET BOLD "%s\n" RESET, interface);
    printf(CYAN "  MAC Address       : " RESET BOLD "%02x:%02x:%02x:%02x:%02x:%02x\n" RESET,
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf(CYAN "  Target (Victim)   : " RESET BOLD "%s\n" RESET, target_ip);
    printf(CYAN "  Spoof IP          : " RESET BOLD "%s\n" RESET, spoof_ip);
    printf("\n");
    print_separator();
    printf("\n");
}

int get_user_config(char *interface, char *target_ip, char *spoof_ip) {
    clear_screen();
    display_logo();
    
    print_separator();
    printf(BOLD YELLOW "                    MANUAL ATTACK CONFIGURATION\n" RESET);
    print_separator();
    printf("\n");
    
    printf(CYAN "➤ Network Interface" RESET " (ex: eth0, wlan0): ");
    if (scanf("%49s", interface) != 1) {
        return -1;
    }
    
    printf(CYAN "➤ Target IP" RESET " (victim): ");
    if (scanf("%49s", target_ip) != 1) {
        return -1;
    }
    
    printf(CYAN "➤ Spoof IP" RESET " (ex: gateway): ");
    if (scanf("%49s", spoof_ip) != 1) {
        return -1;
    }
    
    printf("\n");
    print_status("info", "Configuration saved!");
    
    printf("\n" BOLD YELLOW "Press Enter to launch the attack..." RESET);
    getchar(); // Consommer le \n restant
    getchar(); // Attendre l'utilisateur
    
    return 0;
}
