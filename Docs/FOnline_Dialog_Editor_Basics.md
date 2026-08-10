# FOnline Dialog Editor — Seth Basic Reference

A practical reference for creating and connecting dialogs with the FOnline Dialog Editor, based on the working Seth dialog.

## 1. Basic idea

The Dialog Editor builds a **tree**:

```text
[1] Predialogue installations
    |
    +-- [2] NPC dialog
          |
          +-- [3] Player answer
                |
                +-- [4] Next NPC dialog
```

The important thing is the **tree connection**. A text string can exist in the file but still be unreachable if it is not connected to the correct node.

## 2. Main tabs

- **Options** — dialog/file options and saving.
- **Info** — information about the selected node.
- **Dialog** — NPC speech/dialog node.
- **Answer** — player's response.
- **Demand** — conditions controlling whether a branch is available.
- **Result** — actions performed after an answer is selected.
- **Strings** — dialog text.
- **Comment** — notes.

For a simple conversation, start with:

**Dialog → Answer → Dialog**

Do not add variables, scripts, Demands, or Results until the basic conversation works.

## 3. Predialogue installations

The top node is:

```text
[1] Predialogue installations
```

It is used to choose what happens when the player starts talking to the NPC.

The working Seth dialog has:

```text
[1] Predialogue installations
    |
    +-- [2] Begin
    |     Demand
    |       Script seth@d_IsNotArmed 0
    |
    +-- [12] Weapon check
          Demand
            Script seth@d_IsArmed 0
```

The first node must actually **continue into the tree**. Let the editor create the links rather than manually writing the generated syntax.

## 4. Basic NPC dialog

Create/select a **Dialog** node and enter the NPC's text.

Add an **Answer** under it, enter the player's response, and make that answer point to the next Dialog node.

Example:

```text
[2] Good day. I am called Seth. How can I help you?
    |
    +-- [3] I'd like some information.
            |
            v
        [4] Information? What do you want to know about?
```

This is the basic pattern:

**NPC Dialog → Player Answer → NPC Dialog**

## 5. Answers and exits

An answer can lead to another dialog:

```text
[2] Hello.
    |
    +-- [3] I'd like some information.
```

Or it can end the conversation:

```text
[2] Hello.
    |
    +-- [Exit] Goodbye.
```

Use an Exit when no further NPC response is required.

## 6. Multiple answers

A Dialog node can have several player answers.

Seth's information menu demonstrates:

```text
[3] Information? What do you want to know about?
    |
    +-- [5] I want to know about the Radscorpions.
    +-- [6] I want to know about the raiders.
    +-- [7] I want to know about this place.
    +-- [8] Take me to the Radscorpion caves.
```

Each answer can point to a different Dialog node.

## 7. Demands

A **Demand** is a condition.

The Seth dialog uses script Demands such as:

```text
[2] Begin
    Demand
        Script seth@d_IsNotArmed 0

[12] Weapon check
    Demand
        Script seth@d_IsArmed 0
```

Demands are useful for quest states, player variables, faction checks, scripts, and other conditions.

Add them only when a branch actually needs to be conditional.

## 8. Results

A **Result** performs an action after the player selects an answer.

Results can be used for game actions such as:

- changing a quest variable
- calling a script
- giving a reward
- changing another player value

For the Radscorpion quest, the intended pattern is:

```text
Player accepts quest
        |
        +-- Result: start quest
        +-- Script creates quest location
        +-- Player knows the location
        +-- quest state becomes 1
```

Get the ordinary dialog working first, then add Results.

## 9. Quest-state branching

For:

```text
0 = not started
1 = ongoing
2 = finished
```

use separate conditional branches:

```text
Predialogue
   |
   +-- state 0 --> starting dialog
   |
   +-- state 1 --> ongoing dialog
   |
   +-- state 2 --> completed dialog
```

Each state branch can have its own Demand.

## 10. Working Seth structure

```text
[1] Predialogue installations
    |
    +-- [2] Begin
    |     Demand: seth@d_IsNotArmed
    |
    +-- [12] Weapon check
          Demand: seth@d_IsArmed

[2] Good day. I am called Seth. How can I help you?
    |
    +-- [3] I'd like some information.
    |
    +-- [4] Bloop?

[3] Information? What do you want to know about?
    |
    +-- [5] Radscorpions
    +-- [6] Raiders
    +-- [7] This place
    +-- [8] Radscorpion caves

[8] Cave confirmation
    |
    +-- [10] Yes
    +-- [11] No

[10] Travel preparation
    |
    +-- [Exit] [Transport]

[11] Travel preparation
    |
    +-- [Exit] [End]

[12] Weapon warning
    |
    +-- [Exit] [Done.]
```

## 11. Do not hand-write the generated syntax

The generated `.fodlg` contains compact lines such as:

```text
2 2000 None 0 #
3 2010 #
4 2020 @
```

These are the generated representation of the tree.

When learning the editor, think in terms of:

```text
NPC dialog
    ↓
player answer
    ↓
next NPC dialog
```

rather than memorizing the generated syntax.

## 12. Safe workflow

### Step 1 — Minimal dialog

```text
[1] Predialogue
    |
    +-- [2] Hello.
          |
          +-- [Exit] Goodbye.
```

Test it.

### Step 2 — Add one answer leading to another NPC dialog

```text
[2] Hello. How can I help?
    |
    +-- [3] I have a question.

[3] What do you want to know?
```

Test it.

### Step 3 — Add more answers

Only after the basic conversation works.

### Step 4 — Add Demands

Use them for conditional branches.

### Step 5 — Add Results/scripts

Use them for actual game actions.

### Step 6 — Add quest-state branching

Only after ordinary dialog is confirmed working.

## 13. Debugging rule

If the NPC does not respond, simplify the tree.

Start with:

```text
Predialogue
    ↓
one NPC dialog
    ↓
Exit
```

If that works, add one Answer.

If that works, add the next Dialog.

If something breaks after adding a Demand or Result, the problem is in that newly added part.

Do not debug the entire quest at once.

## 14. The most important distinction

The editor's **tree** is the source of truth.

Text IDs such as:

```text
{2000}
{2010}
{3000}
```

are string IDs.

Tree numbers such as:

```text
[2]
[3]
[5]
[12]
```

are dialog-node links.

They are different things.

Build and connect the tree in the editor; let it generate the `.fodlg` syntax.
