# Admin

~getaccess admin 123456

# Set special

ST ~runscript cheats set_special 1 10 0
PE ~runscript cheats set_special 2 10 0
EN ~runscript cheats set_special 3 10 0
CH ~runscript cheats set_special 4 10 0
IN ~runscript cheats set_special 5 10 0
AG ~runscript cheats set_special 6 10 0
LK ~runscript cheats set_special 7 10 0 

# Skills

Ex.
`~runscript cheats set_skill 200 300 0`
Increase small guns to 300.

## Combat Skills:

200 — Small Guns

201 — Big Guns

202 — Energy Weapons

203 — Close Combat / Unarmed

204 — Melee Weapons (varies by exact server fork, sometimes grouped)

205 — Throwing

Active / Passive Utility Skills:

206 — First Aid

207 — Doctor

208 — Sneak

209 — Lockpick (or equivalent engineering/robbery depending on version)

210 — Steal (Robbery)

211 — Traps

212 — Science

213 — Repair

214 — Speech

215 — Barter

216 — Gambling (if present)

217 — Outdoorsman

# Other abilities
set MAX hp to 500
~runscript cheats set_stat_extended 7 500 0

set current HP to 500
~runscript cheats set_stat_extended 72 500 0

set healing rate to 50
~runscript cheats set_stat_extended 13 50 0

# Others

ST_LOCOMOTION_TYPE          (68)
ST_DAMAGE_TYPE              (69)
ST_AGE                      (70) 
ST_GENDER                   (71) 
ST_CURRENT_HP               (72) 
ST_POISONING_LEVEL          (73) 
ST_RADIATION_LEVEL          (74) 
ST_CURRENT_AP               (75) 
ST_EXPERIENCE               (76) 
ST_LEVEL                    (77) 
ST_UNSPENT_SKILL_POINTS     (78) 
ST_UNSPENT_PERKS            (79) 
ST_KARMA                    (80) 
ST_FOLLOW_CRIT              (81) 
ST_REPLICATION_MONEY        (82) 
ST_REPLICATION_COUNT        (83) 
ST_REPLICATION_TIME         (84) 
ST_REPLICATION_COST         (85) 
ST_TURN_BASED_AC            (86) 
ST_MAX_MOVE_AP              (87) 
ST_MOVE_AP                  (88) 
ST_NPC_ROLE                 (89) 

# set level
`~run cheats set_level <targetLevel> 0 0`

# Map teleport

# Useful scripts
Complete list of maps is at _maps.fos

| Command | Map |
| :--- | :--- |
`~runscript cheats tp_map 100 100 100` | The Hub
103 | Water merchants
`~runscript cheats tp_map 1 100 100` | Den
`~runscript cheats tp_map 63 120 265` | San Francisco Vendor
`~runscript cheats tp_map 16 220 155` | BrokenHills Marcus
`~runscript cheats tp_map 8 100 100` | Modoc
`~runscript cheats tp_map 9 100 100` | Vault City Inner
`~runscript cheats tp_map 12 100 80` | Vault City Vault 3rd floor
`~runscript cheats tp_map 11 100 80` | Vault City Vault 2nd floor
`~runscript cheats tp_map 10 100 80` | Vault City Vault 1st floor
`~runscript cheats tp_map 13 100 80` | Navarro
`~runscript cheats tp_map 14 50 80` | Gecko Junkyard
`~runscript cheats tp_map 15 50 80` | Gecko Powerplant
`~runscript cheats tp_map 17 50 60` | BrokenHills Basement
`~runscript cheats tp_map 20 50 60` | New Reno
`~runscript cheats tp_map 21 60 60` | New Reno Shark Club
`~runscript cheats tp_map 22 60 60` | New Reno Renesco
`~runscript cheats tp_map 23 60 60` | New Reno Wright State
`~runscript cheats tp_map 24 100 50` | New Reno TRay Garage
`~runscript cheats tp_map 29 50 50` | New Reno Eldridge Basement
`~runscript cheats tp_map 33 50 55` | Sierra Army Depot
`~runscript cheats tp_map 39 50 55` | Vault 15 entrance
`~runscript cheats tp_map 43 50 55` | NCR Entrance
`~runscript cheats tp_map 45 50 55` | BOS NCR Entrance
`~runscript cheats tp_map 46 50 55` | Chatedral Entrance
`~runscript cheats tp_map 49 60 75` | Chatedral 2nd floor
`~runscript cheats tp_map 51 104 105` | Chatedral Basement
`~runscript cheats tp_map 54 70 55` | Chatedral Vault Bomb floor
`~runscript cheats tp_map 55 70 55` | Mariposa base entrance
`~runscript cheats tp_map 60 70 55` | Redding
`~runscript cheats tp_map 63 80 55` | San Francisco Oil tanker
`~runscript cheats tp_map 67 100 100` | Navarro entrance
`~runscript cheats tp_map 70 100 100` | Junktown
`~runscript cheats tp_map 72 100 100` | BOS Broken Hills entrance
`~runscript cheats tp_map 78 100 100` | The Glow
`~runscript cheats tp_map 93 100 100` | Necropolis
98 | Necropolis vault entrance
104 | Falcon Maltese basement
105 | Theft guild
107 | New Adytum
109 | Raiders Camp
110 | Raiders Cave
111 | Ares Silos
125 | Lybrary
132 | Respawn 1
134 | Respawn 2
135 | Respawn 3
136 | Sanfrancisco Respawn
137 | NCR Army Respawn
138 | Tribal respawn
139 | Custom doctor respawn
142 | Jail
151 | San Francisco Train Station 
152 50 60 | Boneyard Train Station
153 50 60 | NCR Train Station
154 50 60 | Vault City Train Station
155 | Junkyard

# List of factions by index

`~runscript cheats set_reputation <faction id> <value> 0

0   |   ---Factions/Groups--- |
| :--- | :--- |
1   |   FACTION_NONE
2   |   FACTION_BOS
3   |   FACTION_ENCLAVE
4   |   FACTION_UNITY
5   |   FACTION_NCR
6   |   FACTION_VAULT_CITY
7   |   FACTION_RAIDERS
8   |   FACTION_MORDINOS
9   |   FACTION_SLAVERS_GUILD
10  |   FACTION_SLAGS
11  |   FACTION_KOKOWEEF
12  |   FACTION_MORNINGSTAR
13  |   FACTION_BH_HUMANS
14  |   FACTION_BH_MUTANTS
15  |   FACTION_WRIGHTS
16  |   FACTION_BISHOPS
17  |   FACTION_SALVATORES
18  |   FACTION_NCR_RANGERS
19  |   FACTION_VORTIS_SLAVERS
20  |   FACTION_COMMUNISTS
21  |   FACTION_VAGRANTS
22  |   FACTION_CRYPTS
23  |   FACTION_FOOLS
24  |   FACTION_HUB_CRIMINALS
25  |   FACTION_CRIMSON_CARAVANS
26  |   FACTION_FARGO_TRADERS
27  |   FACTION_WATER_MERCHANTS
28  |   FACTION_HEIGHTS
29  |   FACTION_REGULATORS
30  |   FACTION_SCAVENGERS
31  |   FACTION_GUNRUNNERS
32  |   FACTION_FOLLOWERS
33  |   FACTION_BOUNTY_HUNTERS
34  |   FACTION_FREE_TRADERS
35  |   FACTION_GUTTERSNIPES
36  |   FACTION_FLC
37  |   FACTION_SLAVES
38  |   FACTION_LARAS_GANG
39  |   FACTION_CHILDREN
40  |   FACTION_NR_PIMPS
41  |   FACTION_ELRONOLOGISTS
42  |   FACTION_CATHEDRAL
43  |   FACTION_CORSICAN_BROTHERS
80  |   FACTION_KLAMATH
81  |   FACTION_DEN
82  |   FACTION_MODOC
83  |   FACTION_COURTYARD
84  |   FACTION_GECKO
85  |   FACTION_REDDING
86  |   FACTION_BH_GHOULS
87  |   FACTION_NEW_RENO
88  |   FACTION_NECROPOLIS
89  |   FACTION_JUNKTOWN
90  |   FACTION_HUB
91  |   FACTION_LA