# Bag System

## Basic format

Bags are defined as:

```text
bag_x=itemdef itemdef itemdef
```

- `x` is the bag ID, starting from `1`.
- `itemdef` is an item definition.
- Multiple item definitions are separated with spaces.
- A bag can be empty.
- Bag `0` is generated automatically and is not used.

## Item definitions

An item definition is declared as:

```text
itemdef=pid
```

or with a quantity:

```text
itemdef=pid:count
```

or with a random quantity range:

```text
itemdef=pid:min-max
```

Examples:

```text
Stimpak=PID_STIMPAK
BCapsSmall=PID_BOTTLE_CAPS:0-20
Rocks=PID_ROCK:2-8
```

`pid` can be an item prototype ID or an item prototype name.

## Multiple items

Multiple item definitions on the same line form the bag's possible contents:

```text
bag_9=BCapsSmall Pistol10mm 9_ammo PsychoBuffoutJet FruitOrMeat Stimpak
```

The configuration states that one item is chosen randomly from the listed item definitions.

## Quantity ranges

Use `:min-max` for a random quantity:

```text
BCapsSmall=PID_BOTTLE_CAPS:0-20
```

Use `:count` for a fixed quantity:

```text
StimpakA=PID_STIMPAK:2-3
```

Examples from this file:

```text
PID_ROCK:2-8
PID_JET:2-10
PID_223_FMJ:50
PID_SHOTGUN_SHELLS:20-40
```

## Item slots

A suffix determines the item slot.

```text
^m = active item slot
^e = backup item slot
^a = armor slot
```

Without a suffix, the item uses the inventory slot.

Example:

```text
GunTurretDualPlasma=PID_GUN_TURRET_WEAPON^m+PID_MICRO_FUSION_CELL:500
```

## Combined item definitions

Item definitions can combine multiple items:

```text
SledgeAndKnife=PID_SLEDGEHAMMER+PID_KNIFE
```

This treats the combined definition as one selectable item definition.

Another example:

```text
ShotGunAndShells=PID_SHOTGUN+PID_SHOTGUN_SHELLS:20-40
```

## Example: Radscorpion Tail

The file defines:

```text
ScorpionTail=PID_SCORPION_TAIL:0-1
```

and then assigns it to Bag 1:

```text
bag_1=ScorpionTail
```

This means a creature using Bag ID `1` can receive the `ScorpionTail` item definition.

The file also has:

```text
bag_2=ScorpionTail
```

## Important rules

An item definition cannot be empty.

Bad:

```text
Broken=
```

The configuration notes that an empty item definition can cause the server to crash on startup.

An empty bag itself is allowed:

```text
bag_146=
```

## Common pattern

Define reusable item definitions first:

```text
ScorpionTail=PID_SCORPION_TAIL:0-1
```

Then assign them to bags:

```text
bag_1=ScorpionTail
```

This allows the same item definition to be reused by multiple bags.

## Current file examples

```text
bag_1=ScorpionTail
bag_2=ScorpionTail

bag_9=BCapsSmall Pistol10mm 9_ammo PsychoBuffoutJet FruitOrMeat Stimpak

bag_11=GuardHuntingRifleInHand
bag_12=GuardSniperRifleInHand
bag_13=GuardAssaultRifleInHand
```

## Useful reference

```text
# bag_x=itemdef itemdef itemdef
# x - id of the bag, starting from 1
# itemdef - name of item definition, defined separately in any place of the file
# Multiple item definition are separated with space. A bag can be empty.

# itemdef=pid:min-max
# itemdef - name of a item definition, must be unique
# pid - id or name of item prototype
# :count     fixed quantity
# :min-max   random quantity
```
