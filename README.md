# 📦 FOnline Engine - Server & Development Kit

Welcome to the project repository! This repository contains the source code, scripts, maps, tools, and client runtime files for the custom FOnline server project.

---

# 📁 Repository Structure

```text
.
├── Server/      # Server core, AngelScript, dialogs, maps, databases
├── Client/      # Game client, assets, UI, configuration
├── Mapper/      # Map editor and mapping resources
└── Tools/       # SDK utilities and development tools
```

## Directory Overview

### `Server/`

Contains all server-side logic, game mechanics, scripts, and world data.

**Important directories:**

| Directory | Description |
|-----------|-------------|
| `Server/scripts/` | AngelScript source files (`.fos`) |
| `Server/dialogs/` | NPC dialogue files (`.fodlg`) |
| `Server/maps/` | Map source files (`.fomap`) |

---

### `Client/`

Contains the game client used by players, including:

- Executables
- Graphics and animations
- UI assets
- Configuration files
- Localization resources

---

### `Mapper/`

The official FOnline map editor used for:

- Creating maps
- Editing existing locations
- Managing scenery and objects

---

### `Tools/`

Development utilities including:

- Asset compilers
- Script validation tools
- Batch helpers
- Deployment utilities

---

# ⚙️ Client Setup

## Original Fallout Assets

### Required Game Archives

Depending on your server module, the client may require the original Fallout game archives:

- `fallout.dat`
- `fallout2.dat`

### When are they required?

If your client uses original Fallout graphics, critters, wall tiles, or sounds instead of custom packed archives, you **must** provide the original data files.

### Installation

1. Locate your legal installation of **Fallout 1** or **Fallout 2**.
2. Copy the required archive(s) into the `Client/` directory.

```text
Client/
├── fallout.dat
└── fallout2.dat
```

3. Verify that `Client/FOnline.cfg` references the correct data archives if required by your SDK build.

---

## Client Configuration

Client settings are stored in:

```text
Client/FOnline.cfg
```

Example:

```ini
[Game Options]
ServerHost = 127.0.0.1
ServerPort = 2238

[Language]
Language = engl
```

---

# 🚀 Getting Started

## Prerequisites

Before running the project, install:

- Windows (or Wine on Linux)
- Visual Studio C++ Redistributables
- Git

---

## Running the Server

Launch the server executable:

```text
Server/FOnlineServer.exe
```

or use your platform's startup script.

> **Note**
>
> On the first launch, all AngelScript (`.fos`) files are automatically compiled into `.fosb` bytecode.

---

## Connecting the Client

Launch:

```text
Client/FOnline.exe
```

By default, it connects to:

- **Host:** `127.0.0.1`
- **Port:** `2238`

---

## Editing Maps

Launch the mapper:

```text
Mapper/Mapper.exe
```

Use it to inspect or modify existing `.fomap` files.

> **Important**
>
> Do **not** commit generated binary map files (`.fomapb`) to source control.

---

# 📝 Scripting & Quest Development

## AngelScript

All gameplay logic is written in **AngelScript** using `.fos` files located under:

```text
Server/scripts/
```

---

## NPC Dialogues

NPC conversations are authored with the **FOnline Dialog Editor** using `.fodlg` files.

---

## Testing Tips

When testing quests and game logic:

- Clear temporary test variables before retesting.
- Use in-game debug commands such as:
  - `~set_var`
  - `~check_vars`
- Verify quest state transitions before committing changes.