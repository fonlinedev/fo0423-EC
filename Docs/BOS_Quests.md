# Brotherhood of steel

## Initiate quest
- Main script file: quest_bos_initiate.fos
- Variable: LVAR_q_bos_initiatequest_start 7074
- Dialogue file 1: ncr_bostrainer_henrickson.fodlg
- Dialogue file 2: ncr_bostrainer_henrickson.fodlg
- Dialogue file 3: ncr_bostrainer_henrickson.fodlg
- Dialogue file 4: san_bostrainer_windsted.fodlg

## Crashed Hummer value 21 < 22
- Spawned at worldmap.fos 
``
    _CheckSpecialEncounter(LOCATION_CrashedHummer, (ZoneContainsFlag(zone, FLAG_CrashedHummer) && (GetLvar(leader, LVAR_se_boscrashedhummer) == 0) && (GetLvar(leader, LVAR_q_bos_initiatequest_start) >= 21 && GetLvar(leader, LVAR_q_bos_initiatequest_start) <= 22) && Random(0, 8) == 0), false);
``

## Injured soldier quest value 21 < 22 -> 26
- Mapname: q_bos_medic_002.fomap / _map.fos MAP_Battleground variable
- Map id: 314
- Dialog file: se_bos_injured.fodlg
- Map Variable: LVAR_se_bosbattleground 7056
- Quest Var: LVAR_q_bos_injured 10
- Main script file: quest_bos_initiate.fos
- Spawned at worldmap.fos 
``
    _CheckSpecialEncounter(LOCATION_Battleground, (ZoneContainsFlag(zone, FLAG_BattleGround) && (GetLvar(leader, LVAR_se_bosbattleground) == 0) && (GetLvar(leader, LVAR_q_bos_initiatequest_start) >= 21 && GetLvar(leader, LVAR_q_bos_initiatequest_start) <= 22) && Random(0, 1) == 0), false);
``
Locations.cfg
```
[Area 52]
name=Battleground
size=6
visible=0
auto_garbage=0
geck_visible=0
map_0=q_bos_medic_002 314
entrance=1
#@IsInstancedQuest=1
```

## Last mission quest value 26 < 27 -> 28
# Old bunker spawn
- Rep 1600
- Function: quest_bos_initiate:r_SpawnBunker
- LOCATION_AnOldBunker 53
```
[Area 53]
name=An Old Bunker
size=6
visible=0
auto_garbage=0
geck_visible=0
map_0=q_bos_oldbunker_level1 315
map_1=q_bos_oldbunker_level2 316
map_2=q_bos_oldbunker_level3 317
entrance=1
#@IsInstancedQuest=1
```