# FOnline Encounter Editor

A small utility for removing encounter groups from an FOnline 2238 World Editor `.fowm` map file.

The tool works directly with the World Editor's `.fowm` source file. The World Editor is responsible for compiling the `.fowm` into the server's `.focwm` file.

## Features

- Reads paths from `EncounterEditor.cfg`
- Loads encounter group definitions from `worldmap_h.fos`
- Searches the `.fowm` for a selected `GROUP_*`
- Shows how many cells and encounter entries will be affected
- Asks for confirmation before making changes
- Creates a `.backup` copy of the `.fowm`
- Removes only the selected encounter group
- Leaves all other encounter groups unchanged
- Uses the World Editor's normal `.fowm` → `.focwm` workflow

## Folder Structure

A distribution folder can look like this:

```text
EncounterEditor/
├── EncounterEditor.exe
├── EncounterEditor.cfg
└── README.md
```

The `.exe` and `.cfg` should be in the same directory.

## Configuration

Edit `EncounterEditor.cfg` before using the tool.

Example:

```ini
FOWM_PATH=C:\Games\FOnlineServer\fo0423-EC\WorldEditor\worldmap.fowm
FOCWM_PATH=C:\Games\FOnlineServer\fo0423-EC\Server\maps\worldmap.focwm
GROUPS_PATH=C:\Games\FOnlineServer\fo0423-EC\Server\scripts\worldmap_h.fos
```

### FOWM_PATH

Path to the World Editor's source map:

```text
worldmap.fowm
```

This is the file the tool modifies.

### FOCWM_PATH

Path where the World Editor generates the compiled:

```text
worldmap.focwm
```

The tool uses this path for status information. It does not compile the `.fowm` itself.

### GROUPS_PATH

Path to:

```text
worldmap_h.fos
```

This file contains the `GROUP_*` definitions and their numeric IDs.

For example:

```cpp
#define GROUP_Rad_Scorpions (1)
#define GROUP_MOD_Moonshiners (42)
```

## Usage

1. Close the World Editor before modifying the map.
2. Make sure `EncounterEditor.cfg` contains the correct paths.
3. Run `EncounterEditor.exe`.
4. Enter the exact `GROUP_*` name you want to remove.

Example:

```text
GROUP_MOD_Moonshiners
```

5. The tool searches the `.fowm` and reports the affected cells.
6. Review the number of affected cells and entries.
7. Enter `y` to confirm.

Example:

```text
Remove GROUP_MOD_Moonshiners from the FOWM? (y/n): y
```

8. The tool creates:

```text
worldmap.fowm.backup
```

9. The selected encounter group is removed from the `.fowm`.

## Important: World Editor Compilation

The tool does **not** generate the `.focwm` file.

The normal workflow is:

```text
worldmap.fowm
      |
      v
EncounterEditor
      |
      v
modified worldmap.fowm
      |
      v
World Editor
      |
      v
worldmap.focwm
      |
      v
FOnline Server
```

After modifying the `.fowm`, open/save the map with the World Editor so it can regenerate/update the `.focwm`.

## Example

Before:

```text
GROUP_Giant_Rats:1,GROUP_Manti:1,GROUP_MOD_Moonshiners:4,GROUP_Rad_Scorpions:1
```

Remove:

```text
GROUP_MOD_Moonshiners
```

After:

```text
GROUP_Giant_Rats:1,GROUP_Manti:1,GROUP_Rad_Scorpions:1
```

Only the selected group is removed. Other encounters remain unchanged.

## Backup

Before modifying the `.fowm`, the tool creates:

```text
worldmap.fowm.backup
```

If you need to undo the operation, close the World Editor and restore the backup over the modified `.fowm`.

The project can also be kept under Git for additional version control.

## Safety Recommendations

- Close the World Editor before running the tool.
- Check the configured `FOWM_PATH` carefully.
- Review the affected-cell count before confirming.
- Keep the generated `.backup` until you have verified the result.
- Use Git or another version-control system when possible.
- Test the resulting map in the World Editor before starting the server.

## Troubleshooting

### "Could not open config"

Make sure:

```text
EncounterEditor.cfg
```

is in the same folder as:

```text
EncounterEditor.exe
```

The program prints the exact configuration path it is trying to open.

### "FOWM file does not exist"

Check `FOWM_PATH` in `EncounterEditor.cfg`.

The path must point to the actual World Editor `.fowm` file.

### "Groups file does not exist"

Check `GROUPS_PATH`.

It should point to:

```text
Server\scripts\worldmap_h.fos
```

### "GROUP not found"

The group name must match the definition in `worldmap_h.fos`.

For example:

```text
GROUP_MOD_Moonshiners
```

not:

```text
Moonshiners
```

### World Editor still shows the old encounters

Make sure you opened the same `.fowm` specified by `FOWM_PATH`.

After modifying the `.fowm`, let the World Editor compile/save it to the configured `.focwm` location.

Also make sure the World Editor is not displaying a cached or different copy of the map.

## Requirements

- Windows
- FOnline 2238 World Editor / server files
- A valid `worldmap.fowm`
- The corresponding `worldmap_h.fos`
- `EncounterEditor.cfg`

## Notes

The tool is intended for FOnline 2238 development and testing.

It modifies encounter group assignments in the World Editor's `.fowm` source rather than directly editing the compiled `.focwm`.
