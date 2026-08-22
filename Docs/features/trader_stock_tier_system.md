# FOnline Revival — Trader Stock Tier System

## Purpose

This document records the current trader stock configuration and the rules that govern how trader levels select inventory pools.

The system separates:

- **Trader category level** — selects the stock pool.
- **Stock list** — defines possible items and configured quantities.
- **Trader capacity** — determines how much stock can actually be generated.
- **Barter skill** — controls trading modifiers independently of stock availability.

A higher level does **not** automatically mean more or less stock. It selects a different tier list.

---

## Category Mapping

The seven trader categories are indexed as follows:

| Index | Category |
|---:|---|
| 0 | Small Guns |
| 1 | Big Guns |
| 2 | Energy |
| 3 | Armor |
| 4 | Drugs |
| 5 | Medical |
| 6 | Misc |

The master list is:

```cpp
uint[][] @[] lists = {
    sg1, sg2, sg3,
    bg1, bg2, bg3,
    en1, en2, en3,
    arm1, arm2, arm3,
    drug1, drug2, drug3,
    med1, med2, med3,
    misc1, misc2, misc3
};
```

The selection rule is:

```cpp
list = lists[type * 3 + level - 1];
```

Therefore:

| Category | Level 1 | Level 2 | Level 3 |
|---|---|---|---|
| Small Guns | `sg1` | `sg2` | `sg3` |
| Big Guns | `bg1` | `bg2` | `bg3` |
| Energy | `en1` | `en2` | `en3` |
| Armor | `arm1` | `arm2` | `arm3` |
| Drugs | `drug1` | `drug2` | `drug3` |
| Medical | `med1` | `med2` | `med3` |
| Misc | `misc1` | `misc2` | `misc3` |

Level-0 means that category is not configured for the trader.

---

# Current Stock Lists

## Small Guns

### Level 1

```cpp
uint[][] sg1 = {
    { PID_44_MAGNUM_REVOLVER, 1 },
    { PID_DESERT_EAGLE, 1 },
    { PID_HUNTING_RIFLE, 1 },
    { PID_TOMMY_GUN, 1 },
    { PID_SHOTGUN, 1 },
    { PID_SAWED_OFF_SHOTGUN, 1 },
    { PID_GREASE_GUN, 1 },
    { PID_44_MAGNUM_JHP, 20 },
    { PID_10MM_JHP, 50 },
    { PID_9MM_BALL, 10 },
    { PID_SHOTGUN_SHELLS, 30 },
    { PID_SHOTGUN_SHELLS_FLECHETTE, 10 },
    { PID_SHOTGUN_SHELLS_SLUG, 10 },
    { PID_45_CALIBER_AMMO, 20 }
};
```

### Level 2

```cpp
uint[][] sg2 = {
    { PID_14MM_PISTOL, 1 },
    { PID_ASSAULT_RIFLE, 1 },
    { PID_10MM_SMG, 1 },
    { PID_COMBAT_SHOTGUN, 1 },
    { PID_SHOTGUN_SHELLS, 30 },
    { PID_SHOTGUN_SHELLS_FLECHETTE, 20 },
    { PID_SHOTGUN_DRAGON_BREATH_SHELLS, 10 },
    { PID_SHOTGUN_SHELLS_EMP, 10 },
    { PID_SHOTGUN_SHELLS_SLUG, 20 },
    { PID_5MM_JHP, 250 },
    { PID_14MM_AP, 100 },
    { PID_40MM_GRENADE, 10 },
    { PID_GRENADE_LAUNCHER, 1 },
    { PID_GRENADE_PISTOL, 1 },
    { PID_223_FMJ, 100 }
};
```

### Level 3

```cpp
uint[][] sg3 = {
    { PID_SNIPER_RIFLE, 1 },
    { PID_HK_CAWS, 1 },
    { PID_223_PISTOL, 1 },
    { PID_INDEPENDENT, 1 },
    { PID_HK_P90C, 1 },
    { PID_PANCOR_JACKHAMMER, 1 },
    { PID_40MM_GRENADE, 10 },
    { PID_GRENADE_LAUNCHER, 1 },
    { PID_GRENADE_PISTOL, 1 },
    { PID_5MM_AP, 250 },
    { PID_SHOTGUN_SHELLS, 50 },
    { PID_SHOTGUN_SHELLS_FLECHETTE, 20 },
    { PID_SHOTGUN_SHELLS_SLUG, 20 },
    { PID_SHOTGUN_DRAGON_BREATH_SHELLS, 10 },
    { PID_SHOTGUN_SHELLS_EMP, 20 },
    { PID_10MM_AP, 50 }
};
```

## Big Guns

### Level 1

```cpp
uint[][] bg1 = {
    { PID_FLAMER, 1 },
    { PID_ROCKET_LAUNCHER, 1 },
    { PID_FLAMETHROWER_FUEL, 10 },
    { PID_EXPLOSIVE_ROCKET, 1 },
    { PID_7_62MM_AMMO, 100 },
    { PID_M60, 1 }
};
```

### Level 2

```cpp
uint[][] bg2 = {
    { PID_5MM_AP, 250 },
    { PID_ADV_SUPERBURN, 10 },
    { PID_ROCKET_AP, 8 },
    { PID_5MM_JHP, 250 },
    { PID_223_FMJ, 100 },
    { PID_MINIGUN, 1 },
    { PID_LIGHT_SUPPORT_WEAPON, 1 }
};
```

### Level 3

```cpp
uint[][] bg3 = {
    { PID_AVENGER_MINIGUN, 1 }
};
```

## Energy

### Level 1

```cpp
uint[][] en1 = {
    { PID_LASER_RIFLE, 1 },
    { PID_SMALL_ENERGY_CELL, 30 }
};
```

### Level 2

```cpp
uint[][] en2 = {
    { PID_PLASMA_RIFLE, 1 },
    { PID_RIPPER, 1 },
    { PID_PLASMA_PISTOL, 1 },
    { PID_POWER_FIST, 1 },
    { PID_MICRO_FUSION_CELL, 150 },
    { PID_PLASMA_GRENADE, 5 }
};
```

### Level 3

```cpp
uint[][] en3 = {
    { PID_GEIGER_COUNTER, 1 },
    { PID_MOTION_SENSOR, 1 }
};
```

## Armor

### Level 1

```cpp
uint[][] arm1 = {
    { PID_SAND_ROBE, 1 },
    { PID_LEATHER_ARMOR, 1 },
    { PID_LEATHER_ARMOR_MK_II, 1 },
    { PID_CURED_LEATHER_ARMOR, 1 },
    { PID_GECKO_PELT, 1 },
    { PID_GOLDEN_GECKO_PELT, 1 },
    { PID_BRAHMIN_HIDE, 1 },
    { PID_COMBAT_LEATHER_JACKET_HELMET, 1 },
    { PID_LEATHER_ARMOR_HELMET, 1 },
    { PID_LEATHER_ARMOR_HELMET_MK2, 1 }
};
```

### Level 2

```cpp
uint[][] arm2 = {
    { PID_TESLA_ARMOR, 1 },
    { PID_COMBAT_ARMOR, 1 },
    { PID_METAL_ARMOR, 1 },
    { PID_METAL_ARMOR_MK_II, 1 },
    { PID_COMBAT_ARMOR_MK_II, 1 },
    { PID_COMBAT_HELMET, 1 },
    { PID_COMBAT_HELMET_MK_II, 1 },
    { PID_METAL_HELMET, 1 },
    { PID_METAL_HELMET_MK2, 1 },
    { PID_TESLA_HELMET, 1 }
};
```

### Level 3

```cpp
uint[][] arm3 = {};
```

## Drugs

### Level 1

```cpp
uint[][] drug1 = {
    { PID_BUFFOUT, 1 },
    { PID_MENTATS, 1 }
};
```

### Level 2

```cpp
uint[][] drug2 = {
    { PID_RADAWAY, 1 },
    { PID_RAD_X, 1 }
};
```

### Level 3

```cpp
uint[][] drug3 = {
    { PID_PSYCHO, 1 }
};
```

**Important:** drug tiers are independent pools. Level 2 uses `drug2`; it does not automatically include `drug1`.

## Medical

### Level 1

```cpp
uint[][] med1 = {
    { PID_STIMPAK, 1 },
    { PID_HEALING_POWDER, 1 },
    { PID_ANTIDOTE, 1 }
};
```

### Level 2

```cpp
uint[][] med2 = {
    { PID_HN_NEEDLER_CARTRIDGE, 10 },
    { PID_HN_AP_NEEDLER_CARTRIDGE, 10 },
    { PID_NEEDLER_PISTOL, 1 },
    { PID_SUPER_STIMPAK, 1 }
};
```

### Level 3

```cpp
uint[][] med3 = {
    { PID_FIRST_AID_BOOK, 1 },
    { PID_DOCTORS_BAG, 1 },
    { PID_FIRST_AID_KIT, 1 }
};
```

## Misc

### Level 1

```cpp
uint[][] misc1 = {
    { PID_KNIFE, 5 },
    { PID_SPEAR, 1 },
    { PID_THROWING_KNIFE, 5 },
    { PID_SLEDGEHAMMER, 1 },
    { PID_ROPE, 1 },
    { PID_GECKO_PELT, 1 },
    { PID_BRAHMIN_HIDE, 1 },
    { PID_MOLOTOV_COCKTAIL, 5 },
    { PID_AXE, 1 },
    { PID_LOCKPICKS, 1 },
    { PID_TENT, 1 }
};
```

### Level 2

```cpp
uint[][] misc2 = {
    { PID_SUPER_SLEDGE, 1 },
    { PID_GECKO_PELT, 1 },
    { PID_GOLDEN_GECKO_PELT, 1 },
    { PID_BRAHMIN_HIDE, 1 },
    { PID_FRAG_GRENADE, 5 },
    { PID_DECK_OF_TRAGIC_CARDS, 1 },
    { PID_FUZZY_PAINTING, 1 },
    { PID_GREEN_CONDOM, 1 },
    { PID_LIGHTER, 1 },
    { PID_FROZEN_DINNER, 1 }
};
```

### Level 3

```cpp
uint[][] misc3 = {};
```

---

# Current NPC Configurations

## Beth

```cpp
void _Beth(Critter& npc, bool firstTime)
{
    uint[] levels =
    {
        0, 0, 1, 0, 1, 1, 1
    };

    SetTraderLevels(npc, levels);
    npc.SkillBase[SK_BARTER] = 120;
    DefaultTraderInit(npc, firstTime);
}
```

Beth:

| Category | Level | Pool |
|---|---:|---|
| Small Guns | 0 | None |
| Big Guns | 0 | None |
| Energy | 1 | `en1` |
| Armor | 0 | None |
| Drugs | 1 | `drug1` |
| Medical | 1 | `med1` |
| Misc | 1 | `misc1` |

Barter: **120**

## Jacob

```cpp
void _Jacob(Critter& npc, bool firstTime)
{
    uint[] levels =
    {
        0, 2, 1, 2, 0, 0, 0
    };

    SetTraderLevels(npc, levels);
    npc.SkillBase[SK_BARTER] = 145;
    DefaultTraderInit(npc, firstTime);
}
```

Jacob:

| Category | Level | Pool |
|---|---:|---|
| Small Guns | 0 | None |
| Big Guns | 2 | `bg2` |
| Energy | 1 | `en1` |
| Armor | 2 | `arm2` |
| Drugs | 0 | None |
| Medical | 0 | None |
| Misc | 0 | None |

Barter: **145**

---

# How Stock Generation Works

The configured list is a **candidate pool**, not a guarantee that every listed item will always be present.

The trader refill system uses:

- trader category level;
- the selected list;
- configured item quantity;
- `BaseItemValue()`;
- remaining trader capacity;
- existing inventory.

When stock is refreshed, surviving inventory consumes capacity. Available capacity is then used to generate additional items from the appropriate list.

Therefore, replenishment is **capacity-based**, not an exact replacement of the item that disappeared.

The quantity in:

```cpp
{ PID_SOMETHING, 100 }
```

is the configured quantity used when that stock entry is generated. It is not a promise that the trader will permanently maintain exactly 100 units.

---

# Design Rules

## Tier selection

Use:

```text
0 = category disabled
1 = first stock tier
2 = second stock tier
3 = third stock tier
```

## Tier contents

Keep tier lists intentionally separate.

Do not assume:

```text
Level 2 = Level 1 + Level 2
```

Instead:

```text
Level 1 → list 1
Level 2 → list 2
Level 3 → list 3
```

This allows each trader tier to have a deliberately controlled economy and item progression.

## Quantity

Do not use trader level as a quantity multiplier. Control stock quantity through:

- list quantity;
- item base value;
- trader capacity;
- refill behavior.

## Empty tiers

An empty list is valid and intentionally means no configured stock for that tier.

---

# Master Configuration

The current authoritative master list is:

```cpp
uint[][] @[] lists = {
    sg1, sg2, sg3,
    bg1, bg2, bg3,
    en1, en2, en3,
    arm1, arm2, arm3,
    drug1, drug2, drug3,
    med1, med2, med3,
    misc1, misc2, misc3
};
```

When changing trader behavior, update this document together with `trader.fos`.

