# The Admin & God Mode Saga (`admin_god_mode`)

## Overview
The **Admin & God Mode Saga** documents the administrative tools, server management commands, and ultimate developer capability ("God Mode") integrated into the server architecture [cite: 2]. This documentation serves as a comprehensive reference manual for gamemasters, administrators, and engine developers [cite: 2].

---

## Technical Specifications

### Admin Privilege Tiers
Access control and command authority are strictly partitioned into distinct privilege levels [cite: 2]:

| Tier Level | Role Title | Description & Scope |
| :--- | :--- | :--- |
| **Tier 0** | Player | Standard player level with default gameplay restrictions [cite: 2]. |
| **Tier 1** | Game Master (GM) | Event support tier; capability to spawn items, teleports, and perform basic moderation [cite: 2]. |
| **Tier 2** | Administrator | Server manager tier; access to critter stat manipulation, script reloads, and player bans [cite: 2]. |
| **Tier 3** | Developer / God | Absolute control; full access to memory states, engine hooks, invulnerability, and instant execution routines [cite: 2]. |

---

## Access & Configuration Setup

### 1. Acquiring Access (`config/admin.cfg`)
Admin tiers are registered directly in the server database or mapped inside `config/admin.cfg` [cite: 2]:

```ini
[Admins]
Developer_Main = 3
Admin_Leader   = 2
Event_GM_01    = 1
```

To grant yourself access:
1. Open `config/admin.cfg`.
2. Add your character or account name under the `[Admins]` section.
3. Assign a privilege tier level (`1`, `2`, or `3`) [cite: 2].

### 2. Enabling Cheats (`cheats.cfg`)
To enable developer console overrides and enable in-game cheat execution, update or create `config/cheats.cfg`:

```ini
[Cheats]
EnableCheats = 1
AllowTeleport = 1
AllowGodMode = 1
ConsoleLogging = 1
```

> **Note:** Setting `EnableCheats = 1` enables administrative overrides and command execution for authorized admin accounts on the server.

### 3. Configuring Teleportation (`scripts/teleport.fos`)
To register and enable the `~teleport` command, create or update `scripts/teleport.fos` and register it in `scripts/scripts.cfg`.

#### A. Script Registration (`scripts/scripts.cfg`)
```text
@ server module teleport
```

#### B. Teleport Logic Implementation (`scripts/teleport.fos`)
```cpp
#include "_macros.fos"
#include "_defines.fos"

// Command handler for ~teleport [hexX] [hexY]
bool pr_teleport(Critter& player, string@ command, int hexX, int hexY, int)
{
    // Verify privilege tier before executing
    if(player.GetAccess() < ACCESS_ADMIN)
    {
        player.Say(SAY_NETMSG, "Access denied. Teleportation requires GM or Admin privilege.");
        return true;
    }

    Map@ map = player.GetMap();
    if(@map == null)
    {
        player.Say(SAY_NETMSG, "Cannot teleport while on the Global Map.");
        return true;
    }

    // Validate coordinates on map
    if(hexX <= 0 || hexY <= 0 || hexX >= map.GetWidth() || hexY >= map.GetHeight())
    {
        player.Say(SAY_NETMSG, "Invalid target coordinates.");
        return true;
    }

    // Perform hex transit
    player.TransitToHex(hexX, hexY, player.Dir);
    player.Say(SAY_NETMSG, "Teleported to (" + hexX + ", " + hexY + ").");
    return true;
}
```

---

## Command Reference & Syntax

### 1. General & Navigation Commands
Commands designed for quick map traversal, inspection, and location manipulation [cite: 2].

```text
~teleport [hexX] [hexY]      - Teleport directly to coordinates on the current map [cite: 2].
~goto [player_id]            - Teleport directly to the designated player [cite: 2].
~summon [player_id]          - Teleport the target player to your exact location [cite: 2].
~location [loc_id]           - Spawns and moves you to a global map location instance [cite: 2].
```

### 2. Player & Critter Management
Commands used to inspect, modify, or enforce administrative actions on critters [cite: 2].

```text
~kill                        - Kills the selected critter instantly [cite: 2].
~respawn                     - Respawns a dead critter or target player [cite: 2].
~setstat [stat_id] [value]   - Modifies a specific core stat on the targeted critter [cite: 2].
~setvar [var_id] [value]     - Explicitly overrides a character variable (`ST_VAR`) [cite: 2].
~kick [player_id] [reason]   - Disconnects the target player from the server [cite: 2].
~ban [player_id] [minutes]  - Bans the account from logging into the server [cite: 2].
```

### 3. Inventory & Spawning Tools
Commands for creating items, weapons, and entire container setups on demand [cite: 2].

```text
~additem [pid] [count]       - Spawns the specified Item Proto ID directly into your inventory [cite: 2].
~createitem [pid] [x] [y]    - Spawns an item on the ground at target coordinates [cite: 2].
~clearinv                    - Clears all items currently in the targeted critter's inventory [cite: 2].
```

---

## God Mode Mechanics (`god_mode.fos`)

### Key Features
When activated via `~godmode`, the administrator is granted absolute control over character states [cite: 2]:

* **Complete Invulnerability:** Immunity to all incoming damage types (Unarmed, Normal, Laser, Plasma, Electrical, Explosive, Fire) [cite: 2].
* **Infinite Action Points:** Overrides AP costs to zero, allowing continuous actions and attacks [cite: 2].
* **Status Effect Immunity:** Immune to Knockdown, Crippled Limbs, Blindness, Radiation, and Poisoning [cite: 2].
* **Invisible / Ghost State:** Optional toggle to enter complete invisibility to NPCs and players (`ST_STEALTH_MOD`) [cite: 2].
* **Instant Kill Aura:** Optional toggle causing any hostiles attacking the God Mode user to take lethal retaliatory damage immediately [cite: 2].

---

## Script Interface (`scripts/admin_god_mode.fos`)

```cpp
#include "_macros.fos"
#include "_defines.fos"

// Flag definition for internal God Mode tracking
#define ST_GOD_MODE               (ST_VAR9) // Dedicated variable slot for God Mode state [cite: 2]

// Command Handler Hook
bool pr_godmode(Critter& player, string@ command, int p0, int p1, int p2)
{
    if(player.GetAccess() < ACCESS_ADMIN)
    {
        player.Say(SAY_NETMSG, "Access denied. High-tier administrative privilege required.");
        return true;
    }

    if(player.Param[ST_GOD_MODE] == 0)
    {
        player.ParamBase[ST_GOD_MODE] = 1;
        player.StatBase[ST_MAX_LIFE] += 9999;
        player.StatBase[ST_CURRENT_HP] += 9999;
        player.Say(SAY_NETMSG, "GOD MODE ACTIVATED. Invulnerability online.");
    }
    else
    {
        player.ParamBase[ST_GOD_MODE] = 0;
        player.StatBase[ST_MAX_LIFE] -= 9999;
        if(player.Stat[ST_CURRENT_HP] > player.Stat[ST_MAX_LIFE])
            player.StatBase[ST_CURRENT_HP] = player.Stat[ST_MAX_LIFE];
            
        player.Say(SAY_NETMSG, "GOD MODE DEACTIVATED. Standard mortality restored.");
    }

    return true;
}

// Damage Calculation Override Hook
void god_OnAttacked(Critter& target, Critter& attacker, uint damage)
{
    if(target.Param[ST_GOD_MODE] == 1)
    {
        // Negate all damage completely
        damage = 0;
        target.StatBase[ST_CURRENT_HP] = target.Stat[ST_MAX_LIFE];
    }
}
```

# Useful scripts
`~runscript cheats tp_map 1 100 100` | Den
`~runscript cheats tp_map 8 100 100` | Modoc
`~runscript cheats tp_map 9 100 100` | Vault City Inner
`~runscript cheats tp_map 12 100 80` | Vault City Vault 3rd floor
`~runscript cheats tp_map 11 100 80` | Vault City Vault 2nd floor
`~runscript cheats tp_map 10 100 80` | Vault City Vault 1st floor
`~runscript cheats tp_map 13 100 80` | Navarro
`~runscript cheats tp_map 14 50 80` | Gecko Junkyard
`~runscript cheats tp_map 15 50 80` | Gecko Powerplant
`~runscript cheats tp_map 16 220 155` | BrokenHills Marcus
`~runscript cheats tp_map 17 50 60` | BrokenHills Basement
`~runscript cheats tp_map 20 50 60` | New Reno
`~runscript cheats tp_map 21 60 60` | New Reno Shark Club
`~runscript cheats tp_map 22 60 60` | New Reno Renesco
`~runscript cheats tp_map 23 60 60` | New Reno Wright State
`~runscript cheats tp_map 24 100 50` | New Reno TRay Garage
`~runscript cheats tp_map 29 50 50` | New Reno Eldridge Basement
`~runscript cheats tp_map 33 50 55` | Sierra Army Depot
`~runscript cheats tp_map 39 50 55` | Vault 15 entrance
`~runscript cheats tp_map 43 50 55` | NCR Entrance
`~runscript cheats tp_map 45 50 55` | BOS NCR Entrance
`~runscript cheats tp_map 46 50 55` | Chatedral Entrance
`~runscript cheats tp_map 49 60 75` | Chatedral 2nd floor
`~runscript cheats tp_map 51 104 105` | Chatedral Basement
`~runscript cheats tp_map 54 70 55` | Chatedral Vault Bomb floor
`~runscript cheats tp_map 55 70 55` | Mariposa base entrance
`~runscript cheats tp_map 60 70 55` | Redding
`~runscript cheats tp_map 63 80 55` | San Francisco Oil tanker
`~runscript cheats tp_map 63 120 265` | San Francisco Vendor
`~runscript cheats tp_map 67 100 100` | Navarro entrance
`~runscript cheats tp_map 70 100 100` | Junktown
`~runscript cheats tp_map 72 100 100` | BOS Broken Hills entrance
`~runscript cheats tp_map 78 100 100` | The Glow
`~runscript cheats tp_map 93 100 100` | Necropolis
98 | Necropolis vault entrance
`~runscript cheats tp_map 100 100 100` | The Hub
103 | Water merchants
104 | Falcon Maltese basement
105 | Theft guild
107 | New Adytum
109 | Raiders Camp
110 | Raiders Cave
111 | Ares Silos
125 | Lybrary
132 | Respawn 1
134 | Respawn 2
135 | Respawn 3
136 | Sanfrancisco Respawn
137 | NCR Army Respawn
138 | Tribal respawn
139 | Custom doctor respawn
142 | Jail
151 | San Francisco Train Station 
152 50 60 | Boneyard Train Station
153 50 60 | NCR Train Station
154 50 60 | Vault City Train Station
155 | Junkyard

`~runscript cheats god 0 0 0`
`~reloadscripts`
