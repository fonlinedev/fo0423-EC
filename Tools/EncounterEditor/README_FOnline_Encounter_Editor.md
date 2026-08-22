# FOnline Encounter Editor

A lightweight Windows GUI editor for managing encounter groups assigned to cells in an FOnline SDK 2238 world map.

The editor works directly with the existing `worldmap.fowm` World Editor source file and is designed to preserve compatibility with the standard FOnline WorldEditor.

## Project

This project is a custom C++ Windows GUI application built in Visual Studio.

Current solution structure:

- `EncounterEditor`
  - `ConsoleApplication1` - original console/testing tool
  - `EncounterEditorGUI` - Windows GUI editor

## Supported Files

The editor currently works with:

```text
worldmap.fowm
```

This is the editable World Editor source.

Related FOnline files:

```text
worldmap.fowm   -> World Editor source
worldmap.focwm  -> compiled game world map
worldmap_h.fos  -> encounter group definitions
```

The current editor modifies the `.fowm` file only.

It does **not** currently generate or modify `.focwm`.

## Features

### Groups

The left panel displays all encounter groups loaded from `worldmap_h.fos`.

Groups are displayed with their numeric IDs:

```text
GROUP_ARRO_Rats [11]
GROUP_ARRO_Pig_Rats [12]
GROUP_ARRO_Silver_Geckos [14]
```

The selected group also displays statistics:

```text
GROUP_ARRO_Rats
ID: 11
Cells: 86
Total quantity: 134
Min: 1
Max: 3
```

### Cells

The middle panel displays cells where the selected encounter group is assigned.

Cells show their current quantity:

```text
1,1 x2
2,0 x2
3,2 x2
```

### Cell Encounters

The right panel displays all encounter groups assigned to the currently selected cell.

Example:

```text
GROUP_FISH_Fisherman x2
GROUP_FISH_Wilder x2
GROUP_ARRO_Rats x2
GROUP_ARRO_Pig_Rats x2
GROUP_ARRO_Sm_Scorpions x2
GROUP_ARRO_Silver_Geckos x2
```

### Quantity Editing

The `-` and `+` buttons modify the selected encounter quantity.

### Add Encounter

The `Add Encounter` button opens a group selection interface.

A group can be added to the selected cell.

If the group does not already exist on the cell:

```text
GROUP_Brahmin x1
```

is added.

If the group already exists, its quantity is increased.

### Remove Encounter

`Remove Selected` removes the selected encounter assignment from the current cell.

### Save

The `Save` button writes the modified encounter assignments back to:

```text
worldmap.fowm
```

The save operation preserves the existing cell data and replaces only the encounter field.

This means terrain, locations, and other existing `.fowm` cell information are not rebuilt unnecessarily.

## `.fowm` Encounter Format

The `.fowm` format stores encounter assignments using **group names**, not numeric group IDs.

Example:

```text
GROUP_Rad_Scorpions:2,GROUP_FISH_Fisherman:2,GROUP_Giant_Rats:2
```

The editor therefore resolves the group name against `worldmap_h.fos` to obtain its numeric ID for display and internal use.

The numeric ID is not written back into the `.fowm` encounter field.

## Group Definitions

Encounter group IDs are defined in:

```text
worldmap_h.fos
```

Example:

```cpp
#define GROUP_Raiders                       (0)
#define GROUP_Rad_Scorpions                 (1)
#define GROUP_Giant_Rats                    (2)
#define GROUP_Merchant_Party                (3)
#define GROUP_Manti                         (4)
```

The editor loads these definitions and associates names with IDs.

## File Paths

The current development configuration uses:

```text
C:\Games\FOnlineServer\fo0423-EC\Tools\WorldEditor\worldmap.fowm
```

Group definitions:

```text
C:\Games\FOnlineServer\fo0423-EC\Server\scripts\worldmap_h.fos
```

Compiled world map:

```text
C:\Games\FOnlineServer\fo0423-EC\Server\maps\worldmap.focwm
```

Adjust the configured paths if the FOnline server is installed elsewhere.

## Save Safety

The editor writes the modified `.fowm` through a temporary file before replacing the original.

The intended save flow is:

```text
worldmap.fowm
      |
      v
worldmap.fowm.tmp
      |
      v
successful write
      |
      v
replace original worldmap.fowm
```

This reduces the chance of leaving the main `.fowm` file partially written if the save fails.

## Compatibility Verification

The editor has been tested against the standard FOnline WorldEditor.

A newly added encounter was written to the `.fowm` file and then recognized by the original WorldEditor.

For example:

```text
GROUP_Brahmin  1
```

appeared in the WorldEditor's Encounter Groups list after the custom editor saved the file.

This confirms that the generated `.fowm` data is compatible with the existing WorldEditor.

## Current Workflow

The recommended workflow is:

1. Open the custom Encounter Editor.
2. Select an encounter group.
3. Select a cell.
4. View the cell's encounter assignments.
5. Modify quantities, add encounters, or remove encounters.
6. Save.
7. Open the `.fowm` in the standard FOnline WorldEditor to verify the changes.
8. Use the normal FOnline WorldEditor/compiler workflow to produce the compiled `.focwm`.

Current workflow:

```text
Custom Encounter Editor
        |
        v
   worldmap.fowm
        |
        v
 FOnline WorldEditor
        |
        v
   worldmap.focwm
        |
        v
       Game
```

## Current Limitations

The current version intentionally remains simple.

Not currently implemented:

- Direct `.focwm` generation
- Direct binary `.focwm` editing
- Full World Editor replacement
- Terrain editing
- Location editing
- Weather editing
- Flags editing
- Difficulty editing
- Map visualization
- Undo/redo
- Advanced filtering
- Multi-cell batch editing

The editor focuses specifically on encounter group assignments and quantities.

## Important Design Decision

The editor does not attempt to reverse-engineer or directly generate the `.focwm` binary format at this stage.

Instead, it edits the existing `.fowm` source format and relies on the established FOnline WorldEditor/compiler process for creation of the compiled map.

This keeps the custom editor small and reduces the risk of producing an invalid compiled world map.

## Building

Open the Visual Studio solution:

```text
EncounterEditor
```

Build the GUI project:

```text
EncounterEditorGUI
```

Recommended configuration during development:

```text
Debug
x64
```

or the configuration already used by the FOnline development environment.

## Basic Testing Checklist

After making code changes, verify:

### Load

- [ ] Groups load
- [ ] Group IDs are displayed
- [ ] Cells populate
- [ ] Cell encounters populate

### Quantity

- [ ] `+` increases quantity
- [ ] `-` decreases quantity
- [ ] Cell encounter list remains visible after changing quantity

### Add

- [ ] Add Encounter opens
- [ ] A new group can be selected
- [ ] New group appears at `x1`
- [ ] Existing group quantity increments

### Remove

- [ ] Selected encounter can be removed
- [ ] Cell list refreshes correctly

### Save

- [ ] Save clears the unsaved state
- [ ] Quantity changes are written to `.fowm`
- [ ] New encounter groups are written to `.fowm`
- [ ] Removed encounters disappear from `.fowm`
- [ ] Existing terrain/location data remains intact

### Compatibility

- [ ] Standard FOnline WorldEditor opens the modified `.fowm`
- [ ] Standard WorldEditor recognizes added/modified encounters

## Example

Starting cell:

```text
GROUP_ARRO_Rats x1
```

Increase quantity:

```text
GROUP_ARRO_Rats x2
```

Save:

```text
GROUP_ARRO_Rats:2
```

Add Brahmin:

```text
GROUP_ARRO_Rats x2
GROUP_Brahmin x1
```

Save:

```text
GROUP_ARRO_Rats:2,GROUP_Brahmin:1
```

The standard FOnline WorldEditor should then display the same encounter assignments.

## Status

Current status:

**Working / usable for encounter editing**

Verified functionality:

- Group loading
- Group ID lookup
- Cell lookup
- Cell encounter display
- Quantity increase/decrease
- Add encounter
- Remove encounter
- `.fowm` saving
- Compatibility with the standard FOnline WorldEditor

Next possible development step:

```text
Generate/update worldmap.focwm
```

That should be treated as a separate feature rather than changing the currently working `.fowm` editor.
