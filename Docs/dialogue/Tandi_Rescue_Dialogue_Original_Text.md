# Tandi Rescue Dialogue — Original Text & Branch Structure

> Scope: **Rescue phase only.**
>
> All unrelated Tandi dialogue is intentionally excluded. The dialogue text below is kept exactly as provided in the source file.

---

## 1. Initial Rescue Encounter

### TAND_37 — Initial encounter

**NPC**

> "You? Who are you? What do you want?"

**Player branches**

- **164 + 165**

  > "My name is [player], and your father sent me."

  → **TAND_38**

- **166**

  > "I'm your fairy god-mother. What's it look like?"

- **167**

  > "Duh."

---

## 2. TAND_38 — Tandi realizes the player is here to rescue her

**NPC**

> "Father sent you!? Are you here to get me out?"

**Player branches**

- **169**

  > "Yes, so just sit tight. I'll get you out."

  → **TAND_39**

- **170**

  > "No, he just sent me to look at you."

---

## 3. TAND_39 — Rescue Plan

**NPC**

> "Well, what's your plan?"

**Player branches**

- **172**

  > "Just wait for the doors to open. Then run for it!"

  → **TAND_40**

- **173**

  > "I'm going to fight the Khan leader in exchange for your freedom."

  → **TAND_41**

---

## 4. TAND_40 — Door / Run Plan

**NPC**

> "That's it? That's your plan..."

This branch follows the player's plan to wait for the doors to open and escape.

---

## 5. TAND_41 — Fight the Khan Leader

**NPC**

> "You? Fight him? You got to be kidding me!"

This branch follows the player's plan to fight the Khan leader in exchange for Tandi's freedom.

---

# 6. Follow-up Rescue Conversation

## TAND_42

**NPC**

> "Oh, aren't we the sarcastic one? Are you going to get me out, or what!?"

**Player branches**

- **177**

  > "Yeah. Just hold on, and when you see the door open, run for it."

  → Continue rescue / escape.

- **178**

  > "I'm going to fight the Khan leader in exchange for your freedom."

  → Fight-the-leader rescue plan.

---

# 7. Final Rescue Interaction

## TAND_43

**NPC**

> "Oh, it's you again! Are you going to get me out?"

**Player branches**

- **180**

  > "Yes, so just sit tight. I'll get you out."

- **181**

  > "No, I'm just stopping by to say hi."

- **182**

  > "Duh."

---

## 8. Actual Escape Sequence

### tand_43a

**Tandi**

> "Hello, anybody home? Could you get me out? Could you GET ME OUT OF HERE!"

↓

### tand_43b

**Player / response**

> "This is no time to talk, we have to get out of here."

↓

### 208

> "Let's get outta here."

---

# Branch Overview

```text
TAND_37
"You? Who are you? What do you want?"
        |
        +-- 164/165
        |   "My name is [player], and your father sent me."
        |       |
        |       v
        |     TAND_38
        |     "Father sent you!? Are you here to get me out?"
        |           |
        |           +-- 169
        |           |   "Yes, so just sit tight. I'll get you out."
        |           |       |
        |           |       v
        |           |     TAND_39
        |           |     "Well, what's your plan?"
        |           |          |
        |           |          +-- 172
        |           |          |   "Just wait for the doors to open.
        |           |          |    Then run for it!"
        |           |          |       |
        |           |          |       v
        |           |          |     TAND_40
        |           |          |
        |           |          +-- 173
        |           |              "I'm going to fight the Khan leader
        |           |               in exchange for your freedom."
        |           |                  |
        |           |                  v
        |           |                TAND_41
        |           |
        |           +-- 170
        |               "No, he just sent me to look at you."
        |
        +-- 166
        |   "I'm your fairy god-mother. What's it look like?"
        |
        +-- 167
            "Duh."


Follow-up:
TAND_42
"Oh, aren't we the sarcastic one? Are you going to get me out, or what!?"
        |
        +-- 177
        |   "Yeah. Just hold on, and when you see the door open,
        |    run for it."
        |
        +-- 178
            "I'm going to fight the Khan leader in exchange
             for your freedom."


Final:
TAND_43
"Oh, it's you again! Are you going to get me out?"
        |
        +-- 180
        |   "Yes, so just sit tight. I'll get you out."
        |
        +-- 181
        |   "No, I'm just stopping by to say hi."
        |
        +-- 182
            "Duh."

        ↓

tand_43a
"Hello, anybody home? Could you get me out? Could you GET ME OUT OF HERE!"

        ↓

tand_43b
"This is no time to talk, we have to get out of here."

        ↓

208
"Let's get outta here."
```

---

# Message ID Reference

| Message | Purpose |
|---|---|
| `163 / TAND_37` | Initial rescue encounter |
| `164–167` | Initial player responses |
| `168 / TAND_38` | Tandi recognizes the rescue attempt |
| `169–170` | Confirm / deny rescue |
| `171 / TAND_39` | Tandi asks for the rescue plan |
| `172–173` | Rescue plan choices |
| `174 / TAND_40` | Door/run plan response |
| `175 / TAND_41` | Khan leader plan response |
| `176 / TAND_42` | Follow-up rescue conversation |
| `177–178` | Follow-up rescue choices |
| `179 / TAND_43` | Final rescue interaction |
| `180–182` | Final rescue choices |
| `tand_43a` | Tandi urgently demands escape |
| `tand_43b` | Escape response |
| `208` | Final escape line |

