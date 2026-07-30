md_content = """# FOnline TLA: Building & Running Headless Linux Server on WSL 2

A comprehensive guide for setting up, compiling, baking assets, and running the **FOnline TLA** server within a **Windows Subsystem for Linux 2 (WSL 2)** environment.

---

## 📋 Table of Contents
1. [Overview & Architecture](#overview--architecture)
2. [Prerequisites & System Setup](#prerequisites--system-setup)
   - [Fixing Package Manager (apt/dpkg) Issues](#fixing-package-manager-aptdpkg-issues)
   - [Enabling WSL 2 & Virtualization](#enabling-wsl-2--virtualization)
3. [Installing Build Dependencies](#installing-build-dependencies)
4. [Configuring & Compiling the Engine](#configuring--compiling-the-engine)
   - [CMake Configuration](#cmake-configuration)
   - [Compiling Headless Server & Baker](#compiling-headless-server--baker)
5. [Content Baking Pipeline](#content-baking-pipeline)
6. [Launching the Server](#launching-the-server)
7. [Connecting from Windows Client](#connecting-from-windows-client)
8. [Workflow & Troubleshooting Reference](#workflow--troubleshooting-reference)

---

## 1. Overview & Architecture

The modern FOnline engine (0.3.8+) uses a unified **CMake** build framework and a decoupled **Content Baking Pipeline**:

* **WSL 2 (Ubuntu)**: Runs the authoritative headless server (`TLA_ServerHeadless`) and the content baking utility (`TLA_Baker`).
* **Windows Host**: Runs the graphical game client (`FOClient`), connecting to the WSL 2 server instance via loopback networking (`127.0.0.1`).
* **Baker Pipeline**: Transforms raw script files (`.fos`), maps, prototypes, dialogs, and graphic assets into optimized server/client archives.

---

## 2. Prerequisites & System Setup

### Fixing Package Manager (apt/dpkg) Issues

If `apt` reports unmet dependencies between `systemd`, `libpam-systemd`, and `udev`:

```bash
sudo dpkg --configure -a
sudo apt --fix-broken install
sudo apt clean && sudo apt update