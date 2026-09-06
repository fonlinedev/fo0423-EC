# Raider Camp Dialogue Structure

## Overview

The Raider camp currently has three dialogue sets:

  -----------------------------------------------------------------------
  File                    Purpose                 Current content
  ----------------------- ----------------------- -----------------------
  `GENRAID2.MSG`          Main Raider dialogue    Full conversation

  `GENRAIDR.MSG`          Raider variant          Same structure as
                                                  `GENRAID2.MSG`, with
                                                  one wording change

  `GENRAIDB.MSG`          Hostile/combat Raider   Short combat dialogue
  -----------------------------------------------------------------------

The three dialogues should be treated as **three different Raider
encounter types**, rather than three copies of the same conversation.

------------------------------------------------------------------------

# 1. GENRAID2.MSG --- Normal Raider

## Purpose

This should be the standard Raider who is willing to talk, but is
suspicious of outsiders.

### Opening / Recognition

**NPC description** - `{101}` --- Tough desert Raider.

### Branch A --- Player is recognized as Death-Hand

**NPC** - `{102}` --- "It's the Death-Hand. He's returned!"

Possible responses:

-   `{103}` --- Respectful response.
-   `{104}` --- Ask whether the player has returned to kill Garl.

Possible NPC reactions:

-   `{105}` --- Raider panics: "Aiyeee! A ghost!"
-   `{106}` --- Raider announces that Death-Hand has returned.

This branch should make the Raider afraid/respectful of the player's
reputation.

------------------------------------------------------------------------

## Branch B --- Friendly / Neutral greeting

Opening NPC lines:

-   `{107}` --- "Well met."
-   `{108}` --- "Hi."
-   `{109}` --- Raider explains that he is waiting for Garl to crush
    Shady Sands and wants loot.
-   `{110}` --- "Welcome, friend."
-   `{111}` --- "I'm glad that you could return for a visit."

This is the safe conversational branch.

------------------------------------------------------------------------

## Branch C --- Player has weapon drawn

NPC reactions:

-   `{112}` --- Warning to put the weapon away.
-   `{113}` --- Challenge/party response, requiring the player to put
    the weapon away.
-   `{114}` --- "Oh, a tough one, huh?"
-   `{115}` --- Tells the player to put the weapon away and speak to
    Garl.
-   `{116}` --- Direct threat/challenge.

Possible escalation:

``` text
Weapon drawn
    |
    +-- Put weapon away
    |      |
    |      +-- Continue normal dialogue
    |
    +-- Refuse / antagonize
           |
           +-- Raider becomes hostile
```

------------------------------------------------------------------------

## Branch D --- Raider refuses the player

NPC lines:

-   `{117}` --- "I don't like your looks."
-   `{118}` --- "Go away!"
-   `{119}` --- "What do you want?"

This is the basic suspicious-Raider branch.

------------------------------------------------------------------------

# 2. Tandi Investigation Branch

This is the main information branch in the existing dialogue.

## Entry

Player:

-   `{120}` --- "I'm looking for a woman named Tandi. Seen her?"

NPC can respond:

### Branch T1 --- Raider has not seen Tandi

NPC:

-   `{125}` --- "Nah. I ain't seen her. Why are you asking?"

Player:

-   `{126}` --- "Nothing. Just looking for a friend of mine."

This can end the conversation or return to the main Raider dialogue.

------------------------------------------------------------------------

### Branch T2 --- Player admits there is a bounty

Player:

-   `{127}` --- "There is a bounty on her head. If I can kill her, I can
    get some good cash."

NPC:

-   `{132}` --- Confirms Tandi is in the big house, but warns that Garl
    would want a share of the bounty.

This is the **successful information branch**.

Suggested flow:

``` text
Ask about Tandi
    |
    +-- Say she is a friend
    |      |
    |      +-- Raider does not provide useful information
    |
    +-- Say there is a bounty
           |
           +-- Raider reveals:
                  Tandi is in the big house
                  Garl does not know / may want a share
```

------------------------------------------------------------------------

### Branch T3 --- Raider challenges the player's story

NPC:

-   `{133}` --- Raider suspects the player is lying and threatens to
    claim the bounty himself.

This should lead toward an intimidation/confrontation branch.

------------------------------------------------------------------------

### Branch T4 --- Raider is afraid of Garl

NPC:

-   `{134}` --- "I... I can't say any more or Garl will kill me."

This is the **fear-of-Garl branch**.

The dialogue should terminate or return the player to a general
conversation without giving additional information.

------------------------------------------------------------------------

# 3. GENRAIDR.MSG --- Raider Variant

`GENRAIDR.MSG` follows essentially the **same dialogue tree as
`GENRAID2.MSG`**.

The important difference is:

-   `{113}` is changed to:
    -   "Do you want to party? If not, put your weapon away."

This makes the weapon-warning branch slightly more explicit.

## Structure

Use the same branches as `GENRAID2.MSG`:

1.  Death-Hand recognition
2.  Friendly greeting
3.  Weapon drawn
4.  Suspicious / hostile response
5.  Tandi investigation
6.  Bounty disclosure
7.  Fear of Garl
8.  Conversation termination

The message IDs remain aligned, so the same dialogue logic can be reused
with this variant.

------------------------------------------------------------------------

# 4. GENRAIDB.MSG --- Hostile / Combat Raider

This dialogue should be used when the Raider is already hostile or
combat is about to begin.

It should **not** contain the long Tandi investigation tree.

## Branch H1 --- Warning

NPC:

-   `{100}` --- "Stay out of our way!"

Purpose: - Raider warns the player. - Can be used as the initial hostile
response.

------------------------------------------------------------------------

## Branch H2 --- Combat starts

NPC:

-   `{101}` --- "Kill or be killed!"

Purpose: - Immediate combat transition.

Suggested flow:

``` text
Hostile Raider
    |
    +-- Warning
    |      |
    |      +-- Player backs away
    |
    +-- Player continues
           |
           +-- "Kill or be killed!"
                  |
                  +-- COMBAT
```

------------------------------------------------------------------------

## Branch H3 --- Raider defeated

NPC:

-   `{102}` --- "Enjoy your victory, and pray to your gods that we never
    meet again!"

Use this after the player wins the encounter if the Raider
survives/flees rather than being killed.

------------------------------------------------------------------------

## Branch H4 --- Raider refuses conversation

NPC:

-   `{103}` --- "I have no time for talk!"

This is the generic hostile response.

------------------------------------------------------------------------

# Recommended Three-Raider Setup

The three dialogue types should therefore have clearly different
purposes.

## Raider Type 1 --- Normal Raider

**File:** `GENRAID2.MSG`

``` text
Approach
   |
   +-- Death-Hand?
   |      |
   |      +-- Recognition / fear
   |
   +-- Friendly
   |      |
   |      +-- General Raider conversation
   |
   +-- Weapon drawn
   |      |
   |      +-- Warning
   |      +-- Threat
   |
   +-- Ask about Tandi
          |
          +-- Friend story
          |      |
          |      +-- No useful information
          |
          +-- Bounty story
          |      |
          |      +-- Tandi is in the big house
          |
          +-- Raider suspicious
          |
          +-- Raider afraid of Garl
```

## Raider Type 2 --- Variant Raider

**File:** `GENRAIDR.MSG`

Same tree as the normal Raider, but with slightly different
wording/personality.

``` text
Same logic
   |
   +-- Different wording
   +-- Same message IDs
   +-- Same Tandi information
   +-- Same Garl fear
```

This is useful if different Raider NPCs should feel like different
individuals without requiring a completely different dialogue script.

## Raider Type 3 --- Hostile Raider

**File:** `GENRAIDB.MSG`

``` text
Approach
   |
   +-- Stay out of our way
   |
   +-- Player continues
   |      |
   |      +-- Kill or be killed
   |             |
   |             +-- COMBAT
   |
   +-- Player wins
   |      |
   |      +-- Enjoy your victory...
   |
   +-- Player tries to talk
          |
          +-- I have no time for talk
```

------------------------------------------------------------------------

# Implementation Notes

## Message ID groups

### Normal / Variant

-   `100` --- Fear / special state
-   `101` --- NPC description
-   `102–106` --- Death-Hand recognition
-   `107–111` --- Friendly interaction
-   `112–116` --- Weapon / intimidation
-   `117–119` --- Suspicious interaction
-   `120–134` --- Tandi investigation
-   `135–137` --- Final hostile/exit responses

### Hostile

-   `100` --- Warning
-   `101` --- Combat
-   `102` --- Post-combat victory
-   `103` --- Refuses conversation

------------------------------------------------------------------------

# Suggested Final Dialogue Logic

The cleanest implementation is to select the dialogue **before entering
the conversation**, based on the Raider's state:

``` text
                 Raider
                    |
          +---------+---------+
          |                   |
       Hostile?            Friendly
          |                   |
         YES                  |
          |                   |
     GENRAIDB.MSG       Death-Hand?
                              |
                    +---------+---------+
                    |                   |
                   YES                  NO
                    |                   |
             Death-Hand branch     GENRAID2 / R
                                        |
                              +---------+---------+
                              |                   |
                         Weapon drawn?        Normal
                              |                   |
                         Weapon branch       General
                                                  |
                                             Ask about Tandi
```

This gives the camp three recognizable Raider personalities/states while
keeping the existing message IDs reusable.
