# FOnline 2238: Quest & Spatial Dialogue Implementation Guide

This document outlines the end-to-end process for setting up a custom quest, spatial map triggers, container stocking on map generation, dialogue trees, and Pip-Boy integration in the FOnline 2238 engine.

---

## 1. Engine Variable Registration (`_vars.fos`)

All local variables (`LVAR`) used to track player quest progress must be declared in the server's master variable list. If a variable is only `#define`d in headers without being declared in the variable table, calling `GetLocalVar` will return `@null`, causing a **Null Pointer Access** runtime exception when setting values.

### Step 1.1: Register the Variable Table Entry

Open `Server/scripts/_vars.fos` and add your variable at the bottom of the table using the next available ID:

```text
   $	7327	1	q_quest_water_chip	0	0	99	4
**********
   Water chip quest status variable for Vault 13.
**********

```

#### Field Breakdown:

| Parameter | Value | Description |
| --- | --- | --- |
| **ID** | `7327` | Unique integer identifier across the server. |
| **Type** | `1` | `1` = Local Variable (LVAR) stored per player. |
| **Name** | `q_quest_water_chip` | Internal macro name (`LVAR_q_quest_water_chip`). |
| **Default** | `0` | Initial value when player variable is created. |
| **Min** | `0` | Minimum allowed integer value. |
| **Max** | `99` | Maximum allowed integer value. |
| **Flags** | `4` | `4` = Quest Variable (`VAR_QUEST`). |

> **Note:** A full server restart (`FOnlineServer.exe`) is required whenever `_vars.fos` is modified to load the new variable index into the database.

---

## 2. Map Script Development (`map_v13_ent.fos`)

The map script handles map initialization (spawning items in containers on generation), spatial triggers on hexes, and dialogue evaluation functions (`D _script` / `R _script`).

### Step 2.1: Implement Script Code

```angelscript
// ------------------------------------------------------------------
// MAP INITIALIZATION
// ------------------------------------------------------------------
void map_init(Map& map, bool firstTime)
{
    if (firstTime)
    {
        // Populate container once when map generates
        Item@ skeleton = map.GetItem(72, 41, PID_SKELETON);
        if (valid(skeleton))
        {
            skeleton.AddItem(PID_10MM_PISTOL, 1, 0);
            skeleton.AddItem(PID_10MM_JHP, 24, 0);
            skeleton.AddItem(PID_KNIFE, 1, 0);
            skeleton.AddItem(PID_STIMPAK, 1, 0);
        }
    }
}

```

---

## 3. Dialogue File Setup (`.fod`)

Predialogue evaluation processes lines sequentially from top to bottom and opens the **first** node where all demands (`D`) are met. Unchecked fall-through entries must sit below specific demand checks.

### Step 3.1: Security Intercom Dialogue (`Server/dialogs/v13_gatekeeper.fod`)

```ini
[comment]


[data]
lang=engl

[dialog]
&
1 1000 None 0 #
3 1010 D _script map_v13_ent@d_IsQuestFinished 0 #
2 1020 @
2 2000 None 0 #
0 2010 @
3 3000 None 0 #
0 3010 &

[engl]
{100}{}{Vault Security Intercom}
{200}{}{You see the Vault entry security speaker.}
{1000}{}{Predialogue}
{1010}{}{Quest Finished -> Open Door}
{1020}{}{Default (Not Started / Running) -> Access Denied}
{2000}{}{Halt! Access to the Vault is restricted. Your assigned mission in the wasteland is still in progress. Return when your task is finished.}
{2010}{}{Understood, I'm heading back out.}
{3000}{}{Welcome back. Authentication verified. Stand clear while the blast door cycles open.}
{3010}{}{Thank you.}

```

### Step 3.2: Register Dialogue in `dialogs.cfg`

Open `Server/dialogs/dialogs.cfg` and associate the file with the assigned dialogue ID:

```ini
[dialogs]
4007=v13_gatekeeper

```

---

## 4. Pip-Boy Integration (`FOPipboy.msg`)

Quest entries in the Pip-Boy are mapped using the `LVAR` ID multiplied by `10` (or `100` depending on SDK indexing):

Formula:

* **Title:** `{LVAR_ID * 10}`
* **Description per state:** `{LVAR_ID * 10 + State}`

### Step 4.1: Add Text to `FOPipboy.msg`

Add the text entries to both `Server/text/engl/FOPipboy.msg` and `Client/data/text/engl/FOPipboy.msg`:

```ini
# Vault 13 Water Chip Quest (LVAR 7327 * 10 = 73270)
{73270}{}{Find the Water Chip}
{73271}{}{The Vault's water controller chip has failed. Search the wasteland towns and speak with locals to locate a working replacement.}
{73272}{}{You located the Water Chip! Return to Vault 13 and deliver it to the overseer.}
{73273}{}{Quest Completed. You delivered the Water Chip and saved Vault 13.}

```

---

## 5. Mapper Configuration

1. Launch `FOnlineMapper.exe` and load `v13_ent.map`.
2. Select **Tech** $\rightarrow$ **Spec** on the bottom menu panel.
3. Place a **Trigger** object on the ground hexes leading to the exit or Vault entrance.
4. Select the trigger (`Ctrl + Click`):
* **`ScriptModule`**: `map_v13_ent`
* **`ScriptFunc`**: `t_StartQuest` (for leaving) or `t_CheckVaultStatus` (for entering).


5. Save the map (`Ctrl + S`).

---

## 6. Verification Checklist

1. **Recompile Scripts:** Run script compiler / check `~reloadscripts` in server console.
2. **Server Restart:** Restart `FOnlineServer.exe` to register the new `LVAR_q_quest_water_chip` entry in memory.
3. **Trigger Test:** Walk over the trigger hex; ensure `RunDialog` executes without `Null Pointer Access` errors.
4. **Pip-Boy Test:** Open the Pip-Boy (P key) after running `t_StartQuest` to verify the quest appears under the quest tab.