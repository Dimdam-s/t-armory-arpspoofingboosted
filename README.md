# 🗡️ The Armory - ARP Spoofing Boosted

<div align="center">

```
  ████████╗██╗  ██╗███████╗     █████╗ ██████╗ ███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗
  ╚══██╔══╝██║  ██║██╔════╝    ██╔══██╗██╔══██╗████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝
     ██║   ███████║█████╗      ███████║██████╔╝██╔████╔██║██║   ██║██████╔╝ ╚████╔╝ 
     ██║   ██╔══██║██╔══╝      ██╔══██║██╔══██╗██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝  
     ██║   ██║  ██║███████╗    ██║  ██║██║  ██║██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║   
     ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   
```

**A powerful ARP Spoofing tool with an interactive interface**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))

</div>

---

## 📋 Description

**The Armory - ARP Spoofing Boosted** is an advanced demonstration tool for performing ARP Spoofing/Poisoning attacks. It features an interactive command-line interface with ANSI colors and a professional design.

### ✨ Features

- 🎨 **Stylish interactive interface** with ASCII logo and colored menu
- ⚔️ **Real-time ARP packet injection** (forged packets)
- 🔧 **Manual configuration** or command-line mode
- 📊 **Real-time attack statistics** display
- 🛡️ **Clean signal handling** (Ctrl+C)
- 🌐 **Multi-interface support**
- �� **List available network interfaces** on the system

---

## ⚠️ LEGAL WARNING

> **WARNING:** This tool is intended for **educational and security testing purposes only**.
> 
> Using this tool on networks or systems without explicit authorization is **ILLEGAL** and may result in legal action.
> 
> The authors decline any responsibility in case of malicious use of this tool.

---

## 🛠️ Installation

### Prerequisites

- Linux System (tested on Ubuntu/Debian)
- GCC (C compiler)
- Make
- Root/sudo privileges (required for RAW socket access)

### Compilation

```bash
# Clone the repository
git clone https://github.com/your-username/t-armory-arpspoofingboosted.git
cd t-armory-arpspoofingboosted

# Compile with Make
make

# Run (requires root privileges)
sudo ./arpspoof
```

---

## 🚀 Usage

### Interactive Mode (Recommended)

Simply run the program without arguments to access the interactive menu:

```bash
sudo ./arpspoof
```

You will see:
1. A beautiful ASCII logo
2. A main menu with several options
3. Colored and formatted messages

**Menu Options:**
- **[1]** Manual attack configuration
- **[2]** Automatic mode (in development)
- **[3]** Show network interfaces
- **[4]** About / Help
- **[0]** Exit

### Command Line Mode

For quick command-line usage:

```bash
sudo ./arpspoof <interface> <target_ip> <spoof_ip>
```

**Example:**
```bash
sudo ./arpspoof eth0 192.168.1.10 192.168.1.1
```

This will launch the attack directly, making `192.168.1.10` believe that `192.168.1.1` (the gateway) has your machine's MAC address.

---

## 📁 Project Structure

```
.
├── MakeFile              # Compilation script
├── README.md            # This file
├── include/
│   ├── network.h        # Headers for network functions
│   ├── protocols.h      # Protocol definitions (ARP, Ethernet)
│   └── ui.h             # Headers for user interface
└── src/
    ├── main.c           # Main program with interactive menu
    ├── network.c        # Network management functions
    ├── packet.c         # ARP packet construction
    └── ui.c             # User interface (logo, menu, colors)
```

---

## 🎨 Interface Overview

The interface uses ANSI codes to display:
- ✓ Success messages in green
- ⚔ Attack messages in red
- * Information messages in cyan
- ! Warning messages in yellow
- ✗ Error messages in bright red

---

## 🔧 Cleaning

To clean compiled files:

```bash
make clean
```

---

## 📚 Theory: ARP Spoofing

**ARP Spoofing** (or ARP Poisoning) is a technique that exploits the ARP protocol to:
1. Send fake ARP replies on a network
2. Associate the attacker's MAC address with another machine's IP address
3. Intercept traffic destined for that machine (MITM attack)

**How it works:**
```
Normal:    Victim <--> Gateway
Attack:    Victim <--> Attacker <--> Gateway
```

---

## 🤝 Contribution

Contributions are welcome! Feel free to:
- Report bugs
- Propose new features
- Submit pull requests

---

## 📝 License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

---

## 👨‍💻 Author

Created with ❤️ for network security learning.

---

<div align="center">

**⚡ Use this tool responsibly! ⚡**

</div>
