# Usage Guide - The Armory

## 🚀 Quick Start

### Option 1: Interactive Mode (Recommended)

```bash
sudo ./arpspoof
```

The interactive interface will guide you through all options.

### Option 2: Command Line Mode

```bash
sudo ./arpspoof <interface> <target_ip> <gateway_ip>
```

**Practical Example:**
```bash
sudo ./arpspoof eth0 192.168.1.50 192.168.1.1
```

---

## 📖 Usage Examples

### Scenario 1: Intercepting Device Traffic

**Context:** You want to intercept traffic between your smartphone (192.168.1.50) and your router (192.168.1.1)

```bash
# 1. Enable IP forwarding (to avoid blocking the network)
echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward

# 2. Launch the attack
sudo ./arpspoof eth0 192.168.1.50 192.168.1.1

# 3. In another terminal, capture the traffic
sudo tcpdump -i eth0 -w capture.pcap host 192.168.1.50
```

### Scenario 2: Network Security Test

Test if your network is vulnerable to ARP spoofing attacks:

```bash
# On your test machine
sudo ./arpspoof wlan0 <target_ip> <gateway_ip>

# On the target machine, check the ARP table
arp -a
# You should see the attacker's MAC associated with the gateway IP
```

---

## 🎯 Navigating the Interactive Menu

### [1] Manual Configuration
- Choose this option to manually configure the attack
- You will need to provide:
  - Network interface (ex: eth0, wlan0)
  - Target IP (victim)
  - IP to spoof (usually the gateway)

### [2] Automatic Mode
- **In development** - This feature will allow automatic network scanning

### [3] Show Interfaces
- Lists all available network interfaces on your system
- Useful for identifying which interface to use

### [4] About
- Information about the tool
- Documentation and warnings

### [0] Exit
- Cleanly closes the application

---

## 🔍 Understanding Messages

### Success Messages (Green) ✓
```
[✓] RAW Socket opened successfully
[✓] Interface 'eth0' found (index: 2)
```
Everything is working correctly.

### Information Messages (Cyan) *
```
[*] Initializing attack...
[*] Total packets sent: 42
```
Information about the operation progress.

### Attack Messages (Red) ⚔
```
[⚔] Packet #1 sent → 192.168.1.50 believes 192.168.1.1 is at aa:bb:cc:dd:ee:ff
```
Confirmation of forged ARP packet transmission.

### Warning Messages (Yellow) !
```
[!] Press Ctrl+C to stop the attack
```
Important information to note.

### Error Messages (Bright Red) ✗
```
[✗] Unable to open socket (Are you root?)
```
Something is not working correctly.

---

## 🛡️ Protection against ARP Spoofing

To protect yourself against this type of attack:

1. **Static ARP Entries**: Manually define MAC addresses
   ```bash
   sudo arp -s 192.168.1.1 aa:bb:cc:dd:ee:ff
   ```

2. **ARP Monitoring**: Use tools like `arpwatch`
   ```bash
   sudo apt install arpwatch
   sudo arpwatch -i eth0
   ```

3. **Port Security**: Configure port security on switches
   - Limit the number of MACs per port
   - Enable DHCP snooping

4. **DAI (Dynamic ARP Inspection)**: On managed switches
   - Validates ARP packets
   - Blocks forged ARP packets

---

## 🐛 Troubleshooting

### "Are you root?"
- The tool requires root privileges to create RAW sockets
- Solution: `sudo ./arpspoof`

### "Unable to find interface"
- Check your interface name: `ip link show`
- Common names: eth0, wlan0, enp0s3, wlp2s0

### "No packets sent"
- Check that the interface is up: `ip link set <interface> up`
- Check your network connectivity

### Attack not working
- Enable IP forwarding: `echo 1 > /proc/sys/net/ipv4/ip_forward`
- Check that IPs are correct and reachable
- Some networks have protections against ARP spoofing

---

## ⚖️ Legal Aspects

⚠️ **IMPORTANT**: The use of this tool is regulated by law.

### ✅ Legal Usage
- Your own home network
- Isolated test lab
- Corporate network with written authorization
- Authorized training environment

### ❌ Illegal Usage
- Public network (cafe, library, etc.)
- Corporate network without authorization
- Someone else's network without permission
- Any malicious use

**Possible Sanctions**: Fines, prison sentences, professional ban

---

## 📚 Further Reading

### Complementary Tools
- **Wireshark**: Analyze captured traffic
- **Ettercap**: Complete suite for MITM
- **dsniff**: Network tool collection
- **Bettercap**: Modern framework for network attacks

### Resources
- [RFC 826 - ARP Protocol](https://tools.ietf.org/html/rfc826)
- [OWASP Testing Guide](https://owasp.org/www-project-web-security-testing-guide/)
- Network security documentation

---

**Use this knowledge responsibly!** 🛡️
