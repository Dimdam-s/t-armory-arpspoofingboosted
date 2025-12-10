// Fichier: include/ui.h
#ifndef UI_H
#define UI_H

// ANSI Color Codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BRIGHT_RED  "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_CYAN "\033[96m"

// Displays the ASCII logo
void display_logo(void);

// Displays the main menu
void display_menu(void);

// Clears the screen
void clear_screen(void);

// Displays a stylized status message
void print_status(const char *type, const char *message);

// Displays a separator
void print_separator(void);

// Displays configuration information
void display_config(const char *interface, const char *target_ip, const char *spoof_ip, const uint8_t *mac);

// Displays startup animation
void display_startup_animation(void);

// Get user input for configuration
int get_user_config(char *interface, char *target_ip, char *spoof_ip);

#endif
