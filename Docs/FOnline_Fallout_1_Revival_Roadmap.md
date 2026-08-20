# FOnline Fallout 1 Revival --- Server Roadmap

## 1. Introduction

This project aims to recreate the experience of the original **Fallout
1** game as an online multiplayer server using the FOnline engine.

The goal is not to reproduce Fallout 1 as a strict 1:1 conversion.
FOnline has its own capabilities and limitations, so the original game
must be adapted to an online environment.

The intended result is that players should **feel like they are playing
Fallout 1**, while benefiting from multiplayer systems, persistent
online gameplay, scalable encounters, instances, and other capabilities
provided by FOnline.

The project is open source and intended to be developed collaboratively.
The server is hosted on Linux, players can register and play online, and
the project welcomes contributions, collaboration, and forks through
GitHub.

------------------------------------------------------------------------

# 2. Server Goals

## 2.1 Fallout 1 Recreation

The primary goal is to recreate the original Fallout 1 experience as
faithfully as practical.

This includes:

-   Original Fallout 1 locations
-   Original characters and NPCs
-   Original quests
-   Original major questlines
-   Original world progression
-   Original atmosphere and setting
-   Adapted versions of Fallout 1 gameplay systems

Where the FOnline engine cannot reproduce something exactly, the
implementation should prioritize preserving the **player experience and
intent** of the original content.

## 2.2 Online Experience

The game should use FOnline's online capabilities to expand the original
single-player experience.

Players should be able to:

-   Register online
-   Create and develop characters
-   Explore the Fallout 1 world
-   Complete quests
-   Fight enemies
-   Travel between locations
-   Encounter other players
-   Participate in multiplayer gameplay

The online systems should complement the Fallout 1 experience rather
than turning the project into a completely different type of FOnline
server.

------------------------------------------------------------------------

# 3. Main Gameplay Philosophy

## 3.1 PvE Focus

The server is primarily **PvE focused**.

PvP may exist as part of the online environment, but it should not
prevent players from experiencing and completing the Fallout 1 quest
progression.

The project should avoid a situation where another player can
permanently block essential story content.

### Design Principle

> PvP should never prevent a player from completing the intended Fallout
> 1 experience.

------------------------------------------------------------------------

## 3.2 Instances

Instances will be used to make important content accessible without
requiring players to compete for unique quest objectives.

Instances should be available to:

-   Solo players
-   Groups of players

Main instances can be significantly more difficult than their
normal-world equivalents.

The intended balance is:

**Accessible content + higher difficulty**

rather than:

**Accessible content + trivial difficulty**

This allows solo players to experience important content while still
giving groups an advantage.

------------------------------------------------------------------------

# 4. World Scope

The playable world is based on the original **Fallout 1** world.

## 4.1 Planned Locations

The primary world progression includes:

1.  Vault 13
2.  Vault 15
3.  Shady Sands
4.  The Hub
5.  Junktown
6.  Necropolis
7.  Brotherhood of Steel
8.  Military Base
9.  Cathedral

These locations form the main geographic and narrative structure of the
server.

------------------------------------------------------------------------

## 4.2 Fallout 2 Content

Fallout 2 locations are explicitly **out of scope**.

Some Fallout 2 maps are already available through the FOnline SDK, but
they will remain hidden and will not form part of the playable world.

The project should therefore avoid expanding the roadmap around Fallout
2 content.

### Scope Rule

> The server recreates the Fallout 1 experience. Fallout 2 world content
> is not part of the current project scope.

------------------------------------------------------------------------

# 5. Locations

Locations are one of the main development pillars.

Each location should eventually contain the appropriate combination of:

-   Maps
-   NPCs
-   Dialogue
-   Quests
-   Enemies
-   Loot
-   Interactive objects
-   Travel connections
-   Random encounters
-   Scripts
-   Special events

## 5.1 Location Roadmap

### Vault 13

-   [ ] Main Vault 13 maps
-   [ ] Vault NPCs
-   [ ] Starting interactions
-   [ ] Water Chip storyline
-   [ ] Vault-related quests
-   [ ] Equipment and loot
-   [ ] Travel connection to the outside world

### Vault 15

-   [ ] Vault 15 maps
-   [ ] NPCs
-   [ ] Hostile encounters
-   [ ] Exploration content
-   [ ] Fallout 1 quest interactions
-   [ ] Loot
-   [ ] Travel integration

### Shady Sands

-   [ ] Main Shady Sands maps
-   [ ] NPCs
-   [ ] Guards
-   [ ] Merchants
-   [ ] Brahmin-related content
-   [ ] Radscorpion Cave
-   [ ] Missing Caravans storyline
-   [ ] Main Fallout 1 interactions
-   [ ] Loot
-   [ ] Travel integration

### The Hub

-   [ ] Hub maps
-   [ ] Merchants
-   [ ] Caravans
-   [ ] Faction interactions
-   [ ] Missing Caravans storyline
-   [ ] Fallout 1 quests
-   [ ] NPC dialogue
-   [ ] Loot
-   [ ] Travel integration

### Junktown

-   [ ] Junktown maps
-   [ ] Major NPCs
-   [ ] Local conflicts
-   [ ] Fallout 1 quests
-   [ ] NPC dialogue
-   [ ] Loot
-   [ ] Travel integration

### Necropolis

-   [ ] Necropolis maps
-   [ ] Ghoul NPCs
-   [ ] Hostile encounters
-   [ ] Main quest content
-   [ ] Water Chip-related content
-   [ ] Dungeon/instance content
-   [ ] Loot
-   [ ] Travel integration

### Brotherhood of Steel

-   [ ] Brotherhood maps
-   [ ] Brotherhood NPCs
-   [ ] Brotherhood reputation/interactions
-   [ ] Main Brotherhood questline
-   [ ] Brotherhood equipment
-   [ ] Brotherhood-related instances
-   [ ] Dialogue
-   [ ] Loot

### Military Base

-   [ ] Military Base maps
-   [ ] Enemy population
-   [ ] Military Base raid
-   [ ] Main dungeon/instance
-   [ ] Special encounters
-   [ ] Loot
-   [ ] Quest integration

### Cathedral

-   [ ] Cathedral maps
-   [ ] Cathedral NPCs
-   [ ] Main questline
-   [ ] Final dungeon
-   [ ] Basement
-   [ ] Final boss encounter
-   [ ] End-game progression

------------------------------------------------------------------------

# 6. Respawn Maps and Scavenging

Respawn maps should provide a repeatable source of exploration and
useful equipment.

Random lockers will be placed throughout appropriate respawn maps.

These lockers can contain useful loot, giving players an incentive to
explore the world and revisit locations.

## Goals

-   [ ] Define respawn maps
-   [ ] Add randomized lockers
-   [ ] Define useful loot tables
-   [ ] Balance loot availability
-   [ ] Prevent excessive farming
-   [ ] Make exploration worthwhile
-   [ ] Test respawn behavior

The system should reinforce the server's general philosophy:

> Players should primarily **find and acquire equipment**, rather than
> manufacture everything themselves.

------------------------------------------------------------------------

# 7. NPCs

NPCs are a major part of recreating Fallout 1.

The project should prioritize the original Fallout 1 NPCs and
interactions while allowing additional NPCs where they improve the
online experience.

## 7.1 Original NPCs

-   [ ] Recreate important Fallout 1 NPCs
-   [ ] Recreate important dialogue
-   [ ] Implement quest interactions
-   [ ] Implement relevant NPC states
-   [ ] Implement faction relationships
-   [ ] Implement services
-   [ ] Test dialogue and scripts

## 7.2 Additional Online NPCs

Additional NPCs may be introduced where FOnline functionality benefits
from them.

Examples include:

### Brahmin Dealer

A dedicated NPC can provide interaction around the acquisition and
management of Brahmin.

-   [ ] Define NPC
-   [ ] Define dialogue
-   [ ] Define services
-   [ ] Connect to relevant systems
-   [ ] Test interaction

### Equipment Repair NPC

A dedicated NPC can provide equipment repair without requiring the
server to introduce excessive crafting.

-   [ ] Define NPC
-   [ ] Define repair mechanics
-   [ ] Define costs
-   [ ] Define restrictions
-   [ ] Add dialogue
-   [ ] Test repairs

Additional NPCs should support the original experience rather than
unnecessarily changing it.

------------------------------------------------------------------------

# 8. Caravans and Encounters

Caravans are an important opportunity to use existing FOnline SDK
functionality.

The server should leverage the existing caravan systems rather than
replacing them with an unrelated custom system.

## Goals

-   [ ] Integrate existing FOnline caravan functionality
-   [ ] Define caravan routes
-   [ ] Define caravan encounters
-   [ ] Add random encounters
-   [ ] Scale encounters according to player experience/progression
-   [ ] Balance encounter difficulty
-   [ ] Add appropriate rewards
-   [ ] Test solo encounters
-   [ ] Test group encounters

Caravans should provide players with:

-   Travel gameplay
-   Random encounters
-   Combat opportunities
-   Exploration
-   Progression
-   Additional experience

Encounter scaling is especially important because the server is designed
around both solo players and groups.

------------------------------------------------------------------------

# 9. Crafting

Crafting will be **severely limited compared with many other FOnline
servers**.

The central philosophy is that players should find, buy, trade for, or
otherwise acquire equipment rather than spend most of their time
crafting it.

Crafting will still exist where it is useful or important.

## Intended Crafting

-   [ ] Essential survival items
-   [ ] Important utility items
-   [ ] Tents
-   [ ] Other systems where crafting is necessary

## Avoid

-   [ ] Excessive weapon crafting
-   [ ] Excessive armor crafting
-   [ ] Full equipment manufacturing economy
-   [ ] Crafting replacing exploration
-   [ ] Crafting replacing loot acquisition

### Design Principle

> Equipment should primarily be discovered and acquired, not
> manufactured.

------------------------------------------------------------------------

# 10. Main Questlines

The main quest structure should follow the major progression of Fallout
1.

## 10.1 Water Chip

The Water Chip storyline is one of the central objectives of the server.

-   [ ] Vault 13 setup
-   [ ] Water Chip objective
-   [ ] Relevant locations
-   [ ] Relevant NPCs
-   [ ] Water Chip quest stages
-   [ ] Quest rewards/consequences
-   [ ] End-state

------------------------------------------------------------------------

## 10.2 Shady Sands --- Radscorpion Cave

The Radscorpion Cave provides an early quest and combat experience.

-   [ ] Quest NPC
-   [ ] Cave location
-   [ ] Radscorpion encounters
-   [ ] Quest objectives
-   [ ] Rewards
-   [ ] Quest completion state
-   [ ] Test solo difficulty
-   [ ] Test group difficulty

------------------------------------------------------------------------

## 10.3 Missing Caravans

The missing caravans storyline should connect the appropriate locations
and NPCs.

-   [ ] Quest setup
-   [ ] Caravan NPCs
-   [ ] Investigation
-   [ ] Relevant locations
-   [ ] Encounters
-   [ ] Quest resolution
-   [ ] Rewards
-   [ ] Consequences
-   [ ] Integration with caravan systems

------------------------------------------------------------------------

## 10.4 Brotherhood of Steel Main Questline

The Brotherhood should have a significant progression path.

-   [ ] Brotherhood introduction
-   [ ] Brotherhood NPCs
-   [ ] Entry/progression requirements
-   [ ] Brotherhood quests
-   [ ] Major interactions
-   [ ] Equipment access
-   [ ] Main quest progression
-   [ ] Brotherhood-related instances
-   [ ] Completion state

------------------------------------------------------------------------

## 10.5 Military Base Raid

The Military Base should serve as a major dungeon/instance.

-   [ ] Military Base map
-   [ ] Enemy layout
-   [ ] Instance setup
-   [ ] Solo difficulty
-   [ ] Group difficulty
-   [ ] Major encounters
-   [ ] Loot
-   [ ] Quest integration
-   [ ] Completion state

------------------------------------------------------------------------

## 10.6 Cathedral

The Cathedral represents the final major stage of the Fallout 1
experience.

-   [ ] Cathedral maps
-   [ ] Cathedral NPCs
-   [ ] Main quest progression
-   [ ] Enemy encounters
-   [ ] Cathedral dungeon
-   [ ] Basement
-   [ ] Final boss
-   [ ] End-game sequence
-   [ ] Final quest completion

------------------------------------------------------------------------

# 11. Quest Development Standard

Every major quest should be documented with:

-   [ ] Quest name
-   [ ] Quest giver
-   [ ] Starting location
-   [ ] Required conditions
-   [ ] Dialogue
-   [ ] Objectives
-   [ ] Intermediate stages
-   [ ] NPC states
-   [ ] Location states
-   [ ] Success conditions
-   [ ] Failure conditions
-   [ ] Rewards
-   [ ] Consequences
-   [ ] Follow-up content
-   [ ] Scripts
-   [ ] Testing

The implementation should preserve the original Fallout 1 quest intent
wherever possible.

------------------------------------------------------------------------

# 12. NPC, Location and Quest Relationship

The three primary content pillars should be developed together:

**Locations → NPCs → Quests**

A location should provide the environment.

NPCs should provide the interaction.

Quests should provide the gameplay objective.

For example:

**Shady Sands**

→ Guard/NPC interactions\
→ Radscorpion Cave\
→ Caravan problems\
→ Dialogue\
→ Combat\
→ Rewards\
→ Consequences

This keeps the world interconnected rather than creating isolated maps
or quests.

------------------------------------------------------------------------

# 13. Technical / Infrastructure Goals

## Server

-   [ ] Linux server deployment
-   [ ] Stable server startup
-   [ ] Database/player persistence
-   [ ] Online player registration
-   [ ] Account management
-   [ ] Backup strategy
-   [ ] Logging
-   [ ] Crash recovery
-   [ ] Deployment process

## Open Source

The project should remain publicly accessible and encourage community
development.

-   [ ] Maintain public GitHub repository
-   [ ] Document setup/build process
-   [ ] Document development workflow
-   [ ] Accept contributions
-   [ ] Accept forks
-   [ ] Document tools
-   [ ] Document FOnline-specific development
-   [ ] Maintain issue tracking
-   [ ] Maintain roadmap

------------------------------------------------------------------------

# 14. Development Priorities

Development should prioritize the playable Fallout 1 experience over
secondary systems.

## Priority 1 --- Core World

-   [ ] Vault 13
-   [ ] Vault 15
-   [ ] Shady Sands
-   [ ] Hub
-   [ ] Junktown
-   [ ] Necropolis
-   [ ] Brotherhood of Steel
-   [ ] Military Base
-   [ ] Cathedral

## Priority 2 --- Core NPCs

-   [ ] Fallout 1 NPCs
-   [ ] Guards
-   [ ] Merchants
-   [ ] Quest NPCs
-   [ ] Brotherhood NPCs
-   [ ] Important hostile NPCs
-   [ ] Additional online-service NPCs

## Priority 3 --- Main Quests

-   [ ] Water Chip
-   [ ] Radscorpion Cave
-   [ ] Missing Caravans
-   [ ] Brotherhood questline
-   [ ] Military Base
-   [ ] Cathedral
-   [ ] Final boss

## Priority 4 --- Online Systems

-   [ ] Player registration
-   [ ] Instances
-   [ ] Caravan system
-   [ ] Random encounters
-   [ ] Randomized locker loot
-   [ ] Equipment repair
-   [ ] Limited crafting

## Priority 5 --- Polish

-   [ ] Dialogue testing
-   [ ] Quest testing
-   [ ] Map testing
-   [ ] Balance
-   [ ] Loot balancing
-   [ ] Encounter scaling
-   [ ] Performance
-   [ ] Bug fixing

------------------------------------------------------------------------

# 15. Project Definition

The project can be summarized by the following principles:

### Fallout 1 First

The original Fallout 1 world, characters and quests define the project's
content scope.

### Online Where It Makes Sense

FOnline functionality should enhance the original experience rather than
fundamentally replace it.

### PvE Friendly

Players should be able to experience and complete the main game without
being blocked by PvP.

### Solo and Group Support

Important content should remain accessible to solo players while
allowing groups to experience harder encounters.

### Find, Don't Craft

Equipment acquisition, exploration and scavenging are more important
than extensive crafting.

### Open Source

The project is developed publicly and welcomes collaboration,
contributions and forks.

### Fallout 2 Is Out of Scope

Existing Fallout 2 maps in the FOnline SDK will remain hidden and are
not part of the current playable world.

------------------------------------------------------------------------

# 16. Overall Roadmap

The final progression of the project should follow this general
direction:

**Foundation**

→ Linux server / online registration / open-source infrastructure

**World**

→ Vault 13 → Vault 15 → Shady Sands → Hub → Junktown → Necropolis

**Advanced Progression**

→ Brotherhood of Steel → Military Base → Cathedral

**Gameplay**

→ NPCs → Dialogue → Quests → Combat → Loot → Caravans → Encounters

**Online Adaptation**

→ Instances → Solo/Group scaling → PvE protection → Random encounters

**End Game**

→ Cathedral Basement → Final Boss → Fallout 1 conclusion

The ultimate goal is a complete online adaptation of Fallout 1 where
players can progress through the original world and quest experience
while taking advantage of the multiplayer and persistent-world
capabilities of FOnline.
