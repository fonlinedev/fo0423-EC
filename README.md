# FOnline Engine - Server & Development Kit

Welcome to the project repository! This repository contains the source code, scripts, maps, tools, and client runtime files for our custom FOnline server project.

---

## 📁 Repository Structure

├── Server/      # Server core, AngelScript code (.fos), dialogs (.fodlg), and database files
├── Client/      # Client application, graphics, UI configs, and localized assets
├── Mapper/      # Map editor tools, graphic sets, and map configuration files
└── Tools/       # Utility scripts, SDK compilers, batch helpers, and development utilities

### Breakdown of Directories

* **`Server/`**: Holds all server logic, game mechanics, and world state configuration.
  * `Server/scripts/`: Raw AngelScript source code (`.fos`).
  * `Server/dialogs/`: Dialogue trees and NPC interaction files (`.fodlg`).
  * `Server/maps/`: Raw map files (`.fomap`).
* **`Client/`**: The executable client environment used by players to connect to the server.
* **`Mapper/`**: The official map editor used for level design and editing game spaces.
* **`Tools/`**: External tools for asset compilation, script validation, and server deployment.

---

## ⚙️ Client Setup & Original Fallout Assets

### Required Game Archives (`fallout.dat` / `fallout2.dat`)

Depending on the specific graphic assets, animations, and sound effects used by the server modules:

* **When is it required?**
  If your client build uses original engine graphics, critters, wall tiles, or sound effects directly from *Fallout 1* or *Fallout 2* rather than custom packed `.zip`/`.dat` archives, you **must** copy the original data files into the client directory.
  
* **Setup Instructions:**
  1. Locate your original legal installation of *Fallout 1* or *Fallout 2*.
  2. Copy `fallout.dat` (and/or `fallout2.dat`) into your root `Client/` folder:
     ```
     Client/fallout.dat
     Client/fallout2.dat
     ```
  3. Ensure `Client/FOnline.cfg` references the DAT paths properly under the data section if required by your build.

### Client Configuration (`FOnline.cfg`)

Client settings are stored in `Client/FOnline.cfg`. Key options to review:

[Game Options]
ServerHost = 127.0.0.1
ServerPort = 2238

[Language]
Language = engl

🚀 Getting Started (Developers)
1. Prerequisites
Windows OS / Wine on Linux

C++ Runtime Dependencies (Visual Studio C++ Redistributables)

Git

2. Building & Running the Server
Launch the server via Server/FOnlineServer.exe (or your platform's startup script).

On first run, AngelScript files (.fos) will compile automatically into .fosb bytecode.

Open Client/FOnline.exe to connect locally (127.0.0.1:2238).

3. Editing Maps
Launch Mapper/Mapper.exe to inspect or modify existing .fomap files.

Do not commit generated binary map files (.fomapb).

📝 Scripting & Quest Development
Game logic is written in AngelScript (.fos).

NPC Dialogues are modified using the FOnline Dialog Editor (.fodlg).

Always clear test parameters or use the in-game debug commands (e.g., ~set_var, ~check_vars) when verifying new quest state loops.