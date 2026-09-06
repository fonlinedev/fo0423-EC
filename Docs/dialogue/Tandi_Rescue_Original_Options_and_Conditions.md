# Tandi Rescue — Original Fallout Dialogue Options & Quest Conditions

## Purpose

This document contains the **original dialogue texts that are relevant to rescuing Tandi**, together with the conditions/branches that should be implemented.

The goal is to preserve the original Fallout dialogue wording while separating:

- Tandi's rescue declaration
- Garl negotiation
- Garl barter
- Garl threat/intimidation
- Garl one-on-one duel
- Successful rescue
- Failed/hostile outcomes

> **Important:** The text below is taken from the supplied `GENRAID2.MSG` and `GARL.MSG` files. Message wording should not be changed when implementing these branches.

---

# 1. Tandi — Rescue Declaration

## TAND_37

NPC:

> "You? Who are you? What do you want?"

### Player options

**164 + 165**

> "My name is [player], and your father sent me."

→ Continue to TAND_38.

**166**

> "I'm your fairy god-mother. What's it look like?"

**167**

> "Duh."

---

# 2. TAND_38 — Confirm Rescue

NPC:

> "Father sent you!? Are you here to get me out?"

### Option 169 — Rescue

> "Yes, so just sit tight. I'll get you out."

→ Continue to TAND_39.

### Option 170 — Not a rescue

> "No, he just sent me to look at you."

---

# 3. TAND_39 — Choose Rescue Method

NPC:

> "Well, what's your plan?"

This is the important decision point.

## Option 172 — Escape

> "Just wait for the doors to open. Then run for it!"

### Quest meaning

Set the intended rescue method to **escape**.

Do **not** complete the quest here.

Tandi is still captive.

The quest becomes successful only when Tandi is actually freed.

---

## Option 173 — Fight Garl

> "I'm going to fight the Khan leader in exchange for your freedom."

### Quest meaning

Set the intended rescue method to **Garl duel**.

Do **not** start combat here.

Do **not** complete the quest here.

The player must subsequently deal with Garl.

---

# 4. TAND_40 — Escape Plan

NPC:

> "That's it? That's your plan..."

This is the continuation of option 172.

The player has chosen the **escape route**.

Recommended quest state:

```text
TANDI_RESCUE_ESCAPE
```

Tandi remains captive until the actual escape occurs.

---

# 5. TAND_41 — Duel Plan

NPC:

> "You? Fight him? You got to be kidding me!"

This is the continuation of option 173.

Recommended quest state:

```text
TANDI_RESCUE_DUEL
```

The next important NPC is Garl.

---

# 6. Garl — Entering the Rescue Conversation

## GARL_130

Player:

> "What do you want?"

The rescue conversation can then enter the Tandi branch.

### Option 131

> "I want you to set the girl free."

### Option 132

> "I want you to die."

### Option 133

> "I've come in peace to negotiate the release of Tandi."

These establish the main approaches to Garl.

---

# 7. Garl — Establish That He Has Tandi

NPC:

> "What makes you think I have her?"

### Option 136

> "I know you do. I followed you here."

### Option 137

> "One of your followers told me."

### Option 138

> "I know."

These lead into Garl's response:

> "I hate it when they do that. I will have to make that loudmouth pay for his crimes. After I finish up with you."

---

# 8. Garl — Negotiation

Garl:

> "Hmmm. Well, then. What do you offer for her release?"

This creates three important negotiation directions.

---

## 8A. Threat / Intimidation

### Option 141

> "I represent a threat you don't even understand. Do you really want to risk our wrath."

Garl response:

> "Ha! You should not threaten your betters."

This should be treated as the **failed threat branch** unless the implementation conditions make the threat successful.

---

## 8B. Barter

### Option 142

> "I will give you something of equal value."

Garl:

> "It must be valuable indeed. Show me."

Successful response:

> "It pleases me. You may take the girl, but do not return."

Message:

**148**

This is a successful Tandi release.

Recommended quest state:

```text
TANDI_RESCUE_FREED
```

---

### Failed barter

Garl:

> "Your bribe is not worth my time. Kill him."

Message:

**149**

This is a hostile outcome.

Recommended result:

```text
RESCUE FAILED / COMBAT
```

Do **not** mark Tandi rescued.

---

## 8C. Afterlife / Death-Hand argument

### Option 143

> "It would be good for your eternal soul."

Garl response:

> "You think a man such as I believes in the afterlife? This world is dying, and I shall take what I want! You will never realize the strength required to make such a decision!"

Message:

**150**

This does not release Tandi.

It returns the conversation toward confrontation.

---

# 9. Garl — One-on-One Duel

This is the branch that should be connected to Tandi option **173**.

## Player

**203**

> "I'll fight you one-on-one for Tandi."

This is the direct original text for the duel.

Garl can respond:

**168**

> "Fine. We shall settle this with no weapons. Knock me out, and I'll let you take the woman and leave. I knock you out, you leave without her."

Player:

**169**

> "Fine."

This starts the one-on-one fight.

---

# 10. Duel Alternative

There is also the broader challenge:

### 164

> "Would you be willing to fight me over her?"

Garl response options include:

### 166

> "Of course. Right now, just you and me."

### 167

> "I'll take you and your whole gang on."

The original one-on-one agreement is the important route for the Tandi rescue duel.

---

# 11. Duel Conditions

The duel should have a very specific success condition.

### Before combat

```text
TANDI_RESCUE_DUEL
        +
Player challenged Garl
        +
Garl accepted one-on-one
        ↓
Start Garl duel
```

The dialogue itself does **not** complete the quest.

---

## Player wins

Garl:

**172**

> "Hmm, you fight better then I expected. I haven't had a good beating since my father slapped me around the room. You deserve the Shady Sands woman. Take her and leave. I wouldn't come back."

This is the successful duel outcome.

Set:

```text
TANDI_RESCUE_FREED
```

Then release Tandi.

---

## Player loses

Garl's condition is established by:

> "I knock you out, you leave without her."

Therefore:

```text
Player loses duel
        ↓
Tandi remains captive
        ↓
Do NOT complete rescue
```

The player should be able to pursue another solution if the game state permits it.

---

# 12. Direct Successful Release

There are several Garl responses that explicitly release Tandi.

## Death-Hand / special branch

Garl:

**126**

> "So be it. Release the Shady Sands woman and escort this one out of here . . ."

This is a successful release.

---

## Threat / negotiation success

Garl:

**145**

> "Hmmm. For some reason, I believe you. You may take the girl, but do not return unless you seek our wrath."

This is a successful release.

---

## Barter success

Garl:

**148**

> "It pleases me. You may take the girl, but do not return."

This is a successful release.

---

## Duel success

Garl:

**172**

> "Hmm, you fight better then I expected. I haven't had a good beating since my father slapped me around the room. You deserve the Shady Sands woman. Take her and leave. I wouldn't come back."

This is a successful release.

---

# 13. The Quest Completion Condition

Do not use a dialogue selection as the final quest condition.

Use the actual state of Tandi.

Recommended states:

```text
0 = TANDI_RESCUE_NONE
1 = TANDI_RESCUE_ESCAPE
2 = TANDI_RESCUE_DUEL
3 = TANDI_RESCUE_FREED
4 = TANDI_RESCUE_RETURNED
```

## State transitions

```text
NONE
 |
 +-- Tandi confirms rescue
 |
 +-- "Just wait for the doors to open..."
 |       |
 |       v
 |   ESCAPE
 |
 +-- "I'm going to fight the Khan leader..."
         |
         v
       DUEL
```

Then:

```text
ESCAPE
   |
   +-- Tandi actually escapes
   |
   v
FREED
```

or:

```text
DUEL
   |
   +-- Garl accepts
   |
   +-- Player wins
   |
   v
FREED
```

or:

```text
Garl negotiation
   |
   +-- successful threat
   +-- successful barter
   |
   v
FREED
```

Finally:

```text
FREED
   |
   +-- Tandi returns to Shady Sands
   |
   v
RETURNED
   |
   +-- Aradesh dialogue/reward
   |
   v
QUEST COMPLETE
```

---

# 14. Recommended Script Checks

## Tandi dialogue

When option 173 is selected:

```cpp
// Player has chosen the Garl duel solution.
rescueVar = TANDI_RESCUE_DUEL;
```

When option 172 is selected:

```cpp
// Player has chosen the escape solution.
rescueVar = TANDI_RESCUE_ESCAPE;
```

Neither should complete the quest.

---

## Garl dialogue

Before allowing the duel branch:

```cpp
if(rescueVar == TANDI_RESCUE_DUEL)
{
    // Allow:
    // "I'll fight you one-on-one for Tandi."
}
```

After Garl accepts:

```cpp
// Start special one-on-one combat.
```

After the player wins:

```cpp
rescueVar = TANDI_RESCUE_FREED;
```

---

## Successful negotiation

For the successful Garl branches:

```cpp
rescueVar = TANDI_RESCUE_FREED;
```

Then release Tandi.

---

## Final quest completion

Do not do:

```cpp
quest = COMPLETE;
```

when Tandi is merely released.

Instead:

```cpp
if(rescueVar == TANDI_RESCUE_FREED && TandiIsBackInShadySands())
{
    rescueVar = TANDI_RESCUE_RETURNED;

    // Complete quest / give Aradesh reward.
}
```

This keeps **"Tandi rescued"** separate from **"Tandi returned and quest completed."**

---

# 15. Complete Original-Text Decision Tree

```text
TANDI
 |
 +-- "My name is [player], and your father sent me."
 |       |
 |       v
 |   "Father sent you!? Are you here to get me out?"
 |       |
 |       +-- "Yes, so just sit tight. I'll get you out."
 |       |       |
 |       |       v
 |       |   "Well, what's your plan?"
 |       |       |
 |       |       +-- "Just wait for the doors to open.
 |       |       |    Then run for it!"
 |       |       |       |
 |       |       |       v
 |       |       |    ESCAPE
 |       |       |
 |       |       +-- "I'm going to fight the Khan leader
 |       |            in exchange for your freedom."
 |       |               |
 |       |               v
 |       |             DUEL
 |       |
 |       +-- "No, he just sent me to look at you."
 |
 +-- other original responses
```

Then the duel route:

```text
DUEL
 |
 v
GARL
 |
 +-- "I'll fight you one-on-one for Tandi."
 |
 v
"Fine. We shall settle this with no weapons. Knock me out,
and I'll let you take the woman and leave. I knock you out,
you leave without her."
 |
 +-- "Fine."
 |
 v
ONE-ON-ONE COMBAT
 |
 +-- PLAYER WINS
 |      |
 |      v
 |   "Hmm, you fight better then I expected. I haven't
 |    had a good beating since my father slapped me
 |    around the room. You deserve the Shady Sands
 |    woman. Take her and leave. I wouldn't come back."
 |      |
 |      v
 |   TANDI_RESCUE_FREED
 |
 +-- PLAYER LOSES
        |
        v
     TANDI REMAINS CAPTIVE
```

Negotiation route:

```text
GARL
 |
 v
"Hmmm. Well, then. What do you offer for her release?"
 |
 +-- "I represent a threat you don't even understand.
 |    Do you really want to risk our wrath."
 |       |
 |       +-- success → "Hmmm. For some reason, I believe you.
 |       |             You may take the girl..."
 |       |             → TANDI_RESCUE_FREED
 |       |
 |       +-- failure → "Ha! You should not threaten your betters."
 |
 +-- "I will give you something of equal value."
 |       |
 |       v
 |   "It must be valuable indeed. Show me."
 |       |
 |       +-- sufficient value
 |       |      |
 |       |      v
 |       |   "It pleases me. You may take the girl,
 |       |    but do not return."
 |       |      |
 |       |      v
 |       |   TANDI_RESCUE_FREED
 |       |
 |       +-- insufficient value
 |              |
 |              v
 |           "Your bribe is not worth my time. Kill him."
 |
 +-- "It would be good for your eternal soul."
         |
         v
      "You think a man such as I believes in the afterlife?..."
```

---

# 16. Implementation Principle

The dialogue options and quest state should be kept separate.

**Dialogue choice:**

> "I'm going to fight the Khan leader in exchange for your freedom."

means:

```text
Player chose DUEL.
```

It does **not** mean:

```text
Tandi is rescued.
```

The successful duel response:

> "You deserve the Shady Sands woman. Take her and leave."

means:

```text
Garl has granted Tandi's release.
```

Then the actual game logic should make Tandi free.

Only after Tandi has actually been returned to Shady Sands should the overall quest be marked complete.
