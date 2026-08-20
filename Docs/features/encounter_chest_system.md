# Encounter Chest System

## Overview

The encounter chest system adds a single randomized `PID_CHEST` to an encounter map when active NPC threats are present.

The chest is intentionally tied to active threats rather than simply being generated when a map is initialized. This prevents players from returning to an otherwise inactive encounter map and looting a previously generated chest.

After the chest is successfully created, the existing loot system is called:

```cpp
FillEncounterContainers(map);
```

This keeps encounter loot generation centralized.

---

## Core Behavior

The intended flow is:

```text
Get critters on map
        |
        v
Filter out non-threats
        |
        v
Any active threats?
   NO ---------> Do nothing
   YES
        |
        v
Check existing containers/chest
        |
        v
Already present?
   YES --------> Do nothing
   NO
        |
        v
Find random valid map position
        |
        v
Create PID_CHEST
        |
        v
FillEncounterContainers(map)
```

The system should create **at most one encounter chest**.

---

# Threat Detection

Threat detection uses the same logic already proven by the `kill_all_mobs()` cheat function.

The basic API is:

```cpp
array<Critter@> critters;
uint critterCount = map.GetCritters(0, FIND_ALL, critters);
```

`GetCritters()` provides the critters associated with the map. The returned critters must then be filtered.

## Threat Filtering

A critter counts as an active threat only when all of these conditions are satisfied.

### Valid reference

```cpp
if(!valid(cr))
    continue;
```

Invalid references are ignored.

### NPC

```cpp
if(!cr.IsNpc())
    continue;
```

Players are therefore not considered threats.

### Alive

```cpp
if(cr.IsDead())
    continue;
```

Dead NPCs do not keep the encounter chest active.

### Not a companion

```cpp
if(IsCompanion(cr))
    continue;
```

Player companions are not considered encounter threats.

### Not protected

```cpp
if(GodOfTheRealm(cr.Id))
    continue;
```

Special/admin-protected entities are ignored.

The resulting threat count is:

```cpp
uint threats = 0;

for(uint i = 0; i < critterCount; i++)
{
    Critter@ cr = critters[i];

    if(!valid(cr))
        continue;

    if(!cr.IsNpc())
        continue;

    if(cr.IsDead())
        continue;

    if(IsCompanion(cr))
        continue;

    if(GodOfTheRealm(cr.Id))
        continue;

    threats++;
}
```

---

# Why This Detection Method Is Used

A simple check such as:

```cpp
map.GetCritters(0, FIND_ALL, critters);
```

is not enough to determine whether an encounter is active.

A map can contain players, companions, dead NPCs, and special entities.

The existing `kill_all_mobs()` function already defines which entities should be treated as mobs. Reusing the same filtering makes encounter chest behavior consistent with the server's existing threat logic.

---

# Chest Spawn Condition

The first condition is the threat count.

```cpp
if(threats == 0)
{
    return;
}
```

Therefore:

```text
No active NPC threats
        |
        v
No chest
```

while:

```text
One or more active NPC threats
        |
        v
Chest creation is allowed
```

This is the main anti-exploit mechanism.

---

# Preventing Duplicate Chests

Containers can be detected with:

```cpp
array<Item@> containers;
uint containerCount = map.GetItemsByType(ITEM_TYPE_CONTAINER, containers);
```

This can be used to prevent repeated chest creation.

The important rule is:

```text
Existing container/chest
        |
        v
Do not create another chest
```

This matters if the chest-spawning function can be called more than once.

If the map contains ordinary containers that should not prevent the encounter chest, the container list should instead be inspected for the specific `PID_CHEST` prototype. Do not assume that every `ITEM_TYPE_CONTAINER` is an encounter chest unless that is the intended map design.

---

# Chest Prototype

The encounter chest uses:

```cpp
PID_CHEST
```

Creation:

```cpp
Item@ chest = map.AddItem(hx, hy, PID_CHEST, 1);
```

Only one item is added.

Always validate the returned item:

```cpp
if(!valid(chest))
{
    Log("ScatterChest: Failed to create chest.");
    return;
}
```

---

# Random Chest Position

The chest should be placed at a randomized valid location.

The existing map-location logic can be reused:

```cpp
array<Entire> entires;
ParseEntires(map, entires, 0);
```

If no valid locations are available:

```cpp
if(entires.length() == 0)
{
    Log("ScatterChest: No valid entires found.");
    return;
}
```

Select a random entry:

```cpp
uint re = Random(0, entires.length() - 1);
```

Use the entry as the starting position:

```cpp
uint16 hx = entires[re].HexX;
uint16 hy = entires[re].HexY;
```

Then apply a random direction and distance:

```cpp
uint dist = Random(20, 100);

map.GetHexCoord(
    entires[re].HexX,
    entires[re].HexY,
    hx,
    hy,
    Random(0, 359),
    dist
);
```

The final position must be checked:

```cpp
if(!map.IsHexPassed(hx, hy))
{
    Log("ScatterChest: Selected hex is not passable.");
    return;
}
```

---

# Filling the Chest

Do not implement separate loot logic inside `ScatterChest()`.

After successful creation:

```cpp
FillEncounterContainers(map);
```

The order is important:

```text
Create PID_CHEST
        |
        v
FillEncounterContainers(map)
```

If the fill function is called before the chest exists, the new chest cannot be populated by that call.

---

# Recommended Function Structure

The function should follow this order:

```cpp
void ScatterChest(Map& map)
{
    // 1. Detect active threats.
    // 2. Stop if there are none.
    // 3. Check whether an encounter container/chest already exists.
    // 4. Find valid random map locations.
    // 5. Select a random location.
    // 6. Create PID_CHEST.
    // 7. Validate creation.
    // 8. Call FillEncounterContainers(map).
}
```

This ordering keeps the checks cheap and avoids generating unnecessary map positions when there is no active encounter.

---

# Anti-Exploit Rationale

## Problem

Generating the chest during map initialization can make it persist independently of the encounter.

A player could potentially:

1. Enter the encounter map.
2. Avoid or finish the encounter.
3. Return later.
4. Find the previously generated chest.
5. Loot it without an active encounter.

This makes the chest a permanent map resource instead of an encounter reward.

## Solution

The chest is conditional on active threats.

The desired relationship is:

```text
Active encounter
      |
      +---- Threats exist
      |
      +---- Chest exists
      |
      +---- Loot generated
```

rather than:

```text
Map initialized
      |
      +---- Chest permanently exists
```

---

# Important Edge Cases

## Player only

A player on the map does not count as a threat because:

```cpp
if(!cr.IsNpc())
    continue;
```

No chest should be created.

## Companion only

A companion is excluded:

```cpp
if(IsCompanion(cr))
    continue;
```

No chest should be created.

## Dead enemies only

Dead enemies are excluded:

```cpp
if(cr.IsDead())
    continue;
```

No active threats means no new chest.

## Protected NPC

Protected entities are excluded with:

```cpp
if(GodOfTheRealm(cr.Id))
    continue;
```

They should not activate the chest.

## Multiple enemies

If multiple valid enemies are present, the threat count is greater than zero, but the system still creates only one chest.

---

# Testing Checklist

- [ ] Empty map: no chest.
- [ ] Player only: no chest.
- [ ] Companion only: no chest.
- [ ] Dead NPCs only: no chest.
- [ ] Protected NPC only: no chest.
- [ ] One active enemy: one chest.
- [ ] Multiple active enemies: one chest.
- [ ] Calling the function again does not create another chest.
- [ ] Chest uses `PID_CHEST`.
- [ ] Chest is placed at a randomized valid position.
- [ ] `FillEncounterContainers(map)` runs after successful chest creation.
- [ ] Chest receives encounter loot correctly.

---

# Logging

Useful diagnostic messages include:

```cpp
Log("ScatterChest: No threats found on map " + map.Id + ".");
```

```cpp
Log("ScatterChest: Map " + map.Id +
    " already contains a container. Chest not spawned.");
```

```cpp
Log("ScatterChest: No valid entires found.");
```

```cpp
Log("ScatterChest: Failed to create chest.");
```

```cpp
Log("ScatterChest: Spawned one chest on map " + map.Id +
    " with " + threats + " active threat(s).");
```

These logs make it possible to determine whether the function failed because there were no threats, an existing container prevented spawning, no valid location was found, or item creation failed.

---

# Summary

The encounter chest system follows one central rule:

> **An encounter chest should only be generated when active NPC threats are present.**

Threat detection reuses the established filtering logic from `kill_all_mobs()`.

The chest uses:

```cpp
PID_CHEST
```

and is populated through:

```cpp
FillEncounterContainers(map);
```

The system therefore provides a randomized, single-container encounter reward while avoiding the exploit of permanently pre-generating chests on encounter maps.
