# FOnline Engine (0506-EC Variant) - Doctor NPC Documentation

This document records the exact syntax, structural rules, and files required to create a functional Doctor NPC in the FOnline 0506-EC (Classic/2238) engine generation. This build uses a rigid node-based `.fodlg` layout and explicit script argument counters.

---

## 1. Prototype Definition (NPC Template)
The server maps NPC properties through listing files inside `server/proto/critters/`.

* **File modified:** `server/proto/critters/citizens` (or `citizens.fopro`)
* **Rule:** Append this to the bottom of the file. Ensure the `ProtoId` is unique.

```text
[Proto]
ProtoId=550
ST_BASE_CRTYPE=48   // Vault suit citizen sprite
ST_DIALOG_ID=4000   // Links to our dialogue ID
MODE_NO_LOOT=1      // Prevent corpse looting
MODE_NO_STEAL=1     // Prevent pickpocketing
MODE_NO_PUSH=1      // Prevent players from pushing the NPC
```

---

## 2. Master Dialogue Registry
The engine parses a global index of valid text files on startup.

* **File modified:** `server/dialogs/dialogs.lst`
* **Rule:** Separate tokens with a single **Tab**, not spaces. Append to the bottom:

```text
$	4000	doctor
```

---

## 3. Dialogue Node Configuration (.fodlg)
The engine parser requires a specific entry loop structure (`1 1000` jumping to a speaking node) and demands explicit parameter counting (`0`) for script execution.

* **File created:** `server/dialogs/doctor.fodlg`
* **Rule:** Ensure there is single-space formatting around symbols and exactly **one blank line** at the very bottom of the text file.

```text
[comment]


[data]
lang=engl

[dialog]
&
1 1000 None 0 #
3 1010 @
3 2000 None 0 #
0 2010 R _script npc_doctor@r_HealPlayer 0 #
0 2020 &

[engl]
{100}{}{Doctor}
{200}{}{You see a doctor.}
{1000}{}{Predialogue installations}
{1010}{}{Answer text}
{2000}{}{Hello there. Do you need medical attention?}
{2010}{}{Yes, please patch me up.}
{2020}{}{No, I am fine. Bye.}
```

---

## 4. AngelScript Healing Logic
This variant expects a 2-parameter signature for dialogue actions and relies on `ST_MAX_LIFE` for total health thresholds.

* **File created:** `server/scripts/npc_doctor.fos`
* **Prerequisite:** Ensure `@ server module npc_doctor` is declared in `server/scripts/scripts.cfg`.

```angelscript
#include "_macros.fos"
#include "_defines.fos"

// 2-parameter signature strictly requested by dialogue binder
void r_HealPlayer(Critter& player, Critter@ npc)
{
    if(@npc == null) return;
    
    // Check if player is completely healthy (HP and Limbs)
    if(player.Stat[ST_CURRENT_HP] >= player.Stat[ST_MAX_LIFE] && 
       player.Damage[DAMAGE_EYE] == 0 && player.Damage[DAMAGE_RIGHT_ARM] == 0 &&
       player.Damage[DAMAGE_LEFT_ARM] == 0 && player.Damage[DAMAGE_RIGHT_LEG] == 0 && 
       player.Damage[DAMAGE_LEFT_LEG] == 0)
    {
        npc.Say(SAY_DIALOG, "You look perfectly healthy to me.");
        return;
    }

    // Fully heal Hit Points
    player.StatBase[ST_CURRENT_HP] = player.Stat[ST_MAX_LIFE];

    // Reset all crippled limbs
    if(player.Damage[DAMAGE_EYE] != 0) player.DamageBase[DAMAGE_EYE] = 0;
    if(player.Damage[DAMAGE_RIGHT_ARM] != 0) player.DamageBase[DAMAGE_RIGHT_ARM] = 0;
    if(player.Damage[DAMAGE_LEFT_ARM] != 0) player.DamageBase[DAMAGE_LEFT_ARM] = 0;
    if(player.Damage[DAMAGE_RIGHT_LEG] != 0) player.DamageBase[DAMAGE_RIGHT_LEG] = 0;
    if(player.Damage[DAMAGE_LEFT_LEG] != 0) player.DamageBase[DAMAGE_LEFT_LEG] = 0;

    npc.Say(SAY_DIALOG, "There you go. All patched up!");
}
```

---

## 5. Deployment and Testing
1. Restart the server completely to update data configurations and script binders.
2. In-game, use an Admin/Developer character.
3. Open chat and type **`~crit 550`** to manually spawn the doctor.
4. Left-click to initiate the custom interface loop.
