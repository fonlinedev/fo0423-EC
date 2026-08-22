# FOnline Server — Port 2238 Connectivity Diagnostic

## Overview

This document records the diagnostic procedure used to troubleshoot connectivity to the FOnline server running through Wine on Linux.

**Server port:** `2238`  
**Protocol tested:** TCP  
**Server process:** `wineserver32`

The final issue was not a firewall or networking configuration problem. The server was restarting, and connectivity worked once the restart had completed.

---

## 1. Check Whether Port 2238 Is Listening

Run:

```bash
sudo ss -lntup | grep 2238
```

Expected output:

```text
tcp LISTEN 0 4096 0.0.0.0:2238 0.0.0.0:* users:(("wineserver32",pid=...,fd=...))
```

### What this confirms

- `LISTEN` means a process has opened the port.
- `0.0.0.0:2238` means the service is listening on all IPv4 interfaces.
- If the result is `127.0.0.1:2238`, the service is only accessible locally.
- If nothing is returned, the server is not currently listening on port 2238.

---

## 2. Test the Port Locally

Test localhost:

```bash
nc -vz 127.0.0.1 2238
```

Expected:

```text
Connection to 127.0.0.1 2238 port [tcp/*] succeeded!
```

Then test the server's own IP:

```bash
nc -vz $(hostname -I | awk '{print $1}') 2238
```

Expected:

```text
Connection to <SERVER_IP> 2238 port [tcp/*] succeeded!
```

### Interpretation

If both tests succeed:

- The service is running.
- The service is accepting TCP connections.
- The port is reachable through the server's network interface.
- The problem is unlikely to be a basic bind/listener problem.

---

## 3. Check the Linux Firewall

For UFW:

```bash
sudo ufw status verbose
```

Port 2238 should have an allow rule such as:

```text
2238/tcp ALLOW IN Anywhere
```

If required, add:

```bash
sudo ufw allow 2238/tcp
```

If the application also requires UDP:

```bash
sudo ufw allow 2238/udp
```

Do not open additional ports unless the FOnline server actually requires them.

---

## 4. Check iptables

Run:

```bash
sudo iptables -L -n -v
```

Look for an input rule allowing TCP port 2238.

For example:

```text
ACCEPT ... tcp dpt:2238
```

The packet counter is also useful. If the counter increases while a client attempts to connect, traffic is reaching the firewall rule.

---

## 5. Check nftables

On systems using nftables/iptables-nft:

```bash
sudo nft list ruleset
```

Look for:

```text
tcp dport 2238 ... accept
```

If UFW manages the firewall, avoid manually modifying nftables rules that UFW owns.

---

## 6. Test From the Client PC

On Windows PowerShell:

```powershell
Test-NetConnection SERVER_IP -Port 2238
```

Example:

```powershell
Test-NetConnection 103.240.146.224 -Port 2238
```

A successful result contains:

```text
TcpTestSucceeded : True
```

This confirms that the PC can establish a TCP connection to the server's IP and port.

---

## 7. Use tcpdump for the Final Network Test

If the client still cannot connect, run on the Linux server:

```bash
sudo tcpdump -ni any tcp port 2238
```

Then attempt to connect using the actual FOnline client.

### If nothing appears

The connection is not reaching the server.

Possible causes include:

- Wrong server IP
- External firewall
- Cloud/VPS security group
- Router/NAT configuration
- Routing problem

### If packets appear

The client is reaching the server.

You can then investigate:

- FOnline configuration
- Client/server compatibility
- Application logs
- Connection handling
- Required protocols or ports

Stop tcpdump with:

```text
Ctrl+C
```

---

## 8. Diagnostic Decision Tree

```text
Is port 2238 listening?
        |
        +-- NO --> Start/restart the FOnline server
        |
        +-- YES
             |
             v
Does nc localhost:2238 work?
        |
        +-- NO --> Investigate FOnline/Wine/server process
        |
        +-- YES
             |
             v
Does nc SERVER_IP:2238 work?
        |
        +-- NO --> Investigate interface/network configuration
        |
        +-- YES
             |
             v
Does Windows Test-NetConnection succeed?
        |
        +-- NO --> Investigate external firewall/routing/NAT
        |
        +-- YES
             |
             v
Does the FOnline client connect?
        |
        +-- NO --> Investigate FOnline client/server configuration
        |
        +-- YES --> Network connectivity is working
```

---

## 9. Important Lesson From This Incident

A successful listener and open firewall do **not** necessarily mean the application is ready to accept players.

In this incident:

1. Port `2238` was correctly listening.
2. Local TCP connections succeeded.
3. The server's own IP accepted TCP connections.
4. UFW allowed TCP `2238`.
5. The Windows client could successfully connect to `103.240.146.224:2238`.
6. The actual problem was that the FOnline server was **restarting**.
7. Once the restart completed, the server became usable.

Therefore, before changing firewall or networking configuration, always verify that the application has **fully finished starting/restarting**.

---

## 10. Useful Quick-Diagnostic Commands

### Check listener

```bash
sudo ss -lntup | grep 2238
```

### Check local connectivity

```bash
nc -vz 127.0.0.1 2238
```

### Check server-interface connectivity

```bash
nc -vz $(hostname -I | awk '{print $1}') 2238
```

### Check firewall

```bash
sudo ufw status verbose
```

### Check iptables

```bash
sudo iptables -L -n -v
```

### Check nftables

```bash
sudo nft list ruleset
```

### Watch incoming TCP traffic

```bash
sudo tcpdump -ni any tcp port 2238
```

### Windows client test

```powershell
Test-NetConnection SERVER_IP -Port 2238
```

---

## Recommended Troubleshooting Order

Always work from the inside out:

1. **Is the FOnline server running?**
2. **Is port 2238 listening?**
3. **Does localhost connect?**
4. **Does the server's own IP connect?**
5. **Does the Linux firewall allow 2238?**
6. **Can an external machine reach 2238?**
7. **Does the actual FOnline client connect?**
8. **Only then investigate FOnline-specific protocol/configuration issues.**

This avoids making unnecessary firewall or network changes when the real issue is simply that the server is still starting or restarting.
