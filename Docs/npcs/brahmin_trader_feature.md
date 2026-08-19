# Brahmin Trader Feature

## Overview

The Brahmin Trader allows the player to sell a **tamed Brahmin follower** for **100 bottle caps**.

The feature is designed to work with Brahmin that have been captured/tamed through the normal follower system. The trader does not simply look for any Brahmin on the map; it verifies that the animal is a Brahmin follower owned by the player and close enough to the trader.

## Player Flow

1. The player captures/tames a Brahmin using the normal Brahmin capture mechanic.
2. The Brahmin becomes a follower of the player.
3. The player brings the Brahmin close to the Brahmin trader.
4. The trader searches for a valid Brahmin follower.
5. If a valid Brahmin is found, the trader:
   - removes the Brahmin follower,
   - gives the player **100 bottle caps**,
   - completes the sale.

## Brahmin Validation

The trader uses `GetPlayerBrahmin()` to locate the animal.

A candidate Brahmin must pass all of these checks:

```cpp
GetFollowerType(crits[i]) == FOLLOWER_TYPE_BRAHMIN
```

The critter must be registered as a Brahmin follower.

```cpp
IsOwnersFollower(player, crits[i])
```

The Brahmin must belong to the player.

```cpp
SellableBrahmin(crits[i])
```

The critter must be considered a sellable Brahmin.

```cpp
RightBrahminDistance(crits[i], npc)
```

The Brahmin must be within the required distance from the trader.

The trader returns the first critter that passes all checks.

## Sale

The sale is performed by `r_SellBrahmin()`.

The basic process is:

```cpp
Critter@ brahmin = GetPlayerBrahmin(player, npc);

if(!valid(brahmin))
    return 0;

DeleteNpc(brahmin);
player.AddItem(PID_BOTTLE_CAPS, BRAHMIN_PRICE);
```

The current sale price is:

```cpp
#define BRAHMIN_PRICE 100
```

Therefore, a successful sale awards:

**100 bottle caps**

and removes the sold Brahmin.

## Follower Setup

A Brahmin must be created as a proper follower for the trader to recognize it.

The normal capture process uses:

```cpp
MakeFollower(
    target,
    FOLLOWER_TYPE_BRAHMIN,
    "follower@critter_init",
    FOLLOWER_DLG_BRAHMIN,
    capturer,
    true
);

ClearEnemyStack(target);
ClearFollowersEnemy(capturer, target);

target.FollowerVarBase[FV_MERCID] = 9;
target.FollowerVarBase[FV_TYPE] = FOLLOWER_TYPE_BRAHMIN;
```

The important property is that the follower system must report:

```cpp
GetFollowerType(target) == FOLLOWER_TYPE_BRAHMIN
```

Simply spawning a critter whose type is Brahmin is not sufficient if it has not been registered as a Brahmin follower.

## Test Cheat

A dedicated cheat command is available for testing the trader without having to capture a Brahmin manually.

The command is:

```text
~runscript cheats c_AddBrahmin 0 0 0
```

The cheat function follows the same follower setup used by the normal Brahmin capture process.

The current test Brahmin uses:

```cpp
uint16 brahminPid = 10;
```

The spawned critter is then configured with:

```cpp
MakeFollower(
    brahmin,
    FOLLOWER_TYPE_BRAHMIN,
    "follower@critter_init",
    FOLLOWER_DLG_BRAHMIN,
    player,
    true
);

ClearEnemyStack(brahmin);
ClearFollowersEnemy(player, brahmin);

brahmin.FollowerVarBase[FV_MERCID] = 9;
brahmin.FollowerVarBase[FV_TYPE] = FOLLOWER_TYPE_BRAHMIN;
```

This makes the test Brahmin behave like a normally captured Brahmin from the trader's point of view.

## Testing Procedure

### Basic Sale Test

1. Enter a map with the Brahmin trader.
2. Execute:

```text
~runscript cheats c_AddBrahmin 0 0 0
```

3. Confirm that the Brahmin appears and follows the player.
4. Move the Brahmin close to the trader.
5. Open the trader dialogue.
6. Select the option to sell the Brahmin.
7. Verify that:
   - the Brahmin disappears,
   - the player receives 100 bottle caps.

### Distance Test

Move the Brahmin away from the trader.

The trader should not find it when it is outside the configured selling distance.

Move it back close to the trader and verify that the sale becomes available again.

### Ownership Test

The trader should only accept a Brahmin that is recognized as the player's follower.

### Follower Type Test

The trader specifically checks:

```cpp
GetFollowerType(crits[i]) == FOLLOWER_TYPE_BRAHMIN
```

This prevents unrelated followers from being sold as Brahmin.

## Important Implementation Details

### Brahmin ProtoId

The Brahmin map object used for testing has:

```ini
ProtoId 10
ScriptName all_brahmin
FuncName _Domesticated
```

Therefore the test cheat currently uses ProtoId `10` when spawning the Brahmin.

### Trader Search

The trader searches NPCs on the player's current map:

```cpp
player.GetMap().GetCritters(
    0,
    FIND_ALL | FIND_ONLY_NPC,
    crits
);
```

It then checks each candidate until it finds a valid Brahmin follower.

### First Matching Brahmin

`GetPlayerBrahmin()` returns the first Brahmin that passes all validation checks.

If no suitable Brahmin is found, it returns:

```cpp
return null;
```

and the sale cannot proceed.

## Troubleshooting

### "Brahmin not found"

Check the following:

```cpp
GetFollowerType(brahmin)
```

must return:

```cpp
FOLLOWER_TYPE_BRAHMIN
```

Also verify:

```cpp
IsOwnersFollower(player, brahmin)
```

returns `true`.

Finally, make sure the Brahmin is close enough to the trader.

### Cheat Function Not Found

`~runscript` expects cheat functions with this signature:

```cpp
void FunctionName(Critter& player, int p0, int p1, int p2)
```

Therefore the test function must be declared as:

```cpp
void c_AddBrahmin(Critter& player, int p0, int p1, int p2)
```

It is executed with:

```text
~runscript cheats c_AddBrahmin 0 0 0
```

## Current Feature Specification

| Property | Value |
|---|---|
| Follower type | `FOLLOWER_TYPE_BRAHMIN` |
| Test ProtoId | `10` |
| Sale price | `100` bottle caps |
| Ownership required | Yes |
| Distance check | Yes |
| NPC required | Yes |
| Test command | `~runscript cheats c_AddBrahmin 0 0 0` |

## Relevant Functions

```cpp
GetPlayerBrahmin()
SellableBrahmin()
RightBrahminDistance()
d_PlayerHasBrahmin()
r_SellBrahmin()
c_AddBrahmin()
```

These functions together provide the Brahmin detection, validation, sale, and testing workflow.
