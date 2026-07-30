# 🐧 Running FOnline Engine on Linux

This guide covers setting up, compiling (if using native builds), and running the **FOnline Server**, **Client**, and **Mapper** on Linux systems.

---

## 🛠 Prerequisites

Before starting, install the required dependencies based on whether you're running a native Linux build or Windows binaries through Wine.

### Native Dependencies (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install -y build-essential cmake git \
    libsdl2-dev libgl1-mesa-dev \
    libopenal-dev libvorbis-dev
```

### Wine Environment (Windows Binaries)

If your FOnline SDK contains Windows executables (`.exe`), install Wine.

#### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y wine winetricks
```

#### Arch Linux

```bash
sudo pacman -S wine winetricks
```

---

# ⚙️ Running the Server on Linux

## Option A: Native Linux Binary

If your SDK includes a native Linux server executable:

1. Navigate to the server directory.

```bash
cd Server/
```

2. Make the binary executable.

```bash
chmod +x FOnlineServer
```

3. Start the server.

```bash
./FOnlineServer
```

---

## Option B: Windows Binary via Wine

If you only have `FOnlineServer.exe`:

```bash
cd Server/
wine FOnlineServer.exe
```

> **Note**
>
> On first startup, the server compiles all `.fos` scripts into `.fosb` bytecode. Ensure the server has permission to write to the script directories.

---

# 🎮 Running the Client & Mapper

## File Name Sensitivity ⚠️

Linux filesystems (such as **ext4**) are case-sensitive, unlike Windows.

Make sure:

- Asset filenames match the exact casing referenced by your scripts.
  - ✅ `fallout.dat`
  - ❌ `Fallout.DAT`
- Configuration paths in `Client/FOnline.cfg` use the correct filename casing.

---

## Running the Client

```bash
cd Client/
wine FOnline.exe
```

---

## Running the Mapper

```bash
cd Mapper/
wine Mapper.exe
```

---

# 🔧 Wine Troubleshooting & Performance Tips

## Virtual Desktop Mode

If the client or mapper has problems with fullscreen, resolution scaling, or mouse capture, run Wine inside a virtual desktop.

```bash
wine explorer /desktop=FOnline,1024x768 Client/FOnline.exe
```

---

## Headless Dedicated Server (No GUI)

When running `FOnlineServer.exe` on a remote Linux VPS without a desktop environment, use **Xvfb**.

### Install Xvfb

```bash
sudo apt install xvfb
```

### Launch the server

```bash
xvfb-run -a wine Server/FOnlineServer.exe
```

---

# 📜 Systemd Service

To automatically start and restart the server on boot, create:

```text
/etc/systemd/system/fonline.service
```

Contents:

```ini
[Unit]
Description=FOnline Game Server
After=network.target

[Service]
Type=simple
User=fonline
WorkingDirectory=/home/fonline/server/Server
ExecStart=/home/fonline/server/Server/FOnlineServer
Restart=on-failure
RestartSec=5s

[Install]
WantedBy=multi-user.target
```

Reload systemd and enable the service:

```bash
sudo systemctl daemon-reload
sudo systemctl enable --now fonline
```

Check the service status:

```bash
systemctl status fonline
```

View live logs:

```bash
journalctl -u fonline -f
```