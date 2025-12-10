# Changelog - The Armory: ARP Spoofing Boosted

## Version 2.0 - Full Graphical Interface (December 2024)

### 🎨 New Features

#### User Interface
- ✨ **Stylized ASCII Logo** - Professional "THE ARMORY" banner
- 🎨 **ANSI Color Codes** - Colorful and readable interface
  - Green: Success messages
  - Red: Errors and attacks
  - Cyan: Information
  - Yellow: Warnings
  - Magenta: Section titles
- 📊 **Interactive Menu** - Easy navigation with numbered options
- 🎯 **Visual Separators** - Better display organization

#### Navigation Features
- 🔧 **Manual Configuration Mode** - Guided interface for entering parameters
- 📡 **Network Interface Display** - Automatic list of available interfaces
- ℹ️ **"About" Page** - Integrated documentation
- 🚪 **Exit Menu** - Clean application exit

#### Technical Improvements
- 📈 **Packet Counter** - Real-time statistics of sent packets
- 🎭 **Typed Status Messages** - Consistent format for all messages
- 📋 **Configuration Display** - Summary before launching the attack
- 🔄 **Backward Compatibility** - Support for classic command-line mode

### 📁 New Files

```
include/ui.h          - User interface declarations
src/ui.c              - Interface implementation
README.md             - Updated main documentation
USAGE.md              - Detailed usage guide
LOGO.txt              - ASCII Logo for reference
demo.sh               - Demonstration script
CHANGELOG.md          - This file
```

### 🔧 Modifications

#### src/main.c
- Complete refactoring of main()
- Added interactive menu
- Separated attack logic into run_attack()
- Support for both modes (CLI and interactive)
- Added display_interfaces() and display_about() functions

#### Makefile
- Added ui.c to sources
- Fixed formatting with tabs
- Added .PHONY for targets

### 🎯 Usage

#### Before (v1.0)
```bash
./arpspoof eth0 192.168.1.50 192.168.1.1
```

#### Now (v2.0)
```bash
# Interactive mode (recommended)
sudo ./arpspoof

# OR classic mode (still supported)
sudo ./arpspoof eth0 192.168.1.50 192.168.1.1
```

### 📊 Statistics

- **Lines of code added**: ~400+
- **New files**: 7
- **Functions added**: 10+
- **ANSI colors used**: 8

---

## Version 1.0 - Initial Version

### Basic Features
- ⚔️ Sending forged ARP packets
- 🔌 RAW socket support
- 🎯 Configuration via CLI arguments
- ⌨️ SIGINT signal handling (Ctrl+C)
- 📦 ARP/Ethernet packet construction

### Original Files
```
src/main.c            - Main program
src/network.c         - Network functions
src/packet.c          - Packet construction
include/network.h     - Network headers
include/protocols.h   - Protocol structures
Makefile              - Compilation
```

---

## 🚀 Next Versions (Roadmap)

### Version 2.1 (Planned)
- [ ] Automatic mode with network scan
- [ ] Automatic gateway detection
- [ ] Statistics export to file
- [ ] Support for multiple simultaneous targets

### Version 3.0 (Future)
- [ ] Advanced ncurses interface
- [ ] Real-time traffic graphs
- [ ] "Stealth" mode with random delays
- [ ] Integration with Wireshark
- [ ] HTML report generation

### Version 4.0 (Vision)
- [ ] Web Interface (optional)
- [ ] IPv6 Support
- [ ] Countermeasure detection
- [ ] Interactive learning mode
- [ ] Plugin system

---

## 🐛 Known Bugs

No critical bugs identified at the moment.

### Limitations
- Requires root privileges (system limitation)
- No IPv6 support (planned for v4.0)
- Automatic mode not implemented (planned for v2.1)

---

## 🤝 Contributions

Contributions are welcome! Please:
1. Fork the project
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

---

## 📝 Release Notes

**Release Date v2.0**: December 2024
**Compatibility**: Linux (tested on Ubuntu/Debian)
**Dependencies**: gcc, make, root privileges

---

*For any questions or suggestions, feel free to open an issue on GitHub.*
