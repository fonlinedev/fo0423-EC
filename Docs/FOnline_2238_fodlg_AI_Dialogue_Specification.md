# FOnline: 2238 Legacy Dialogue Specification
## `.fodlg` authoring, parsing, branching, conditions, results, scripts, lexems, and AI-generation rules

**Purpose:** This document is a strict authoring specification for generating `.fodlg` dialogue files for the **legacy FOnline: 2238 SDK/dialogue system** represented by the supplied sample files.

**Primary goal:** An AI should be able to use this document to generate dialogue files that preserve the legacy `.fodlg` grammar and the conventions actually used by the supplied server content, rather than inventing syntax from modern FOnline versions or from generic dialogue systems.

**Important scope rule:** The samples in this project are the authoritative examples for project-specific identifiers, script functions, variables, item IDs, faction IDs, skills, professions, and quest variables. This document distinguishes:
- **format rules** that can be inferred confidently from the files and the historical 2238 documentation;
- **project-specific APIs/identifiers** that must already exist in the server;
- **patterns that are observed but should not be invented without an existing example.**

---

# 1. The most important rule

A `.fodlg` file is **not** a modern JSON/XML/YAML dialogue definition and it is **not** an arbitrary text tree.

Do not invent syntax such as:

```text
[dialogue]
node=...
condition=...
choice=...
```

Do not invent braces around the dialogue tree.

Do not invent named node types such as `npc`, `player`, `condition`, `choice`, `goto`, or `end`.

The legacy format is a compact, line-oriented format produced by the FOnline 2238 DialogEditor. The `[dialog]` section is essentially a serialized directed dialogue graph.

A safe AI workflow is:

1. Start from a known-good `.fodlg` template.
2. Preserve the exact section structure.
3. Create unique numeric IDs.
4. Make every answer's first numeric field point to the intended dialogue node.
5. Put all conditions/results in the correct `D`/`R` position.
6. Use only identifiers and dialog functions that actually exist in the server.
7. Make every referenced text ID exist in `[engl]`.
8. Terminate every branch correctly with `@` or `&`.
9. Compile/test before assuming the dialogue is valid.

**Never replace a legacy syntax construct with a more intuitive one.**

---

# 2. File structure

A normal English-only dialogue has this general shape:

```text
[comment]
<optional human-readable comments>

[data]
lang=engl

[dialog]
&
<serialized dialogue graph>

[engl]
<message strings>
```

A multilingual file may instead have:

```text
[data]
lang=russ engl chin-simplified
```

followed by multiple language sections:

```text
[russ]
...

[engl]
...

[chin-simplified]
...
```

The supplied project samples are overwhelmingly English-only and use:

```text
[data]
lang=engl
```

### 2.1 `[comment]`

This is human-readable metadata/comments.

Example:

```text
[comment]
Caravan Informant - All Caravans Overview
```

It does not define dialogue behavior.

It is safe to put explanatory text here, but do not rely on comments to implement logic.

---

# 3. `[data]`

For the supplied project, the normal form is:

```text
[data]
lang=engl
```

The language list identifies the language trees represented in the file.

The historical 2238 documentation states that additional language trees can be created and that language identifiers are four-character identifiers in the editor workflow, although existing project files can contain the language naming conventions already used by the project.

**AI rule:** If the project is English-only, copy:

```text
[data]
lang=engl
```

Do not change `engl` to another invented value.

---

# 4. `[dialog]` is a graph, not a script in the normal programming-language sense

The dialogue graph is composed of:

- a **pre-dialog node**;
- **dialog/NPC text nodes**;
- **answer/player-choice nodes**;
- **demands/conditions** attached to answers;
- **results/actions** attached to answers;
- special terminal/flow markers;
- optional dialog functions attached to NPC text nodes.

The historical 2238 dialogue guide describes the same conceptual model as:

> node, answer, demand, and result.

The graph is serialized as compact numeric records.

---

# 5. The numeric ID convention

This is one of the most important parts of the format.

Typical NPC/dialogue nodes use IDs like:

```text
2000
3000
4000
5000
...
```

Their answer nodes use IDs immediately below the corresponding block:

```text
2010
2020
2030
2040

3010
3020
3030

4010
4020
4030
```

A useful mental model is:

```text
2000 = dialogue node 2
2010 = answer under dialogue node 2
2020 = another answer under dialogue node 2
2030 = another answer under dialogue node 2
2040 = another answer under dialogue node 2

3000 = dialogue node 3
3010 = answer under dialogue node 3
3020 = answer under dialogue node 3
...
```

The **first integer on each serialized line is the destination/dialogue target**, not the line's own ID.

Example:

```text
2 2000 None 0 #
3 2010 #
4 2020 #
5 2030 #
0 2040 @
```

This means approximately:

```text
Dialogue 2 (ID 2000)
    answer 2010 -> dialogue 3 (ID 3000)
    answer 2020 -> dialogue 4 (ID 4000)
    answer 2030 -> dialogue 5 (ID 5000)
    answer 2040 -> terminate
```

The destination value is therefore normally related to the target node's thousand-block:

```text
3 3010
```

means answer ID `3010` leads to node `3000`.

Likewise:

```text
4 4020
```

means answer ID `4020` leads to node `4000`.

And:

```text
0 4030
```

means the answer terminates instead of entering another dialogue node.

---

# 6. The canonical simple tree

The supplied `start_journey.fodlg` is an excellent minimal example:

```text
[dialog]
&
1 1000 None 0 #
2 1010 @

2 2000 None 0 #
3 2010 #
4 2020 #
5 2030 #
0 2040 @

3 3000 None 0 #
4 3010 #
5 3020 #
0 3030 @

4 4000 None 0 #
3 4010 #
5 4020 #
0 4030 @

5 5000 None 0 #
4 5010 #
3 5020 #
0 5030 &
```

The corresponding logical graph is:

```text
PRE-DIALOG
  |
  +--> dialogue 2 / 2000
          |
          +--> answer 2010 --> dialogue 3 / 3000
          |
          +--> answer 2020 --> dialogue 4 / 4000
          |
          +--> answer 2030 --> dialogue 5 / 5000
          |
          +--> answer 2040 --> END

dialogue 3 / 3000
          |
          +--> answer 3010 --> dialogue 4 / 4000
          +--> answer 3020 --> dialogue 5 / 5000
          +--> answer 3030 --> END
...
```

This is the safest basic structure for an AI to copy.

---

# 7. The pre-dialog node

The dialogue begins with a pre-dialog section.

Typical form:

```text
1 1000 None 0 #
2 1010 @
```

or, with conditional entry choices:

```text
1 1000 None 0 #
3 1010 D _script water_merchant@d_QuestNotStarted 0 #
4 1020 D _script water_merchant@d_QuestInProgress 0 #
5 1030 D _script water_merchant@d_QuestReadyToComplete 0 #
6 1040 D _script water_merchant@d_QuestCompleted 0 @
```

The first line establishes the pre-dialog node.

The following answer-like lines select which actual dialogue node is entered.

The historical 2238 guide explicitly describes the pre-dialog node as the first node executed when a conversation begins and explains that its child answers determine the initial state.

## 7.1 Default pre-dialog

Simple:

```text
1 1000 None 0 #
2 1010 @
```

Text:

```text
{1000}{}{Predialogue}
{1010}{}{Default}
```

This sends the conversation to node `2000`.

## 7.2 Conditional pre-dialog

Example:

```text
1 1000 None 0 #
3 1010 D _script water_merchant@d_QuestNotStarted 0 #
4 1020 D _script water_merchant@d_QuestInProgress 0 #
5 1030 D _script water_merchant@d_QuestReadyToComplete 0 #
6 1040 D _script water_merchant@d_QuestCompleted 0 @
```

The first matching/available route determines the initial dialogue state.

**AI rule:** Put more specific states before broader/default states when order matters.

Do not assume the engine performs arbitrary "best match" scoring. Treat pre-dialog entries as ordered conditional routes.

---

# 8. Dialogue node vs answer node

A normal dialogue node has an ID ending in `000`:

```text
2000
3000
4000
5000
```

Its text is the NPC's spoken line.

Its children normally have IDs ending in `010`, `020`, `030`, etc.:

```text
2010
2020
2030
2040
```

Those children are the player's answer choices.

Example:

```text
2 2000 None 0 #
3 2010 #
4 2020 #
0 2030 @
```

Conceptually:

```text
NPC: [text at 2000]

Player:
  [text at 2010] -> node 3000
  [text at 2020] -> node 4000
  [text at 2030] -> close
```

---

# 9. Answer IDs do not determine destination by themselves

Do not make the common mistake of assuming:

```text
2010 -> 2010
```

The first integer is the important destination selector:

```text
3 2010
```

means destination node 3 / ID `3000`.

Similarly:

```text
4 2020
```

means destination node 4 / ID `4000`.

This distinction is critical.

---

# 10. The four most important terminal/flow markers

The supplied files repeatedly use:

```text
#
@
&
```

and, separately, special numeric destinations.

The safest practical interpretation from the samples is:

### `#`

The current node continues as a graph branch and has child nodes/answers.

Example:

```text
2 2000 None 0 #
3 2010 #
4 2020 #
0 2030 @
```

### `@`

The current branch/dialogue path terminates at this point without continuing to another ordinary dialogue node.

Example:

```text
0 2040 @
```

This is the normal "close/end this answer path" pattern.

### `&`

The dialogue is at a final/end boundary.

Example:

```text
0 5030 &
```

The supplied project uses `&` for final dialogue termination in some cases.

**AI rule:** Do not casually substitute `#`, `@`, and `&`. Preserve the pattern of the template you are extending.

---

# 11. The `None 0` node-action fields

A normal NPC dialogue node commonly looks like:

```text
2 2000 None 0 #
```

Breakdown:

```text
2
```

destination/graph index associated with this record;

```text
2000
```

the node's text ID;

```text
None
```

no dialog function is attached to the node;

```text
0
```

zero function arguments;

```text
#
```

the graph continues into child answers.

For a plain NPC text node, this is the safest template.

---

# 12. Dialog functions attached to NPC text nodes

The historical 2238 guide documents special dialog functions that can be attached to **dialog nodes** to dynamically process text/lexems.

Example from the supplied files:

```text
3 3000 dialog@dlg_ShowHealingPrice 0 #
```

Another:

```text
99 11000 dialog@dlg_DisplayFactionPoints 0 #
```

These are not generic functions that an AI may invent.

They must exist in the server's dialogue script system.

## 12.1 Syntax

For a zero-argument dialog function:

```text
<target> <nodeId> <module>@<function> 0 <terminal>
```

Example:

```text
3 3000 dialog@dlg_ShowHealingPrice 0 #
```

For an answer demand/result function, the syntax is different:

```text
D _script <module>@<function> <argument-count> <args...>
```

or:

```text
R _script <module>@<function> <argument-count> <args...>
```

Do not confuse a **dialog text function** with a **demand function** or **result function**.

---

# 13. Demands (`D`)

`D` means a condition/demand.

A demand determines whether an answer or pre-dialog route is available.

Examples from the samples:

```text
D _var p faction_bos_quartermaster > 0
```

```text
D _param p ST_INTELLECT < 4
```

```text
D _item p PID_BOTTLE_CAPS > 2499
```

```text
D _script dialog@d_IsBusy 0
```

A player answer with a failed demand should not be available.

The historical 2238 guide explicitly states that multiple demands on the same answer are combined so that **all demands must be satisfied**.

Therefore:

```text
D condition_A
D condition_B
D condition_C
```

means approximately:

```text
condition_A AND condition_B AND condition_C
```

not OR.

---

# 14. Results (`R`)

`R` means an action/result that occurs when the answer is selected.

Examples:

```text
R _item p PID_FIRST_AID_BOOK + 1
```

```text
R _var p acquaintance = 1
```

```text
R _script water_merchant@r_StartQuest 0
```

```text
R _script dialog@r_RemoveMoney 1 1500
```

Multiple results on one answer execute as a group.

Example:

```text
R _item p PID_LASER_PISTOL + 1
R _item p PID_LEATHER_JACKET + 1
R _var p faction_bos_quartermaster = 2
```

Conceptually:

```text
give laser pistol
give leather jacket
set local variable
```

The historical 2238 guide explicitly states that multiple results attached to an answer are all executed when that answer is chosen.

---

# 15. Demands and results can appear on the same answer

This is extremely common and extremely important.

Example from the samples:

```text
8 7010 D _param p SK_ENERGY_WEAPONS } 50 R _item p PID_SMALL_ENERGY_CELL + 20 R _item p PID_LASER_PISTOL + 1 R _item p PID_LEATHER_JACKET + 1 R _var p faction_bos_quartermaster = 2 #
```

Logical interpretation:

```text
IF player skill Energy Weapons satisfies condition
THEN:
    give 20 small energy cells
    give 1 laser pistol
    give 1 leather jacket
    set faction_bos_quartermaster = 2
```

Do not split this into separate invented blocks.

---

# 16. Demand/result ordering

A serialized answer line may contain a sequence such as:

```text
D ...
D ...
R ...
R ...
R ...
```

The supplied files use this compact form repeatedly.

Safe rule:

```text
<destination> <answer-id>
    [zero or more D clauses]
    [zero or more R clauses]
    <terminal marker>
```

Do not insert arbitrary prose or JSON between clauses.

---

# 17. `_var`

The `_var` operator accesses a registered game variable.

Examples:

```text
D _var p faction_bos_quartermaster > 0
```

```text
D _var n name_mem_npc_player = 0
```

```text
R _var n name_mem_npc_player = 1
```

```text
R _var p q_mordinos_induction_doctor + 200
```

The variable name must exist in the server's variable definitions.

The supplied project contains variables such as:

```text
name_mem_npc_player
acquaintance
faction_bos_quartermaster
q_mordinos_joining_quest
q_redding_mcgrew_runner
q_mordinos_induction_doctor
factions_player_faction
factions_player_rank
```

These are **examples from this server**, not a complete universal variable list.

An AI must never invent a variable name because it "sounds right."

---

# 18. `_var` target/type letter

The samples use:

```text
p
n
```

after `_var`.

Examples:

```text
_var p faction_bos_quartermaster
_var n name_mem_npc_player
```

This distinction is meaningful in the legacy dialogue system and must be preserved.

The historical documentation describes:
- global variables;
- local variables;
- unique/unicum player-NPC variables.

The exact target/type letter used by a particular variable must follow the server's existing definitions and examples.

**AI rule:** Do not replace `p` with `n`, or vice versa, merely because the variable name sounds like it belongs to a player/NPC.

Copy the convention used by the variable's existing definition or a known-good dialogue.

---

# 19. `_param`

`_param` reads a critter parameter/stat/skill/etc.

Examples:

```text
D _param p ST_INTELLECT < 4
```

```text
D _param p ST_CHARISMA > 6
```

```text
D _param p SK_BARTER > 99
```

```text
D _param p PE_PROFESSION_DOCTOR < 2
```

The common form is:

```text
D _param p <PARAMETER> <OPERATOR> <VALUE>
```

or:

```text
R _param p <PARAMETER> <OPERATOR> <VALUE>
```

depending on the operation.

The samples demonstrate parameters including:

### SPECIAL
```text
ST_STRENGTH
ST_INTELLECT
ST_CHARISMA
ST_GENDER
```

### Skills
```text
SK_BARTER
SK_SCIENCE
SK_ENERGY_WEAPONS
SK_BIG_GUNS
SK_SMALL_GUNS
SK_MELEE_WEAPONS
SK_THROWING
```

### Perks/professions
```text
PE_PROFESSION_DOCTOR
PE_PROFESSION_DEMOLITION_EXPERT
```

### Other project parameters
```text
REPUTATION_JUNKTOWN
ST_EXPERIENCE
```

Again: these are identifiers, not strings to invent.

---

# 20. Operators

The samples demonstrate operators such as:

```text
=
<
>
}
```

Examples:

```text
ST_INTELLECT < 4
```

```text
faction_bos_quartermaster > 0
```

```text
PID_BOTTLE_CAPS > 2499
```

```text
SK_ENERGY_WEAPONS } 50
```

The legacy syntax is not necessarily visually identical to normal programming-language comparison syntax.

In particular, the sample:

```text
D _param p SK_ENERGY_WEAPONS } 50
```

must not automatically be rewritten to:

```text
D _param p SK_ENERGY_WEAPONS >= 50
```

even if that appears more intuitive.

**AI rule:** Copy the exact operator spelling used by the target server's existing dialogues and editor output.

---

# 21. `_item`

`_item` checks or changes inventory quantities.

Demand example:

```text
D _item p PID_BOTTLE_CAPS > 2499
```

This is used to test whether the player has enough caps.

Result example:

```text
R _item p PID_FIRST_AID_BOOK + 1
```

This adds one item.

Result example:

```text
R _item p PID_HEALING_POWDER + 4
```

This adds four healing powders.

The supplied files also use negative arithmetic through scripts for money removal, rather than inventing another syntax.

---

# 22. Caps / money

A common 2238 pattern is:

```text
D _item p PID_BOTTLE_CAPS > 1499
R _script dialog@r_RemoveMoney 1 1500
```

Notice the apparent off-by-one style:

```text
> 1499
```

for a cost of:

```text
1500
```

Likewise:

```text
D _item p PID_BOTTLE_CAPS > 2999
R _script dialog@r_RemoveMoney 1 3000
```

Do not "correct" these to a different operator unless you have verified the server's intended semantics.

For purchases, use the exact pattern from a known-good existing purchase dialogue.

---

# 23. `_script`

Script calls are represented in demand/result expressions as:

```text
D _script <module>@<function> <argument-count> [arguments...]
```

or:

```text
R _script <module>@<function> <argument-count> [arguments...]
```

Examples:

```text
D _script dialog@d_IsBusy 0
```

```text
D _script dialog@d_IsNotMemberOf 1 FACTION_BOS
```

```text
D _script dialog@d_CheckFactionPoints 2 FACTION_BOS 150
```

```text
R _script dialog@r_RemoveMoney 1 1500
```

```text
R _script dialog@r_AddFactionPoints 2 FACTION_BOS -150
```

```text
R _script water_merchant@r_StartQuest 0
```

---

# 24. Script argument counts are mandatory

This is one of the easiest ways for an AI to generate broken syntax.

Zero arguments:

```text
D _script dialog@d_IsBusy 0
```

One argument:

```text
R _script dialog@r_RemoveMoney 1 1500
```

Two arguments:

```text
D _script dialog@d_CheckFactionPoints 2 FACTION_BOS 150
```

The count must agree with the arguments that follow it.

Do not write:

```text
R _script dialog@r_RemoveMoney 1500
```

Do not write:

```text
R _script dialog@r_RemoveMoney 1
```

if the function requires the `1500` argument.

Do not write a function signature such as:

```text
r_RemoveMoney(int amount)
```

inside the `.fodlg`.

Only the call format belongs there.

---

# 25. Only use script functions that actually exist

The supplied samples use functions including:

```text
dialog@d_IsBusy
dialog@d_IsNotMemberOf
dialog@d_CheckFactionPoints
dialog@d_CheckProfession
dialog@d_CheckDoctor0
dialog@d_CheckDoctor1
dialog@d_CheckDemolitionExpert0
dialog@d_CheckDemolitionExpert1
dialog@d_WeaponInHand
dialog@d_CheckNight
dialog@d_IsGangMember
dialog@d_NotMemberOfAnyFaction
dialog@d_CanAfford
dialog@dlg_ShowHealingPrice
dialog@dlg_DisplayFactionPoints

dialog@r_RemoveMoney
dialog@r_AddFactionPoints
dialog@r_TeleportToMap
dialog@r_TeleportToMapFollowers

water_merchant@d_QuestNotStarted
water_merchant@d_QuestInProgress
water_merchant@d_QuestReadyToComplete
water_merchant@d_QuestCompleted
water_merchant@r_StartQuest
water_merchant@r_CompleteQuest

mine@d_CanAfford
mine@r_PurchaseMine
```

This is **not a universal API list**.

The AI must first search the project's `.fos`/header/script definitions or copy an existing invocation.

---

# 26. Dialog text functions and lexems

A dialog node can use a function to populate runtime text.

Example:

```text
3 3000 dialog@dlg_ShowHealingPrice 0 #
```

and the corresponding text:

```text
{3000}{}{I think I can cure what ails ya. It's gonna cost you @lex price@$ bucks.}
```

The function supplies the value for:

```text
@lex price@
```

The historical guide explicitly documents this pattern.

Another supplied example:

```text
99 11000 dialog@dlg_DisplayFactionPoints 0 #
```

with:

```text
{11000}{}{Alright, let's see....You have @lex fp_bos@ points remaining.}
```

The function supplies:

```text
@lex fp_bos@
```

**AI rule:** Never invent a lexem handler. A lexem is meaningful only if the associated dialog function/server implementation understands it.

---

# 27. Hard-coded lexems

The historical 2238 guide documents hard-coded lexems including:

```text
@sex@
```

and:

```text
@pname@
```

Example from the supplied `junktown_entrancegrd.fodlg`:

```text
{17000}{}{@sex@|We will shoot you, boy.||We will shoot you, lady.|}
```

The sex lexem uses the pipe-separated male/female text form.

Another supplied example from `nr_jules.fodlg`:

```text
{2000}{}{(Smiles, displaying a gleaming gold tooth. His cologne is so strong it almost chokes you.)@sex@|WELCOME to the glittering city of New Reno, my brother!||WELCOME to the glittering city of New Reno, my sister!|}
```

Do not replace this with a programming conditional.

---

# 28. The `@sex@|male||female|` pattern

The safe legacy form is:

```text
@sex@|male text||female text|
```

Example:

```text
@sex@|brother||sister|
```

Do not write:

```text
@if gender == male ...
```

Do not write:

```text
{male:...}{female:...}
```

Use the legacy lexem syntax.

---

# 29. Message string section `[engl]`

The dialogue graph does not normally contain the visible text directly.

Instead, the graph references numeric message IDs.

Example:

```text
2 2000 None 0 #
3 2010 #
4 2020 #
0 2030 @
```

and:

```text
{2000}{}{What do you need?}
{2010}{}{Tell me about this place.}
{2020}{}{Do you have work for me?}
{2030}{}{Nothing. Goodbye.}
```

The general message format is:

```text
{ID}{}{TEXT}
```

Historical documentation describes this message-string format as inherited from the original Fallout-style resource conventions.

---

# 30. Message ID uniqueness

Within a language tree, every active dialogue text ID must be unique.

Do not create:

```text
{2010}{}{First text}
{2010}{}{Second text}
```

The samples generally use unique IDs.

A safe numbering scheme is:

```text
1000     pre-dialog text
1010+    pre-dialog choices
2000     first NPC node
2010+    answers
3000     second NPC node
3010+    answers
4000     third NPC node
...
```

The numeric sequence is not merely cosmetic: it participates in graph routing.

---

# 31. The `{100}`, `{200}`, `{210}` records

Many dialogues contain:

```text
{100}{}{NPC Name}
{200}{}{Short description}
{210}{}{Long description}
```

These are NPC information/description strings, not normal dialogue nodes.

Example:

```text
{100}{}{Zaius}
{200}{}{You see a tough-looking old mutant.}
{210}{}{You see Zaius, the mine foreman.}
```

The `[dialog]` graph itself normally starts around:

```text
1000
```

Therefore do not try to create a graph node with ID `100` simply because the NPC name exists at `{100}`.

---

# 32. Description variants

Some samples contain additional description IDs such as:

```text
{240}{}{You see a dead guard of Junktown.}
{260}{}{You see a slain guard of Junktown.}
```

These are project/editor conventions for alternate NPC descriptions.

They are not normal dialogue graph nodes.

Do not add them unless the project already uses the corresponding engine/editor behavior.

---

# 33. Extra high-numbered text records

Some files contain message IDs such as:

```text
{100000001}{}{Welcome to Junktown, traveller!}
{100000002}{}{I'm sorry, the gates are closed for the night. Come back in the morning.}
{100000003}{}{Guys, get him!}
```

These are not normal `1000`, `2000`, `3000` dialogue nodes in the supplied graph.

They may be consumed by scripts or other server functionality.

**AI rule:** Do not assume every `[engl]` message is a dialogue-node text.

When generating normal dialogue, only generate text IDs that correspond to actual graph records unless a script integration explicitly requires an auxiliary message.

---

# 34. The `&` at the very beginning

Many files begin the `[dialog]` section with:

```text
[dialog]
&
1 1000 None 0 #
```

Preserve this.

Do not remove it because it looks like an empty statement.

It is part of the serialized dialogue format generated by the editor.

---

# 35. `None 1`, `None 0`, and other node fields

The first sample files commonly contain:

```text
None 0
```

but some contain:

```text
None 1
```

Example:

```text
1 1000 None 1 #
```

The second numeric value is related to the node's argument/script configuration.

**Do not normalize every node to `None 0`.**

If copying a known-good template that contains:

```text
None 1
```

preserve it.

The safest method is to generate these fields by copying the structure of an equivalent existing node produced by the same DialogEditor/version.

---

# 36. Special node actions

The supplied files show special values such as:

```text
Attack 0 @
```

Example:

```text
10 10000 Attack 0 @
```

with:

```text
{10000}{}{An Enclave spy! Kill him!}
```

This is not an ordinary NPC dialogue node with `None`.

It represents a special dialogue action.

The historical guide also documents special answer behavior such as:
- close dialogue;
- barter;
- attack.

**AI rule:** Do not invent special action names. Use only special action strings already supported by the project's DialogEditor/server.

---

# 37. `Attack`

Known sample:

```text
10 10000 Attack 0 @
```

This is a strong example of a special terminal/action node.

If the intended behavior is to initiate combat, use an existing known-good `Attack` record as the template.

Do not write:

```text
R Attack
```

or:

```text
D Attack
```

The sample shows `Attack` occupying the node-action field.

---

# 38. The first number can be greater than ordinary dialogue indexes

The samples contain:

```text
99
100
101
102
103
104
105
106
107
108
```

as destination/graph numbers.

Example:

```text
99 11000 dialog@dlg_DisplayFactionPoints 0 #
100 11010 #
```

and:

```text
100 12000 None 0 #
101 12010 #
102 12020 #
103 12030 #
104 12040 #
106 12050 #
108 12060 #
105 12070 #
0 12080 #
```

This is valid legacy graph serialization.

**Do not assume the first number must be between 0 and 20.**

---

# 39. Why node IDs can jump

A large dialogue such as `nr_jules.fodlg` can contain hundreds of records.

The project deliberately uses IDs like:

```text
52000
53000
54000
...
62000
63000
64000
```

The graph index may therefore grow substantially.

Do not renumber a large existing dialogue simply to make it "cleaner."

Renumbering changes destinations and can break the graph.

---

# 40. Existing dialogue IDs should be treated as immutable

When editing an existing dialogue:

**DO NOT** casually renumber:

```text
2000 -> 2100
3000 -> 4000
```

unless you update every destination and all language strings.

The first number on answer records is coupled to destination nodes.

A safe editing strategy is:

1. keep all existing IDs;
2. append new nodes in unused numeric blocks;
3. point new answers at those blocks;
4. add matching `[engl]` strings;
5. never reuse an existing ID.

---

# 41. Safe ID allocation

For a new dialogue, use a predictable sequence:

```text
1000     pre-dialog
1010+    pre-dialog choices

2000     NPC node 2
2010+    its answers

3000     NPC node 3
3010+    its answers

4000     NPC node 4
4010+    its answers
```

For example:

```text
5000
5010
5020
5030
```

If a node needs more than 9 answers, use the next available ten-step IDs as supported by the editor/project, but avoid collisions.

For large or heavily branched trees, allocate blocks deliberately.

---

# 42. Conditions on pre-dialog answers

A pre-dialog branch can use demands.

Example:

```text
3 1010 D _script dialog@d_QuestNotStarted 0 #
4 1020 D _script dialog@d_QuestInProgress 0 #
5 1030 D _script dialog@d_QuestReadyToComplete 0 #
6 1040 D _script dialog@d_QuestCompleted 0 @
```

This is ideal for state-driven quest dialogue.

Logical design:

```text
IF quest not started -> node 3000
ELSE IF quest in progress -> node 4000
ELSE IF quest ready -> node 5000
ELSE IF quest completed -> node 6000
```

Use this pattern rather than creating one enormous initial node full of mutually exclusive choices.

---

# 43. Quest-state dialogue pattern

The supplied `water_merchant.fodlg` is a very clean template:

```text
1 1000 None 0 #
3 1010 D _script water_merchant@d_QuestNotStarted 0 #
4 1020 D _script water_merchant@d_QuestInProgress 0 #
5 1030 D _script water_merchant@d_QuestReadyToComplete 0 #
6 1040 D _script water_merchant@d_QuestCompleted 0 @

3 2000 None 0 #
0 2010 R _script water_merchant@r_StartQuest 0 #
0 2020 @

4 3000 None 0 #
0 3010 @

5 4000 None 0 #
0 4010 R _script water_merchant@r_CompleteQuest 0 @

6 5000 None 0 #
0 5010 &
```

This pattern is recommended when a quest has explicit state transitions.

---

# 44. Quest transition safety

A quest-start result should happen on the answer that accepts the quest:

```text
0 2010 R _script water_merchant@r_StartQuest 0 #
```

A quest-completion result should happen on the answer that actually claims/completes the reward:

```text
0 4010 R _script water_merchant@r_CompleteQuest 0 @
```

Do not rely on a later goodbye node to perform a critical state transition if the player can escape the dialogue before clicking that goodbye.

The historical 2238 guide explicitly warns that quest rewards and final quest state should be performed together to avoid reward/XP exploits caused by forcing dialogue exit.

---

# 45. Relationship/memory variables

The historical documentation describes "unicum" variables that are associated with a particular player/NPC pair.

The supplied files commonly use:

```text
name_mem_npc_player
```

Example:

```text
2 1010 D _var n name_mem_npc_player = 0 R _var n name_mem_npc_player = 1 #
```

This can be used to distinguish:

```text
first conversation
```

from:

```text
already met
```

A common pattern is:

```text
IF name_mem_npc_player = 0
    -> first-time greeting
    -> set name_mem_npc_player = 1
ELSE
    -> returning-player greeting
```

Do not invent a new relationship variable if the server already has the appropriate one.

---

# 46. Example first-time/returning structure

From `all_mine_foreman.fodlg`:

```text
1 1000 None 0 #
2 1010 D _var n name_mem_npc_player = 0 R _var n name_mem_npc_player = 1 #
20 1020 @

20 2000 None 0 #
4 2010 #
0 2020 @
```

The key idea is:

- test memory;
- mark the relationship;
- route the first-time/default branch;
- use a different node for returning players.

---

# 47. Do not confuse NPC-player memory variables

The historical guide distinguishes relationship variables by direction, such as:

```text
name_mem_npc_player
```

versus a player-side counterpart.

The distinction matters because one belongs to a relationship perspective and the other to another critter/relationship context.

**AI rule:** If an existing NPC uses `name_mem_npc_player`, use it consistently. Do not replace it with a similarly named variable unless the project definitions require it.

---

# 48. Logical OR

The samples demonstrate explicit OR syntax.

Example:

```text
D _param p REPUTATION_JUNKTOWN < 500 D or D _script dialog@d_IsGangMember 0
```

and:

```text
D _param p REPUTATION_JUNKTOWN } 500 D or D _script dialog@d_NotMemberOfAnyFaction 0
```

This is an important warning:

**Multiple ordinary `D` conditions are AND.**

The project can explicitly encode OR using the legacy `D or D` construct.

Do not write modern:

```text
(conditionA || conditionB)
```

inside `.fodlg`.

Do not invent parentheses.

Copy the exact editor-generated pattern.

---

# 49. Multiple conditions

AND example:

```text
D _var n name_mem_npc_player = 0
D _var p q_mordinos_joining_quest = 1
```

This means both conditions must pass.

OR example:

```text
D conditionA D or D conditionB
```

The exact syntax is legacy/editor-generated and should be copied from an existing working example.

---

# 50. Inventory purchase pattern

A common safe purchase pattern is:

```text
71 10010 D _item p PID_BOTTLE_CAPS > 2499 R _script dialog@r_RemoveMoney 1 2500 R _item p PID_BLUEPRINT_STIMPAK + 1 #
```

The corresponding text might be:

```text
{10010}{}{I'd like Stimpak blueprint.}
```

The important sequence is:

```text
CHECK MONEY
REMOVE MONEY
GIVE ITEM
```

Do not give the item before the money check.

Do not remove money without a demand.

---

# 51. Faction-point purchase pattern

From `bos_quartermaster.fodlg`:

```text
107 13010 D _script dialog@d_CheckFactionPoints 2 FACTION_BOS 150 R _item p PID_10MM_JHP + 20 R _item p PID_10MM_PISTOL + 1 R _script dialog@r_AddFactionPoints 2 FACTION_BOS -150 #
```

Logical interpretation:

```text
IF BOS faction points >= required amount:
    give ammo
    give weapon
    subtract faction points
```

This is a strong template for currency-like faction purchases.

---

# 52. Skill-gated reward pattern

Example:

```text
8 7010 D _param p SK_ENERGY_WEAPONS } 50 R _item p PID_SMALL_ENERGY_CELL + 20 R _item p PID_LASER_PISTOL + 1 R _item p PID_LEATHER_JACKET + 1 R _var p faction_bos_quartermaster = 2 #
```

The answer is visible only when the required skill condition passes.

If the skill check fails, that answer should not be available.

---

# 53. No-condition result

Some answers intentionally have no demand and only results:

```text
0 7050 R _item p PID_LEATHER_JACKET + 1 R _item p PID_SPIKED_KNUCKLES + 1 R _var p faction_bos_quartermaster = 2 R _item p PID_HEALING_POWDER + 4 #
```

This is valid if the option is always available.

Do not add a fake `D True` condition.

---

# 54. Answer with no result

Ordinary answer:

```text
3 2010 #
```

This simply routes to node 3000.

No demand and no result.

This is the cleanest form for a normal conversational choice.

---

# 55. Answer that closes the dialogue

Typical:

```text
0 2020 @
```

The text might be:

```text
{2020}{}{Not interested right now.}
```

Use this for normal goodbye/leave answers.

---

# 56. Final goodbye answer

A final branch can use:

```text
0 5010 &
```

as seen in the water merchant sample.

Preserve the template when you want the final end marker.

---

# 57. Dynamic text: do not hard-code values unnecessarily

If a server already has a function such as:

```text
dialog@dlg_ShowHealingPrice
```

then the dialogue should use:

```text
@lex price@
```

rather than hard-code a number.

Likewise:

```text
@lex fp_bos@
```

should be used with the corresponding faction-point function.

This keeps displayed values synchronized with server logic.

---

# 58. Script module naming

A script call has:

```text
module@function
```

Examples:

```text
dialog@d_CheckNight
```

```text
water_merchant@r_StartQuest
```

```text
mine@r_PurchaseMine
```

The module name is not arbitrary.

The function must be exported/registered in a way the dialogue system can call.

**AI rule:** Never create:

```text
quest@start
npc@checkQuest
merchant@buy
```

unless those functions really exist.

---

# 59. `.fos` integration

The supplied project also contains `.fos` files such as:

```text
caravans.fos
```

and a compiled/binary-looking:

```text
overseer.fosb
```

The important relationship is:

```text
.fodlg
    |
    +--> calls server dialogue functions
             |
             +--> .fos implementation
```

For example:

```text
D _script caravans@d_CheckAllCaravans 1 0
```

or:

```text
R _script mine@r_PurchaseMine 0
```

means the dialogue depends on code in the relevant script module.

The dialogue file does not magically create those functions.

---

# 60. If an AI wants to add new behavior

There are two separate tasks:

### Task A: dialogue-only change

Use existing:

```text
D _var
D _param
D _item
R _var
R _param
R _item
```

or existing dialogue functions.

### Task B: new server behavior

If the required behavior does not exist, write/register a script function first, then call it from `.fodlg`.

Do not fake a script function in `.fodlg`.

---

# 61. Project identifiers must be verified

Before generating a condition/result, verify:

### Variables
Against the project's variable definitions.

### Parameters
Against `_defines.fos` or equivalent project definitions.

### Items
Against item PID definitions.

### Factions
Against faction definitions.

### Skills
Against parameter definitions.

### Script functions
Against `.fos` source/function declarations.

### Dialog IDs
Against the existing `.fodlg`.

This is more important than making the dialogue text clever.

---

# 62. Safe AI generation workflow

When an AI is asked:

> "Create a dialogue for NPC X"

the AI should internally perform this sequence.

## Step 1 — identify the target server

Assume **legacy FOnline: 2238 SDK format**, not modern FOnline Engine.

## Step 2 — inspect existing dialogue

Find a nearby dialogue with similar behavior.

Examples:

- simple conversation -> `start_journey.fodlg`
- quest state -> `water_merchant.fodlg`
- first-time/returning -> `all_mine_foreman.fodlg`
- skill-gated rewards -> `bos_quartermaster.fodlg`
- item/currency purchases -> `ncr_doctor_jubiley.fodlg`, `nr_renesco.fodlg`
- dynamic lexem -> `caravan_informant.fodlg`, `bos_quartermaster.fodlg`
- complex branches -> `nr_jules.fodlg`

## Step 3 — allocate graph IDs

Do not reuse IDs.

## Step 4 — build the graph first

Write the node/answer topology before writing prose.

## Step 5 — add conditions

Use only verified conditions.

## Step 6 — add results

Use only verified results.

## Step 7 — add script calls

Only if the function exists.

## Step 8 — add `[engl]` strings

Every graph text ID must have a corresponding string.

## Step 9 — verify destination routing

For every answer:

```text
<target> <answerID>
```

verify that the target node actually exists.

## Step 10 — verify termination

Every terminal branch must use the correct existing `@`/`&` pattern.

---

# 63. AI generation rule: never generate a partial graph

Bad approach:

```text
I'll write the dialogue text first and figure out IDs later.
```

This frequently produces broken routing.

Correct approach:

```text
1. Design graph.
2. Assign node IDs.
3. Assign answer IDs.
4. Assign destinations.
5. Add conditions/results.
6. Add strings.
```

---

# 64. AI generation rule: never invent syntax

Forbidden inventions include:

```text
IF ...
ELSE ...
GOTO ...
END
CHOICE ...
NODE ...
CONDITION ...
RESULT ...
SCRIPT(...)
```

unless they are inside comments or dialogue text.

The serialized dialogue section must use the legacy line syntax.

---

# 65. AI generation rule: never invent functions

Bad:

```text
D _script dialog@d_PlayerHasQuest 0
```

unless `d_PlayerHasQuest` actually exists.

Good:

```text
D _script water_merchant@d_QuestInProgress 0
```

because it exists in the supplied project.

---

# 66. AI generation rule: never invent variables

Bad:

```text
D _var p water_merchant_quest = 1
```

unless that variable exists.

Good:

```text
D _var p faction_bos_quartermaster > 0
```

because it appears in the supplied project.

---

# 67. AI generation rule: never invent item constants

Bad:

```text
R _item p PID_WATER_BOTTLE + 3
```

unless that PID is defined.

Good:

```text
R _item p PID_HEALING_POWDER + 4
```

if the server defines it.

---

# 68. AI generation rule: never assume modern operators

Bad:

```text
>=
<=
&&
||
!=
```

unless the actual 2238 editor/server format has been verified for that construct.

Use the exact operators demonstrated by the project.

---

# 69. AI generation rule: do not put conditions into prose

Bad:

```text
{2010}{}{[IF SK_BARTER > 75] I can give you a discount.}
```

That only displays the condition as text.

The condition belongs in the graph:

```text
D _param p SK_BARTER > 75
```

---

# 70. AI generation rule: do not put actions into prose

Bad:

```text
{3010}{}{[Give player 100 caps] Thanks!}
```

Instead:

```text
0 3010 R _item p PID_BOTTLE_CAPS + 100 @
```

with:

```text
{3010}{}{Thanks!}
```

The graph controls the action.

---

# 71. AI generation rule: text and graph are separate

The graph:

```text
3 2010 D _param p ST_CHARISMA > 6 #
```

and text:

```text
{2010}{}{I have a question for you.}
```

are two halves of the same record.

The graph line determines behavior.

The `[engl]` line determines displayed text.

---

# 72. AI generation rule: every graph text ID must exist

If `[dialog]` contains:

```text
3 3010 #
```

there must be:

```text
{3010}{}{...}
```

in `[engl]`.

Do not leave it undefined.

---

# 73. AI generation rule: every node target must exist

If an answer contains:

```text
7 4010 #
```

the graph must contain node:

```text
7 7000 ...
```

or the corresponding destination structure expected by the editor.

More simply:

```text
answer target = first field
target node ID = target * 1000
```

for ordinary numbered nodes.

Check this mechanically.

---

# 74. AI generation rule: terminal answers use target `0`

Typical:

```text
0 2040 @
```

or:

```text
0 5010 &
```

Do not point a goodbye answer to a nonexistent node such as:

```text
99 2040 @
```

unless the intention really is to route to node 99000.

---

# 75. AI generation rule: keep answer IDs in their parent block

For node:

```text
5000
```

prefer:

```text
5010
5020
5030
5040
```

not:

```text
8010
12010
```

unless you are deliberately following an existing graph structure.

This makes the graph maintainable and matches DialogEditor output.

---

# 76. AI generation rule: do not reuse IDs

If `5010` already exists, do not create another `5010`.

This applies to both:

```text
[dialog]
```

IDs and:

```text
[engl]
```

message IDs.

---

# 77. AI generation rule: don't renumber an existing dialogue

When adding a new branch to a 200-line dialogue, append unused blocks instead of renumbering the existing tree.

Example:

Existing:

```text
2000
3000
4000
...
62000
```

New:

```text
63000
64000
```

if those IDs are unused.

Then point the new answer to the correct target.

---

# 78. Complex-dialogue template

A safe generic structure is:

```text
[comment]
NPC dialogue

[data]
lang=engl

[dialog]
&
1 1000 None 0 #
2 1010 @

2 2000 None 0 #
3 2010 #
4 2020 #
0 2030 @

3 3000 None 0 #
5 3010 #
6 3020 #
0 3030 @

4 4000 None 0 #
7 4010 #
8 4020 #
0 4030 @

5 5000 None 0 #
0 5010 @

6 6000 None 0 #
0 6010 &

[engl]
{100}{}{NPC Name}
{200}{}{You see NPC Name.}
{210}{}{A longer description of NPC Name.}

{1000}{}{Predialogue}
{1010}{}{Default}

{2000}{}{Hello. What can I do for you?}
{2010}{}{Tell me about this place.}
{2020}{}{Do you have work?}
{2030}{}{Nothing. Goodbye.}

{3000}{}{This place is dangerous.}
{3010}{}{What kind of danger?}
{3020}{}{Do you have work?}
{3030}{}{I'll be going.}

{4000}{}{I may have work for someone useful.}
{4010}{}{What kind of work?}
{4020}{}{Tell me something else.}
{4030}{}{Goodbye.}

{5000}{}{Here's what I know.}
{5010}{}{Thanks. Goodbye.}

{6000}{}{Come back when you need me.}
{6010}{}{Goodbye.}
```

This is a **template**, not a universal drop-in file for every server.

---

# 79. Quest-state template

```text
[dialog]
&
1 1000 None 0 #
3 1010 D module@d_QuestNotStarted 0 #
4 1020 D module@d_QuestInProgress 0 #
5 1030 D module@d_QuestReady 0 #
6 1040 D module@d_QuestCompleted 0 @

3 2000 None 0 #
0 2010 R module@r_StartQuest 0 #
0 2020 @

4 3000 None 0 #
0 3010 @

5 4000 None 0 #
0 4010 R module@r_CompleteQuest 0 @

6 5000 None 0 #
0 5010 &
```

Replace `module@...` only with functions that actually exist.

---

# 80. Purchase template

```text
70 9000 None 0 #
71 9010 D _item p PID_BOTTLE_CAPS > 999 R _script dialog@r_RemoveMoney 1 1000 R _item p PID_SOME_DEFINED_ITEM + 1 #
0 9020 @
```

Before using this, verify:
- the PID exists;
- the money function exists;
- the desired price semantics match the server;
- the target graph number is correct.

---

# 81. Skill-gated purchase/reward template

```text
8 7010 D _param p SK_SOME_DEFINED_SKILL } 50 R _item p PID_SOME_DEFINED_ITEM + 1 R _var p SOME_DEFINED_VARIABLE = 2 #
```

Only use the `}` operator if the target server's editor/generated files use it for the intended comparison.

---

# 82. Script-only quest template

If a script owns the entire action:

```text
0 2010 R _script module@r_StartQuest 0 #
```

The script should perform the actual state mutation.

The dialogue remains responsible for:
- exposing the option;
- checking any demands;
- invoking the result.

---

# 83. Conditional answer template

```text
3 2010 D _var p some_existing_variable = 1 #
```

or:

```text
3 2010 D _param p ST_CHARISMA > 6 #
```

or:

```text
3 2010 D _item p PID_BOTTLE_CAPS > 999 #
```

or:

```text
3 2010 D _script dialog@some_existing_demand 0 #
```

---

# 84. Conditional answer with multiple AND conditions

```text
3 2010 D _var p quest_state = 2 D _param p ST_CHARISMA > 6 D _item p PID_BOTTLE_CAPS > 999 #
```

All three must pass.

Do not replace this with a prose explanation.

---

# 85. Conditional answer with result

```text
3 2010 D _item p PID_BOTTLE_CAPS > 999 R _script dialog@r_RemoveMoney 1 1000 R _item p PID_DEFINED_ITEM + 1 #
```

This is the standard "only show if affordable, then charge and reward" pattern.

---

# 86. Relationship greeting template

```text
1 1000 None 0 #
2 1010 D _var n name_mem_npc_player = 0 R _var n name_mem_npc_player = 1 #
20 1020 @

2 2000 None 0 #
...
```

A returning-player route can be represented by another pre-dialog answer targeting a different node.

Use the exact variable convention already used by the NPC/project.

---

# 87. Dynamic lexem template

Graph:

```text
3 3000 dialog@dlg_SomeExistingFunction 0 #
```

Text:

```text
{3000}{}{That will cost you @lex some_value@ caps.}
```

The script function must populate/replace the lexem.

---

# 88. Sex-specific template

```text
{3000}{}{@sex@|Welcome, brother.||Welcome, sister.|}
```

Do not alter the separator structure.

---

# 89. Returning to another branch

A dialogue can loop.

Example:

```text
3 2010 #
4 2020 #
```

and:

```text
3 3000 None 0 #
```

The answer under node 3000 can point back to node 4:

```text
4 3010 #
```

This is how many large dialogue trees create menus where the player can repeatedly ask about different subjects.

Loops are valid.

---

# 90. Menu-style dialogue

A menu can be built as:

```text
2 2000 None 0 #
3 2010 #
4 2020 #
5 2030 #
6 2040 #
0 2050 @
```

Then each answer routes to its own node.

A later answer can return to the menu:

```text
2 3010 #
```

where `2` means the `2000` node.

This pattern is common in merchant and information dialogues.

---

# 91. Merchant/menu example

The Brotherhood quartermaster demonstrates a nested menu:

```text
100 12000 None 0 #
101 12010 #
102 12020 #
103 12030 #
104 12040 #
106 12050 #
108 12060 #
105 12070 #
0 12080 #
99 12090 @
```

Then each category leads to another node.

This allows a large inventory tree without putting every purchase under one node.

---

# 92. Why the first number looks strange in large menus

For:

```text
100 12000 None 0 #
```

the target is node `10000`, not `12000`.

The line ID `12000` is the NPC text/answer block's own ID.

The first number `100` is the destination graph index.

Therefore:

```text
100 12000
```

means:

```text
answer/text record 12000 -> dialogue node 10000
```

This distinction is essential when reading raw `.fodlg`.

---

# 93. Never infer graph structure from text ID alone

For every line, inspect both:

```text
FIRST NUMBER
```

and:

```text
SECOND NUMBER
```

Example:

```text
100 12000
```

is not "node 10012000".

It is two separate fields.

---

# 94. Parsing model for an AI

An AI should mentally parse each `[dialog]` line as:

```text
<TARGET> <MESSAGE_ID> <PAYLOAD...> <FLOW_MARKER>
```

For ordinary nodes:

```text
<TARGET> <MESSAGE_ID> <ACTION> <ARG_COUNT> [ARGS...] <FLOW>
```

For answers:

```text
<TARGET> <MESSAGE_ID> [DEMANDS...] [RESULTS...] <FLOW>
```

This is a **conceptual parser model**, not a claim that the entire grammar can be safely represented by one universal EBNF rule. Some special node forms exist.

---

# 95. Recommended AI internal representation

Before writing the file, represent the dialogue internally as:

```text
Node 2:
    npc_text = 2000
    action = None
    answers:
        2010 -> node 3
        2020 -> node 4
        2030 -> node 5
        2040 -> END

Node 3:
    npc_text = 3000
    answers:
        3010 -> node 4
        3020 -> node 5
        3030 -> END
```

Only after the graph is correct should the AI serialize it to `.fodlg`.

This prevents the most common destination-ID mistakes.

---

# 96. Recommended validation algorithm

An AI or validation script should check:

## File-level
- `[comment]` exists or is intentionally absent.
- `[data]` exists.
- `lang=engl` exists for English dialogue.
- `[dialog]` exists.
- `[engl]` exists.

## Graph-level
- initial `&` exists where expected by the template.
- pre-dialog starts with node ID `1000`.
- all graph IDs are unique.
- all message IDs used by graph records exist in `[engl]`.
- all normal target indices resolve to actual dialogue nodes.
- all terminal branches use valid `0` destinations.
- no answer points to a nonexistent node.
- no duplicate message IDs exist.

## Script-level
- every `_script` module/function exists.
- argument counts match arguments.
- demand functions are used with `D`.
- result functions are used with `R`.
- dialog text functions are attached to dialog nodes, not mistakenly used as answer results.

## Data-level
- every `_var` variable exists.
- every `_param` parameter exists.
- every `PID_*` exists.
- every `FACTION_*` exists.
- every `SK_*`, `ST_*`, `PE_*` identifier exists.

---

# 97. Structural validator pseudocode

A useful conceptual validator is:

```text
read [dialog]

for each record:
    parse target
    parse message_id
    verify message_id is unique

    if record is an ordinary answer:
        if target == 0:
            verify terminal marker
        else:
            verify target * 1000 exists as a dialogue node

    scan every D clause:
        verify syntax
        verify referenced variable/param/item/script

    scan every R clause:
        verify syntax
        verify referenced variable/param/item/script

read [engl]

for every graph message_id:
    verify {message_id} exists

for every _script call:
    verify module/function exists
    verify argument count
```

A real parser should be more conservative than this pseudocode because special records exist.

---

# 98. Common failure #1: using modern FOnline syntax

Wrong:

```text
node 2000 {
    text = "Hello"
}
```

Correct legacy style:

```text
2 2000 None 0 #
```

and:

```text
{2000}{}{Hello}
```

---

# 99. Common failure #2: writing only `[engl]`

Wrong:

```text
[engl]
{2000}{}{Hello}
{2010}{}{Goodbye}
```

without a valid `[dialog]` graph.

The text alone does not create the conversation.

---

# 100. Common failure #3: writing a graph without strings

Wrong:

```text
2 2000 None 0 #
0 2010 @
```

with no:

```text
{2000}{}{...}
{2010}{}{...}
```

The graph references missing text.

---

# 101. Common failure #4: wrong destination

Wrong:

```text
2 2010 #
```

when the intention is to go to node 3000.

Correct:

```text
3 2010 #
```

---

# 102. Common failure #5: wrong answer target because of "obvious" numbering

Wrong reasoning:

```text
answer ID 3010 -> target 3010
```

Correct reasoning:

```text
answer ID 3010
first field 4
therefore -> dialogue node 4000
```

---

# 103. Common failure #6: using `>=` because it looks normal

Wrong unless verified:

```text
D _param p SK_BARTER >= 75
```

The samples use the legacy operator vocabulary, including `}`.

Use the exact operator generated by the server's DialogEditor.

---

# 104. Common failure #7: inventing a condition function

Wrong:

```text
D _script dialog@d_HasQuest 0
```

unless the function actually exists.

A missing script function can cause compile/runtime failure.

---

# 105. Common failure #8: forgetting the argument count

Wrong:

```text
R _script dialog@r_RemoveMoney 1500
```

Correct:

```text
R _script dialog@r_RemoveMoney 1 1500
```

---

# 106. Common failure #9: putting a result on the NPC node

Bad conceptual design:

```text
2 2000 R _item p PID_BOTTLE_CAPS + 100 #
```

A result should normally be attached to the answer that causes the action.

Example:

```text
0 2010 R _item p PID_BOTTLE_CAPS + 100 @
```

---

# 107. Common failure #10: giving rewards before checking payment

Wrong:

```text
R _item p PID_SOME_ITEM + 1
D _item p PID_BOTTLE_CAPS > 999
R _script dialog@r_RemoveMoney 1 1000
```

Use:

```text
D _item p PID_BOTTLE_CAPS > 999
R _script dialog@r_RemoveMoney 1 1000
R _item p PID_SOME_ITEM + 1
```

---

# 108. Common failure #11: relying on a later goodbye for quest completion

Bad:

```text
player accepts reward
    -> NPC reward dialogue
    -> player can escape
    -> goodbye node finally sets quest complete
```

This can create an incomplete state or exploit.

Prefer:

```text
player clicks completion/reward answer
    -> give reward
    -> set final quest state
```

in the same result sequence when appropriate.

---

# 109. Common failure #12: using a non-existent PID

Wrong:

```text
R _item p PID_MAGIC_SWORD + 1
```

unless the project defines it.

Use real PIDs from the server's item definitions.

---

# 110. Common failure #13: changing variable scope

Wrong:

```text
_var p name_mem_npc_player
```

when the existing project uses:

```text
_var n name_mem_npc_player
```

Do not infer scope from the variable's English name.

---

# 111. Common failure #14: missing terminal marker

An answer record should end with the correct flow marker.

Examples:

```text
3 2010 #
```

or:

```text
0 2020 @
```

Do not leave a record as:

```text
3 2010
```

---

# 112. Common failure #15: using prose comments inside `[dialog]`

Bad:

```text
2 2000 None 0 #  <-- NPC greeting
```

The parser expects the actual legacy record format.

Put human-readable comments in `[comment]`, not arbitrarily after graph records.

---

# 113. Common failure #16: duplicate IDs

Do not reuse:

```text
2000
2010
```

for a second branch.

Every message ID/graph record must have a unique identity within the file.

---

# 114. Common failure #17: forgetting special `[engl]` records

The NPC metadata:

```text
{100}{}{Name}
{200}{}{Short description}
{210}{}{Long description}
```

is separate from the graph.

A dialogue file can compile with graph IDs that start at `1000` even though the visible NPC metadata starts at `100`.

Do not "fix" this difference.

---

# 115. Common failure #18: assuming every `[engl]` ID must appear in `[dialog]`

The opposite is also false.

These are legitimate:

```text
{100}
{200}
{210}
```

and project-specific auxiliary messages.

Therefore the validation rule should be:

```text
Every graph message ID must exist in the language section.
```

not:

```text
Every language message ID must appear in the graph.
```

---

# 116. Common failure #19: replacing existing lexems with static text

If the server uses:

```text
@lex caravan_info@
```

do not replace it with invented static output unless requested.

The lexem is deliberately dynamic.

---

# 117. Common failure #20: generating a "cleaned up" version of a working file

Do not reformat a working `.fodlg` into:

```text
pretty indentation
```

if the goal is server compatibility.

The DialogEditor serialization is intentionally compact.

Preserve the exact line-oriented style.

---

# 118. Common failure #21: copying modern FOnline documentation

The public modern FOnline Engine documentation describes newer engine systems and internal formats, but this project uses the **legacy FOnline: 2238 DialogEditor format**.

A modern engine document must not override the syntax demonstrated by the supplied `.fodlg` samples.

Use legacy 2238 samples as the primary syntax reference.

---

# 119. Known-good project samples and what each teaches

## `start_journey.fodlg`
Best minimal branching example.

Teaches:
- pre-dialog;
- ordinary NPC node;
- answer routing;
- terminal answers;
- clean numeric progression.

## `caravan_informant.fodlg`
Best minimal dynamic script/lexem example.

Teaches:
- script demand;
- dialog function;
- `@lex caravan_info@`.

## `water_merchant.fodlg`
Best clean quest-state example.

Teaches:
- state-based pre-dialog;
- quest start result;
- quest completion result;
- clean state routing.

## `all_mine_foreman.fodlg`
Best relationship-memory example.

Teaches:
- `name_mem_npc_player`;
- first-time/returning logic;
- script-backed purchase.

## `bos_quartermaster.fodlg`
Best complex reward/menu example.

Teaches:
- faction membership demand;
- nested menus;
- skill gates;
- item rewards;
- faction-point costs;
- dynamic faction-point lexem;
- attack action.

## `ncr_doctor_jubiley.fodlg`
Best medical/profession/currency/blueprint example.

Teaches:
- profession conditions;
- money checks;
- multiple prices;
- dialog text function;
- blueprint rewards.

## `nr_renesco.fodlg`
Best complex legacy tree example.

Teaches:
- many branches;
- variables;
- skill/attribute conditions;
- quest variables;
- dynamic/persistent state;
- nested menus;
- item purchases.

## `nr_jules.fodlg`
Best very large dialogue example.

Teaches:
- extensive branching;
- gender-dependent text;
- science/barter gates;
- item purchasing;
- large ID space.

## `junktown_entrancegrd.fodlg`
Best example of:
- night check;
- weapon-in-hand check;
- reputation;
- OR conditions;
- attack;
- teleport scripts;
- sex lexem;
- special auxiliary messages.

---

# 120. How to make an AI "copy the dialect"

When asking another AI to generate a dialogue, explicitly tell it:

```text
This is a legacy FOnline: 2238 SDK .fodlg file.

Treat the supplied existing .fodlg files as executable syntax examples.

Do not use modern FOnline Engine dialogue syntax.

Do not invent variables, PIDs, script functions, factions, skills, operators, or node types.

Preserve the exact:
[data]
[dialog]
[engl]
structure.

Use numeric graph IDs where ordinary nodes are N000 and answers are N010/N020/etc.

The first numeric field is the destination graph index.

The second numeric field is the message/record ID.

Use D for demands and R for results.

Use the exact legacy _var, _param, _item and _script syntax shown in the examples.

For script calls, include the exact argument count.

Every graph message ID must have a matching language message.

Do not renumber existing IDs.

Use existing dialogue files as templates for special constructs.

If a requested behavior requires a script function that does not exist, say so instead of inventing one.

Return a complete .fodlg file, not pseudo-code.
```

This prompt dramatically reduces the chance of generic/modern syntax contamination.

---

# 121. Recommended AI instruction for unknown functionality

If the AI does not know whether a function/variable/PID exists, it should respond:

```text
I cannot safely generate this reference without the server definition.
Please provide the relevant .fos/_defines/_vars/item definition or an existing
dialogue that uses the intended behavior.
```

It should **not** guess.

---

# 122. "No hallucination" policy for `.fodlg`

For server-safe generation, treat these as closed-world namespaces:

```text
Variables
Parameters
Items
Factions
Script modules
Script functions
Dialog special actions
Lexems
```

If an identifier is not verified, it is **unknown**.

Unknown does not mean "probably valid."

---

# 123. Recommended source hierarchy

When there is a conflict, use this priority:

## Priority 1
A known-good `.fodlg` from the exact target server.

## Priority 2
The project's actual `.fos` / headers / `_defines.fos` / variable definitions / item definitions.

## Priority 3
The historical FOnline: 2238 DialogEditor documentation.

## Priority 4
Other FOnline: 2238 examples.

## Priority 5
Generic FOnline/Fallout knowledge.

## Priority 6
Modern FOnline Engine documentation.

Generic or modern knowledge should never override an exact legacy sample from the target server.

---

# 124. Minimal "golden" dialogue

If all you need is a guaranteed basic shape to start from:

```text
[comment]
Simple NPC dialogue

[data]
lang=engl

[dialog]
&
1 1000 None 0 #
2 1010 @
2 2000 None 0 #
3 2010 #
0 2020 &

3 3000 None 0 #
0 3010 &

[engl]
{100}{}{NPC Name}
{200}{}{You see NPC Name.}
{210}{}{A description of NPC Name.}

{1000}{}{Predialogue}
{1010}{}{Default}

{2000}{}{Hello. What can I do for you?}
{2010}{}{Tell me about yourself.}
{2020}{}{Goodbye.}

{3000}{}{There isn't much to tell.}
{3010}{}{Thanks. Goodbye.}
```

Before deployment, verify the exact `@`/`&` ending style against an existing working file from the target server.

---

# 125. Practical graph-design rules

For maintainability:

1. One topic per NPC node.
2. Keep menu answers short.
3. Use dedicated nodes for detailed answers.
4. Return to a central menu when appropriate.
5. Use conditions to hide unavailable options.
6. Put state-changing results on the answer that causes the state transition.
7. Keep purchase checks and payment/removal together.
8. Avoid giant single nodes with dozens of unrelated answers.
9. Avoid unnecessary scripts.
10. Prefer existing engine/dialogue functions over new scripts when they already solve the problem.

---

# 126. Practical quest-design rules

For quest dialogues:

```text
PRE-DIALOG
    |
    +-- NOT STARTED
    |       |
    |       +-- accept -> START QUEST
    |
    +-- IN PROGRESS
    |       |
    |       +-- status
    |
    +-- READY
    |       |
    |       +-- claim -> REWARD + COMPLETE
    |
    +-- COMPLETED
            |
            +-- already done
```

This is more reliable than trying to infer quest state from the visible text.

---

# 127. Practical economy-design rules

For purchases:

```text
condition: enough currency
        |
        v
result: remove currency
        |
        v
result: give item
```

For faction points:

```text
condition: enough faction points
        |
        v
result: give reward
        |
        v
result: subtract faction points
```

For skill-gated rewards:

```text
condition: required skill
        |
        v
result: reward
        |
        v
result: set progression variable
```

---

# 128. Never use dialogue text as state

Bad:

```text
If NPC says "I already gave it to you", assume quest complete.
```

State belongs in:

```text
_var
```

or:

```text
_script
```

not in natural language.

---

# 129. Never use player-visible choices as security

A choice such as:

```text
I'll take the reward.
```

does not itself prove the player is eligible.

The answer should have a demand or script validation.

Example:

```text
D _script module@d_QuestReady 0
R _script module@r_CompleteQuest 0
```

---

# 130. Reusable AI generation checklist

Before returning a `.fodlg`, the AI must answer "yes" to all applicable items:

### File
- [ ] Correct `[comment]`.
- [ ] Correct `[data]`.
- [ ] Correct language section.
- [ ] `[dialog]` present.
- [ ] `[engl]` present.
- [ ] Initial `&` preserved.

### IDs
- [ ] `1000` pre-dialog exists.
- [ ] No duplicate graph IDs.
- [ ] No duplicate text IDs.
- [ ] Every graph text ID exists in language section.
- [ ] Every nonzero destination resolves.
- [ ] Existing IDs were not renumbered.

### Conditions
- [ ] Every `D` is valid.
- [ ] Every variable exists.
- [ ] Every parameter exists.
- [ ] Every item PID exists.
- [ ] Every script demand exists.
- [ ] AND/OR semantics are intentional.

### Results
- [ ] Every `R` is valid.
- [ ] Every result function exists.
- [ ] Script argument counts are correct.
- [ ] Money is checked before removal.
- [ ] Rewards are granted only after eligibility.

### Scripts
- [ ] Module exists.
- [ ] Function exists.
- [ ] Correct argument count.
- [ ] Demand function used with `D`.
- [ ] Result function used with `R`.
- [ ] Dialog text function attached to a dialog node.

### Text
- [ ] Every text is inside `[engl]`.
- [ ] No accidental missing braces.
- [ ] No invented parser syntax.
- [ ] Lexems use known forms.
- [ ] `@sex@` syntax is exact when used.

### Flow
- [ ] Every normal answer points to the intended node.
- [ ] Goodbye answers use `0`.
- [ ] Terminal markers are correct.
- [ ] No accidental dead-end nodes.
- [ ] No accidental infinite loop unless intended.

---

# 131. Recommended "AI output contract"

When another AI is generating a dialogue for this server, require this exact behavior:

```text
OUTPUT CONTRACT

1. Output a complete legacy FOnline: 2238 .fodlg file.
2. Do not output pseudo-code.
3. Do not output JSON/YAML/XML.
4. Do not modernize the syntax.
5. Do not invent server identifiers.
6. Preserve existing IDs when modifying a file.
7. Allocate new IDs without collisions.
8. Verify every answer destination.
9. Verify every message ID.
10. Verify every D/R expression.
11. Verify every script function and argument count when definitions are available.
12. If a required identifier/function is unknown, mark it as an unresolved dependency instead of hallucinating it.
13. Keep all dialogue text in the language section.
14. Keep behavior in `[dialog]`.
15. Use the supplied project dialogues as templates for special constructs.
```

---

# 132. Recommended AI modification contract

When editing an existing `.fodlg`:

```text
MODIFICATION CONTRACT

- Do not rewrite the entire file unless necessary.
- Preserve all existing graph IDs.
- Preserve all existing text IDs.
- Preserve all existing scripts.
- Preserve all existing special actions.
- Add new nodes in unused ID blocks.
- Update only the branches required for the new behavior.
- Add every new text string to [engl].
- Do not change unrelated dialogue.
- Do not replace working legacy syntax with cleaner-looking syntax.
```

---

# 133. Recommended AI debugging contract

If a dialogue fails to parse:

```text
DEBUG IN THIS ORDER

1. Compare [data] and language section names.
2. Check [dialog] header and initial &.
3. Check every line's numeric fields.
4. Check for missing terminal marker.
5. Check duplicate IDs.
6. Check target references.
7. Check [engl] message IDs.
8. Check D/R syntax.
9. Check script argument counts.
10. Check script function existence.
11. Check variables/PIDs/parameters.
12. Compare the failing line byte-for-byte in structure with a known-good sample.
```

Do not immediately rewrite the whole file.

---

# 134. Why "copy an existing file" is often safer than generating from scratch

The historical 2238 documentation itself recommends using an existing dialogue as a template.

That is especially important for this legacy format because:
- some serialized fields are not self-explanatory;
- special node actions exist;
- graph IDs have routing semantics;
- language IDs include metadata/auxiliary strings;
- the server has project-specific functions and variables.

Therefore the safest AI behavior is:

```text
find closest existing dialogue
        |
        v
copy structural pattern
        |
        v
replace text
        |
        v
replace verified identifiers
        |
        v
validate
```

not:

```text
invent an entire .fodlg grammar from memory
```

---

# 135. What this specification does NOT claim

This document intentionally does not claim that every possible 2238 `.fodlg` construct has been reverse-engineered from the supplied files.

In particular, do not assume this document exhaustively defines:
- every DialogEditor special action;
- every legacy operator;
- every multi-language serialization detail;
- every special node type;
- every editor-only metadata field;
- every server-specific script function.

For those cases, the exact existing `.fodlg` produced by the same server/editor is authoritative.

---

# 136. The safest possible principle

If two rules conflict:

```text
"this seems logical"
```

and:

```text
"the existing working .fodlg does it this way"
```

choose:

```text
THE EXISTING WORKING .FODLG
```

The goal is parser/server compatibility, not aesthetic elegance.

---

# 137. Final compact reference

## Sections

```text
[comment]
[data]
[dialog]
[language]
```

## Normal node

```text
<TARGET> <ID> None 0 #
```

## Node with dialog text function

```text
<TARGET> <ID> module@function 0 #
```

## Demand

```text
D _var ...
D _param ...
D _item ...
D _script module@function N ...
```

## Result

```text
R _var ...
R _param ...
R _item ...
R _script module@function N ...
```

## Normal route

```text
3 2010 #
```

## Close

```text
0 2020 @
```

## Final end

```text
0 5030 &
```

## Message

```text
{2010}{}{Visible text}
```

## NPC metadata

```text
{100}{}{NPC Name}
{200}{}{Short description}
{210}{}{Long description}
```

## Dynamic lexem

```text
@lex name@
```

with an appropriate existing dialog text function.

## Sex lexem

```text
@sex@|male text||female text|
```

## Player name

```text
@pname@
```

---

# 138. Final "do not generate garbage" rule set

An AI generating FOnline 2238 dialogues must obey all of these:

```text
DO:
- use the legacy .fodlg format;
- copy known-good samples;
- preserve [comment]/[data]/[dialog]/[engl];
- use numeric graph IDs;
- use N000 dialogue nodes;
- use N010/N020/... answer records;
- understand the first numeric field as the destination graph index;
- use D for demands;
- use R for results;
- use verified _var/_param/_item/_script constructs;
- use exact script argument counts;
- use existing lexems/functions;
- keep message IDs synchronized;
- preserve existing IDs during edits;
- validate every destination;
- validate every dependency;
- use the same syntax as the project's working dialogues.

DO NOT:
- use JSON;
- use YAML;
- use XML;
- use modern dialogue syntax;
- invent `IF`, `ELSE`, `GOTO`, `CHOICE`, `NODE`, or `END`;
- invent variables;
- invent item PIDs;
- invent factions;
- invent script functions;
- invent operators;
- invent special actions;
- invent lexem handlers;
- change `p`/`n` scope markers casually;
- change existing IDs;
- assume every `[engl]` message is a graph node;
- assume every graph node has a matching `{1000}`-style metadata entry;
- put game logic into visible dialogue text;
- give rewards without eligibility checks;
- remove currency without a preceding check;
- leave a graph target unresolved;
- omit the terminal marker;
- modernize working syntax because it looks ugly.
```

---

# 139. Source/reference basis

This specification was built from:

1. The supplied project `.fodlg` files, including:
   - `start_journey.fodlg`
   - `caravan_informant.fodlg`
   - `bos_doctor.fodlg`
   - `bos_quartermaster.fodlg`
   - `q_vault13_overseer.fodlg`
   - `tanner.fodlg`
   - `doctor.fodlg`
   - `bh_marcus.fodlg`
   - `water_merchant.fodlg`
   - `all_mine_foreman.fodlg`
   - `bh_steve.fodlg`
   - `junktown_entrancegrd.fodlg`
   - `ncr_rondo.fodlg`
   - `hub_jacob.fodlg`
   - `ncr_doctor_jubiley.fodlg`
   - `nr_renesco.fodlg`
   - `bh_zaius.fodlg`
   - `nr_jules.fodlg`

2. The supplied `caravans.fos` and `overseer.fosb` files for script-integration context.

3. **The Art of Dialog Creation for FOnline: 2238**, Ghosthack, Revision 1, 2010-07-05. This historical guide documents the DialogEditor workflow, pre-dialog/dialog/answer/demand/result concepts, variables, dialog functions, lexems, naming conventions, and quest-reward safety rules.

4. The historical FOnline: 2238 SDK/forum material describing the legacy SDK structure and `.fodlg` files.

5. Modern FOnline Engine documentation was treated only as background context and **not** as the authoritative grammar for these legacy dialogues.

---

# 140. One-sentence rule for another AI

If this entire document must be reduced to one instruction:

> **Generate the dialogue exactly as the legacy FOnline: 2238 DialogEditor would serialize it, using an existing working `.fodlg` from this server as the syntax authority, and never invent a variable, PID, function, operator, special action, or graph construct that has not been verified in the server.**


---

# 138. Verified Seth / runtime debugging findings (2026-08-08)

This section records findings that were **actually tested against the supplied 2238 server/editor**, rather than inferred from syntax alone. These findings are especially important for AI generation.

## 138.1 `dialogs.lst` is the registration source for the Mapper dialogue ID

For Seth, the project contains:

```text
$ 4010 seth
```

and the Mapper NPC uses:

```text
ST_DIALOG_ID = 4010
```

This combination is correct.

**Do not confuse these two kinds of numbers:**

```text
4010
```

is the **registered NPC dialogue ID** from `dialogs.lst`.

It is **not** the same thing as a `.fodlg` graph node/message ID.

The verified chain is:

```text
Mapper ST_DIALOG_ID 4010
        |
        v
dialogs.lst
4010 -> seth
        |
        v
seth.fodlg
```

The NPC does not need a separate Mapper path to `seth.fodlg` when this normal project registration is used.

---

## 138.2 A minimal Seth dialogue was verified working

The following structure was tested successfully in-game:

```text
[dialog]
&
1 1000 None 0 #
3 1010 @
3 2000 None 0 #
0 2010 &
```

with:

```text
{100}{}{Seth}
{200}{}{You see Seth, leader of the guards of Shady Sands.}
{1000}{}{Predialogue installations}
{1010}{}{Talk to Seth}
{2000}{}{Good day. I am called Seth. How can I help you?}
{2010}{}{Goodbye.}
```

This is now a **known-good baseline** for this server.

It proves all of the following:

- `ST_DIALOG_ID = 4010` works.
- `dialogs.lst` registration works.
- `seth.fodlg` is loaded.
- `{100}` and `{200}` are valid NPC metadata entries.
- the basic pre-dialogue route works.
- the `2000/2010` dialogue/answer pair works.
- the `@`/`&` flow shown above works.

When developing a larger Seth dialogue, keep a copy of this file as a rollback baseline.

---

## 138.3 `D _script` boolean conditions can be runtime-tested independently

A server-side function was written to test whether the player has a weapon equipped in either hand:

```cpp
bool d_IsArmed(Critter& player, Critter@ npc)
{
    Item@ hand1 = player.GetItem(0, SLOT_HAND1);
    if(valid(hand1) && hand1.GetType() == ITEM_TYPE_WEAPON) return true;

    Item@ hand2 = player.GetItem(0, SLOT_HAND2);
    if(valid(hand2) && hand2.GetType() == ITEM_TYPE_WEAPON) return true;

    return false;
}
```

The important verified behavior was:

```text
weapon equipped
    -> d_IsArmed() returns true
    -> the armed dialogue branch is reached
```

and:

```text
no weapon equipped
    -> d_IsArmed() returns false
    -> the dialogue engine attempts the false-condition link
```

This proves that the script condition itself was executing. The subsequent runtime failure was a **graph-link problem**, not evidence that the script was not called.

### AI rule

When adding a boolean `D _script` condition:

1. First verify that the script compiles.
2. Use a minimal dialogue with two clearly different outcomes.
3. Test both true and false states.
4. Only after both routes work should the condition be combined with quest logic.

This is substantially safer than adding a condition to a large quest dialogue immediately.

---

## 138.4 Runtime error: `Dialog_Begin ... link<3> not found`

A test produced:

```text
FOServer::Dialog_Begin - Dialog from link<3> not found, client<narg>, dialog pack<4010>.
```

It occurred **only when the player had no weapon equipped**.

This was a crucial diagnostic result.

It means the condition had already been evaluated and the server was attempting to follow:

```text
link 3
```

The problem was therefore the **destination graph**, not the weapon-check function.

### Correct interpretation

The first numeric field in a dialogue record is a **graph destination/index**, while the second numeric field is the **message/record ID**.

For example:

```text
3 2010 #
```

means:

```text
answer/message 2010
    -> destination node/index 3
    -> corresponding dialogue text block is 3000
```

It does **not** mean:

```text
2010 -> node 2010
```

This distinction is fundamental.

---

## 138.5 Do not invent a destination number independently of the message block

A common failed construction was effectively treating records such as:

```text
20 2000 None 0 #
```

as though `20` were simply a freely chosen node identifier.

That is incorrect.

The supplied working files demonstrate the relationship:

```text
2 2000 None 0 #
3 2010 #
4 2020 #
```

where:

```text
2000 -> dialogue block 2
2010 -> answer whose destination is 3
2020 -> answer whose destination is 4
```

The target/index must agree with the intended thousand-block.

Therefore:

```text
2 2000
```

is coherent.

But:

```text
20 2000
```

is not a valid way to say "node 20 with text 2000"; the two fields describe different things.

### AI generation rule

Before emitting a record, calculate:

```text
message ID  = N000 / N010 / N020 ...
destination = intended dialogue block number
```

For an ordinary dialogue block:

```text
<N> <N000> None 0 #
```

For an answer leading to dialogue block `M`:

```text
<M> <N010> #
```

For a terminal answer:

```text
0 <N010> @
```

Do not choose the first field merely because the number is unused.

---

## 138.6 A particularly important correction: "duplicate node IDs" was an imprecise diagnosis

The earlier debugging discussion incorrectly described some failures as duplicate "node IDs."

The more precise model is:

- the **second number** is the message/record ID;
- the **first number** is the graph destination/index;
- dialogue text blocks normally use `N000`;
- answer records normally use `N010`, `N020`, etc.;
- an answer's first number selects the destination dialogue block.

Therefore a repeated first field is not automatically a duplicate-ID error.

For example, this is normal:

```text
3 1010 @
3 2000 None 0 #
```

The `3` in both records does **not** mean that the node was declared twice.

They are two different records involving destination/index `3`.

What matters is whether the message IDs and graph relationships form a coherent structure.

---

## 138.7 The working complex example confirms that high destination/index values can exist

The supplied Vault 13 Overseer dialogue contains:

```text
1 1000 None 0 #
2 1010 D _script overseer@d_IsLowStat 0 #
3 1020 D _script overseer@d_IsQuestNotStarted 0 #
10 1030 D _script overseer@d_IsQuestStarted 0 #
20 1040 D _script overseer@d_IsQuestCompleted 0 @
```

and later:

```text
10 9000 None 0 #
20 9100 None 0 #
```

This is valid because the **message IDs match the intended blocks**:

```text
9000 -> block/index 9
9100 -> block/index 9
```

and the pre-dialog condition destinations are independently meaningful graph links.

Therefore an AI must not apply a simplistic rule such as:

> "All node numbers must be small."

The real rule is:

> **The first field is a graph destination/index whose meaning must be derived from the surrounding serialized structure and the target message block.**

The exact existing working file is authoritative.

---

## 138.8 `Text not found!` was a graph/text mismatch, not a cache problem

During testing, a dialogue window opened but displayed:

```text
Text not found!
```

Clearing caches and saves did not solve it.

The successful minimal Seth dialogue subsequently demonstrated that the server was loading the `.fodlg` correctly.

Therefore, when:

```text
Dialogue window opens
+
Text not found!
```

the first debugging checks should be:

1. Does the graph reference the intended message ID?
2. Does that message ID exist in `[engl]`?
3. Is the destination/index consistent with the message block?
4. Is the answer connected to the intended dialogue block?
5. Is the record using the same structural pattern as a known-good file?

**Do not start by clearing caches.**

---

## 138.9 Verified development workflow for a new scripted condition

For a new script such as:

```text
d_IsArmed
```

use this workflow:

```text
1. Compile the script.
        |
        v
2. Add ONE D _script condition.
        |
        v
3. Make true and false outcomes visibly different.
        |
        v
4. Open with condition TRUE.
        |
        v
5. Open with condition FALSE.
        |
        v
6. Check server log if one route fails.
        |
        v
7. Fix graph routing before adding quest logic.
```

Only after this works:

```text
armed check
        +
quest state
        +
quest result
```

should be combined.

---

## 138.10 Current verified Seth baseline

At the time of this update, the safest known-good Seth baseline is:

```text
[dialog]
&
1 1000 None 0 #
3 1010 @
3 2000 None 0 #
0 2010 &
```

with:

```text
[engl]
{100}{}{Seth}
{200}{}{You see Seth, leader of the guards of Shady Sands.}
{1000}{}{Predialogue installations}
{1010}{}{Talk to Seth}
{2000}{}{Good day. I am called Seth. How can I help you?}
{2010}{}{Goodbye.}
```

Treat this as a **golden rollback test**.

Do not modify this baseline when experimenting with the Radscorpion quest. Copy it first.

---

## 138.11 Updated AI safety rules from the Seth debugging session

An AI generating `.fodlg` for this server must now additionally obey:

### Rule A — distinguish registration IDs from graph/message IDs

```text
4010
```

in `dialogs.lst` is not interchangeable with:

```text
4000
4010
```

inside `[dialog]` / `[engl]`.

### Rule B — never choose the first numeric field arbitrarily

The first field is a destination/index.

The second field is the message/record ID.

Always derive the first field from the intended graph target.

### Rule C — don't diagnose a runtime link error as a script failure automatically

If:

```text
Dialog_Begin ... link<X> not found
```

appears only on one condition branch, inspect the target graph first.

### Rule D — preserve a known-good baseline

Before modifying an existing working dialogue:

```text
copy working file
        |
        v
make one structural change
        |
        v
DialogEditor
        |
        v
server test
```

### Rule E — DialogEditor acceptance is necessary but not sufficient

A file may pass the editor and still fail at runtime because a destination link is invalid or points to an unintended graph block.

Therefore validation must occur at two levels:

```text
DialogEditor
    +
runtime server log/game test
```

### Rule F — never use cache clearing as the first response to a graph-link error

First inspect:

```text
destination
message ID
target block
language message
condition branch
```

Only investigate caching after those are verified.

---

## 138.12 Current status of the Seth Radscorpion task

Verified:

```text
4010 -> seth registration
Seth NPC -> dialogue starts
basic Seth dialogue -> works
d_IsArmed() -> script executes
armed route -> reaches its branch
false route -> exposed an invalid graph link
```

Not yet verified:

```text
complete armed/unarmed Seth dialogue
Radscorpion quest-state pre-dialog
q_shady_radscorpions transitions
r_ShowRadscorpionCaves
r_FinishRadscorpionQuest
```

Therefore these should **not yet be described as server-verified constructs** in an AI-generated dialogue.

---

## 138.13 Recommended next test

The next test should contain:

```text
pre-dialog
    |
    +-- d_IsArmed = true  -> armed dialogue
    |
    +-- d_IsArmed = false -> normal Seth dialogue
```

and nothing else.

Once both routes work, add the Radscorpion quest.

This isolates one dependency at a time and prevents a broken quest graph from obscuring a condition-routing problem.

---

# 139. Newly verified: multi-branch informational dialogue graph

The Aradesh informational dialogue was used as a real editor/runtime test and successfully demonstrated that the legacy graph format supports a clean multi-topic dialogue tree when the graph is serialized using the canonical destination pattern.

This is now a **project-verified working pattern**, not merely a theoretical interpretation.

## 139.1 Validated basic structure

The working structure follows this pattern:

```text
[dialog]
&
1 1000 None 0 #
2 1010 @

2 2000 None 0 #
3 2010 #
4 2020 #
5 2030 #
0 2040 @

3 3000 None 0 #
4 3010 #
5 3020 #
0 3030 @

4 4000 None 0 #
3 4010 #
5 4020 #
0 4030 @

5 5000 None 0 #
4 5010 #
3 5020 #
0 5030 &
```

This structure was tested successfully with actual dialogue options and branch navigation.

The important point is that the first integer is the **destination graph index**. The second integer is the text ID.

For example:

```text
3 3010 #
```

means:

```text
answer/text ID 3010 -> dialogue node 3 -> text block 3000
```

It does **not** mean that answer `3010` points to text `3010`.

## 139.2 The previous `Bad node 3` mistake

A previous generated dialogue produced:

```text
Bad node 3
```

because the graph referenced destination node `3` without defining the corresponding dialogue node:

```text
3 3010 @
```

The correction is to define the destination node itself:

```text
3 3000 None 0 #
```

and then provide its answer records:

```text
4 3010 #
5 3020 #
0 3030 @
```

Therefore:

> **Every nonzero destination used by an answer must have a corresponding graph node defined in the `[dialog]` section.**

Do not treat the second number (`3010`) as the node definition. The node definition is the `3000` block whose graph index is `3`.

## 139.3 Safe way to create additional informational branches

For an informational NPC, use a new thousand block for each dialogue node:

```text
N  N000  None 0 #
```

with answer IDs:

```text
N+1  N010  #
N+2  N020  #
N+3  N030  #
0    N040  @
```

where the destination numbers refer to already-defined or subsequently-defined graph nodes.

Example:

```text
6 6000 None 0 #
7 6010 #
8 6020 #
0 6030 @
```

requires nodes `7` and `8` to exist somewhere in the graph:

```text
7 7000 None 0 #
8 8000 None 0 #
```

This is preferable to inventing a modern-looking tree syntax or assuming that IDs themselves define routing.

## 139.4 Informational dialogue can be built without scripts

A dialogue does **not** need demands, results, variables, or script functions simply to provide multiple informational topics.

A clean informational NPC can therefore use:

```text
NPC introduction
    |
    +-- Topic A -> information -> return/another topic
    +-- Topic B -> information -> return/another topic
    +-- Topic C -> information -> return/another topic
    +-- Goodbye
```

The Aradesh test confirmed that this kind of branching works in the current editor/runtime when the serialized graph is correct.

This should be the preferred starting point for a new informational NPC. Add scripts and quest conditions only after the plain graph has been compiled and tested.

## 139.5 Do not overcomplicate the first version

For a new NPC, the recommended workflow is now:

1. Create the `[comment]`, `[data]`, `[dialog]`, and `[engl]` sections.
2. Start from the canonical working graph pattern.
3. Add the NPC's main informational node.
4. Add topic branches.
5. Add a terminal answer.
6. Compile in the Dialogue Editor.
7. Test every topic and return path in-game.
8. Only then add `D` demands, `R` results, variables, or script calls.

This is especially important for quest NPCs. A working informational dialogue gives a known-good graph before quest-state logic is introduced.

## 139.6 Project rule: preserve a known-good graph

Once a graph has been successfully tested in the current Dialogue Editor, treat its structure as a **template**.

Do not rewrite it into a theoretically cleaner representation merely because another numbering scheme looks simpler.

The safest operation is:

```text
copy known-good graph
        ↓
change text IDs/content
        ↓
change destinations deliberately
        ↓
verify every destination
        ↓
compile
        ↓
test every branch
```

This is now considered a stronger rule than generating a dialogue graph from abstract assumptions.

---

# 140. Newly verified: dialogue debugging lessons

Several practical failures during Seth development produced useful rules for future dialogue work.

## 140.1 A dialogue ID alone is not enough to diagnose a failure

A valid dialog registration such as:

```text
4010
```

does not guarantee that every graph route is valid.

The server can successfully identify the dialogue pack and still fail when a selected answer points to a missing graph node.

Typical symptom:

```text
FOServer::Dialog_Begin - Dialog from link<3> not found, ... dialog pack<4010>.
```

When this occurs, inspect the graph destination before investigating caches.

## 140.2 Cache clearing does not repair an invalid graph

During the Seth tests, cache/save cleanup did not resolve the `Dialog from link` problem because the underlying issue was graph routing.

Therefore, for future failures:

```text
1. inspect graph destination
2. inspect node definition
3. inspect text ID
4. inspect condition/result syntax
5. compile again
6. only then investigate cache/state issues
```

## 140.3 A successful condition can still lead to a missing graph link

Seth's `d_IsArmed()` test demonstrated that the script condition itself can work correctly while the dialogue branch still fails because the selected route is malformed.

Therefore debug these independently:

```text
condition execution
        !=
dialogue graph routing
```

Do not assume that a failed dialogue after a successful condition means the condition is wrong.

---

# 141. Newly verified: Seth/guard event findings relevant to dialogue-triggered NPC behavior

These findings are outside the `.fodlg` graph itself but are useful when a dialogue is opened or triggered by server events.

## 141.1 `MSG_ATTACK_ME` is part of the guard system

The reliable Brotherhood guard example demonstrated this working pattern:

```cpp
player.SendMessage(MSG_ATTACK_ME, 0, MESSAGE_TO_VISIBLE_ME);
```

The important discovery was that the NPC's own message handler did not need to implement the attack manually. The working guard implementation delegates messages through the shared guard system.

The relevant pattern is:

```cpp
void _OnMessage(Critter& npc, Critter& player, int num, int val)
{
    GuardOnMessage(npc, player, num, val);

    // NPC-specific messages...
}
```

## 141.2 `GuardInit()` has broader side effects

Adding:

```cpp
GuardInit(guard);
```

to Seth made the attack behavior work, but it also caused other NPCs sharing the same guard role to become hostile, including nearby dogs.

Therefore:

> Do not add `GuardInit()` to an NPC merely because `MSG_ATTACK_ME` is needed. First inspect the existing guard implementation and understand what initialization changes globally or by role.

The successful attack behavior and the broader guard initialization are separate concerns.

## 141.3 `GuardOnMessage()` and `GuardInit()` must not be assumed interchangeable

The current project evidence shows:

```text
GuardOnMessage()
    -> processes guard messages

GuardInit()
    -> initializes broader guard behavior
```

A future NPC that only needs the message-processing behavior should not automatically receive the complete initialization behavior without testing.

---

# 142. Current strongest AI-generation rule

When generating a new `.fodlg` for this project:

> **Start from a known-good `.fodlg` graph that has actually compiled and been tested in the current Dialogue Editor. Preserve its serialized graph conventions. Every nonzero destination must resolve to a defined graph node. The first integer is the destination graph index; the second integer is the text ID. Build and test plain informational branches before adding scripts, demands, results, or quest variables. Never invent a cleaner graph syntax.**

This rule incorporates the successful Aradesh branch test and should take precedence over any abstract interpretation that conflicts with an existing working project example.

---

# 143. Stop-update: confirmed findings from the Seth investigation

This section records only findings that are useful for future dialogue generation and that are supported by the project files examined during the Seth work. It intentionally does **not** claim that the current Seth quest dialogue is finished or runtime-complete.

## 143.1 Large dialogues prove that the graph is not limited to node 12

The project contains working `.fodlg` files with substantially more than twelve dialogue graph nodes. In particular:

- `den_metzger.fodlg` contains graph indices continuing far beyond 12, including nodes 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, etc.
- `bos_doctor.fodlg` contains graph indices well beyond 12 and continues through at least node 203 in the supplied file.
- `bos_lh_qmaster(1).fodlg` contains a large quest dialogue graph with graph indices continuing well beyond 12.

Therefore:

> **There is no general `.fodlg`/DialogEditor rule that limits a dialogue to graph nodes 0–12.**

If a small dialogue fails when adding node 13/14/15/16, the correct response is to inspect the graph links, node definitions, text IDs, demands/results, and parser/runtime errors—not to redesign the dialogue around an assumed twelve-node limit.

Reference examples:

```text
den_metzger.fodlg
bos_doctor.fodlg
bos_lh_qmaster(1).fodlg
```

These files are project-provided examples and should be treated as structural references when generating larger NPC dialogues.

## 143.2 Graph index and text ID remain separate

The large examples reinforce the already-established rule that the first number is the destination graph index and the second number is the text/message ID.

For example, the supplied `den_metzger.fodlg` contains records such as:

```text
42 12000 None 0 #
43 13000 None 0 #
12 14000 dialog@dlg_ShowFaction 0 #
44 14010 #
```

The graph can therefore use destination indices that do not numerically resemble the text IDs. The graph index is an internal routing index; the second number identifies the dialogue text record.

## 143.3 Large NPCs use the same compact D/R system

The large NPC examples confirm that complicated conversations do not require duplicated dialogue text or duplicated copies of the same greeting.

`den_metzger.fodlg` demonstrates long sequences of:

```text
D <demand/condition>
R <result/action>
```

on the same answer record.

Examples include variable checks, item checks, parameters, script demands, and script results.

`bos_doctor.fodlg` similarly combines conditions and results on individual answers.

Therefore the preferred architecture for a quest NPC is:

```text
one dialogue node
    |
    +-- answer with D conditions
    +-- answer with D conditions
    +-- answer with D conditions + R actions
```

rather than duplicating the same NPC greeting three times merely to represent quest states.

## 143.4 Three-state quest variables are a normal and sufficient pattern

The Seth design uses:

```text
q_shady_radscorpions
0 = not started
1 = ongoing
2 = completed
```

This is a valid state-machine pattern for a simple quest.

The supplied Brotherhood quartermaster dialogue provides a stronger project reference: `q_bos_initiatequest_start` is used across many dialogue branches with comparisons such as:

```text
= 11
< 11
= 12
= 13
= 14
= 15
= 20
= 26
```

and results that advance or reset the variable.

So a dialogue does **not** need separate duplicate greetings for every quest state. The state can be selected by pre-dialog demands and the answer/branch conditions.

## 143.5 A clean three-state Seth structure is therefore valid in principle

The intended architecture is:

```text
pre-dialog
  |
  +-- d_IsArmed -> weapon-warning branch
  |
  +-- q_shady_radscorpions = 0 -> node 3
  |
  +-- q_shady_radscorpions = 1 -> node 4
  |
  +-- q_shady_radscorpions = 2 -> node 5
```

Then:

```text
node 3 (not started)
  -> information
  -> offer Radscorpion help
       -> result: q = 1 + reveal caves

node 4 (ongoing)
  -> information
  -> "Not yet"
  -> "The Radscorpions are dead"
       -> result: q = 2 + reward

node 5 (completed)
  -> information
  -> normal completed-state response
```

This is structurally consistent with the much larger quest dialogues already present in the SDK.

## 143.6 Script callback signatures are part of dialogue binding

A runtime error observed during Seth testing was:

```text
Script::Bind - Function<void r_StartRadscorpionQuest(Critter&,Critter@)> in module<seth> not found.
DialogManager::LoadDemandResult - Script<seth@r_StartRadscorpionQuest> bind error.
```

This establishes an important rule:

> A dialogue script reference is bound by the function's expected signature, not merely by its name.

The supplied current `seth.fos` uses the project-compatible callback form:

```angelscript
bool d_IsArmed(Critter& player, Critter@ npc)
void r_ShowRadscorpionCaves(Critter& player, Critter@ npc)
void r_FinishRadscorpionQuest(Critter& player, Critter@ npc)
```

Therefore new Seth dialogue result functions should follow the same callback signature convention:

```angelscript
void r_SomeAction(Critter& player, Critter@ npc)
```

**Important:** the exact function name in the `.fodlg` must also exactly match a function actually present in the loaded `seth` script module. A function can exist under a different name and still produce a `Function ... not found` binding error.

The current supplied `seth.fos` contains:

```text
r_ShowRadscorpionCaves
r_FinishRadscorpionQuest
```

while the current `seth(2).fodlg` references:

```text
r_StartRadscorpionQuest
r_CompleteRadscorpionQuest
```

That name mismatch must be treated as a separate script-binding issue. It is **not** a dialogue-node-limit issue.

## 143.7 The current Seth file is already a useful compact reference

The supplied `seth(2).fodlg` demonstrates several useful patterns in one small file:

### Conditional pre-dialog

```text
2 1010 D _script seth@d_IsArmed 0 #
3 1020 D _var p q_shady_radscorpions = 0 #
4 1030 D _var p q_shady_radscorpions = 1 #
5 1040 D _var p q_shady_radscorpions = 2 @
```

### Information branch

```text
7 7000 None 0 #
8 7010 #
9 7020 #
10 7030 #
0 7050 @
```

### Demand + result on one answer

```text
0 14010 R _var p q_shady_radscorpions = 1 R _script seth@r_StartRadscorpionQuest 0 #
```

This confirms that a result chain can contain multiple `R` operations on the same answer record.

## 143.8 Recommended reference set for future NPC dialogue generation

For a new NPC, use the following project files as the primary structural references:

### Minimal/simple graph

```text
start_journey.fodlg
```

Use for understanding the basic graph and answer routing.

### Small informational NPCs

```text
water_merchant.fodlg
caravan_informant.fodlg
all_mine_foreman.fodlg
```

Use for simple conversations without unnecessary quest-state duplication.

### Medium/large informational or service NPCs

```text
bos_doctor.fodlg
hub_jacob.fodlg
bh_steve.fodlg
```

Use for multiple branches, demands, and results.

### Large quest/dialogue graphs

```text
den_metzger.fodlg
nr_jules.fodlg
bos_lh_qmaster(1).fodlg
aradesh.fodlg
```

Use these when the NPC has multiple quest states, services, information branches, and result chains.

### Seth-specific references

```text
seth.fos
seth(2).fodlg
seth(1).fodlg
seth_test.fodlg
map_shadysands.fos
```

These should be used together when debugging Seth because the `.fodlg` graph, script functions, and Shady Sands event handling are separate layers.

## 143.9 Stop point

The useful conclusion at this stage is:

1. **The dialogue system absolutely supports graphs larger than 12 nodes.**
2. **A three-state quest variable (0/1/2) is sufficient for the intended Seth quest.**
3. **Large existing NPCs use the same D/R branching system and do not need duplicated greetings for each state.**
4. **The first integer in a graph record is the destination graph index; the second is the text ID.**
5. **Every nonzero destination must correspond to a defined graph node.**
6. **Dialogue script calls require an exact function name and compatible callback signature.**
7. **The current Seth script uses `Critter@ npc` in its dialogue callback signatures.**
8. **The current Seth `.fodlg` has a script-name mismatch (`r_StartRadscorpionQuest` / `r_CompleteRadscorpionQuest` versus the functions currently present in `seth.fos`).**
9. **Future dialogue generation should use the large SDK NPCs as structural references rather than trying to force the conversation into an artificially small graph.**

This is the stopping point for the current dialogue investigation. The next implementation step can be taken from these verified patterns without redesigning the dialogue architecture.
