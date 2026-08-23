# DialogEditor — ComboBox Editing Patch

## Overview

This document records the IL changes made to the original FOnline DialogEditor to make selected ComboBox fields editable.

The original controls use:

```csharp
ComboBoxStyle.DropDownList
```

which prevents manually typing or pasting values. The patch changes selected controls to:

```csharp
ComboBoxStyle.DropDown
```

This keeps the dropdown list while allowing direct text input.

## Original executable

```text
C:\Games\FOnlineServer\fo0423-EC\Tools\DialogEditor\DialogEditor.exe
```

Assembly information:

```text
DialogEditor
Version 0.5.4863.1315
Architecture: x86
Runtime: .NET Framework 4.0
```

The executable was edited with dnSpyEx.

---

# 1. Demand Variables — `cbDemandVars`

## Location

`DialogEditor.Form1.InitializeComponent()`

The relevant IL pattern was:

```text
ldarg.0
ldfld    DialogEditor.Form1::cbDemandVars
ldc.i4.2
call     ComboBox::set_DropDownStyle(...)
```

The only instruction changed was:

```diff
- ldc.i4.2
+ ldc.i4.1
```

This changes the effective setting from:

```csharp
this.cbDemandVars.DropDownStyle = ComboBoxStyle.DropDownList;
```

to:

```csharp
this.cbDemandVars.DropDownStyle = ComboBoxStyle.DropDown;
```

## Verification

The patch was tested successfully: `cbDemandVars` became editable.

This enables:

- typing values directly;
- copying values;
- pasting values;
- editing the text without being restricted to existing list entries.

---

# 2. Result Item — `cbResultItem`

## Location

`DialogEditor.Form1.InitializeComponent()`

The relevant instructions were found around IL offsets `45B5`–`45BB`:

```text
ldfld    ... DialogEditor.Form1::cbResultItem
ldc.i4.2
call     ... ComboBox::set_DropDownStyle(...)
```

The exact change is:

```diff
- ldc.i4.2
+ ldc.i4.1
```

Resulting sequence:

```text
ldarg.0
ldfld    ... DialogEditor.Form1::cbResultItem
ldc.i4.1
call     ... ComboBox::set_DropDownStyle(...)
```

This changes the effective C# setting from:

```csharp
ComboBoxStyle.DropDownList
```

to:

```csharp
ComboBoxStyle.DropDown
```

The purpose is the same as `cbDemandVars`: allow manual typing and copy/paste.

---

# IL Mapping

For these ComboBoxes, the important pattern is:

```text
ldfld ...::TARGET_COMBOBOX
ldc.i4.2
call ... ComboBox::set_DropDownStyle(...)
```

The style value maps as follows:

```text
ldc.i4.2  -> ComboBoxStyle.DropDownList
ldc.i4.1  -> ComboBoxStyle.DropDown
```

Therefore the patch is simply:

```diff
- DropDownList
+ DropDown
```

at the IL level.

---

# Important: What Was Not Changed

Do not confuse the style value with the dropdown height.

For example:

```text
ldc.i4 0x190
call ComboBox::set_DropDownHeight(...)
```

`0x190` equals `400`, so this corresponds to:

```csharp
ComboBox.DropDownHeight = 400;
```

That setting was deliberately left unchanged.

Also, do not blindly replace every `ldc.i4.2` in `InitializeComponent()`. The method contains many controls and integer constants.

Only modify the `ldc.i4.2` that immediately belongs to the desired ComboBox's:

```text
set_DropDownStyle(...)
```

call.

---

# Why This Was Needed

The DialogEditor variable lists are not conveniently sorted. Finding variables through the existing dropdown can therefore be difficult.

The original `DropDownList` configuration also prevents entering a value that is not already present in the list.

Changing the controls to `DropDown` provides an immediate usability improvement:

- type a variable/value;
- paste a variable/value;
- edit the text directly;
- still open and use the existing dropdown list.

This is a small, targeted usability patch and does not require rebuilding the original application from source.

---

# Future Improvement: Sort Variables

Sorting the variable list would be a larger but useful improvement.

A likely place to investigate is:

```text
DialogEditor.Form1.UpdateVarsList()
```

The desired flow would be:

```text
Load variables
    ↓
Sort variables
    ↓
Populate cbDemandVars
```

The sorting work should be treated as a separate patch rather than modifying the large `InitializeComponent()` method.

Possible future goal:

```text
Alphabetical variable list
+
Editable ComboBox
+
Copy/paste support
```

---

# Backup / Recovery

Keep the original executable untouched.

Recommended arrangement:

```text
DialogEditor.exe
DialogEditor_patched.exe
```

The original should remain available so the patched version can be replaced immediately if a future modification causes a problem.

---

# Successful dnSpyEx Workflow

The working process was:

1. Open `DialogEditor.exe` in dnSpyEx.
2. Open `DialogEditor.Form1`.
3. Locate `InitializeComponent()`.
4. Search for the target ComboBox name.
5. Find its field load (`ldfld`).
6. Confirm the following sequence contains:
   ```text
   ldc.i4.2
   call ... ComboBox::set_DropDownStyle(...)
   ```
7. Change only that `ldc.i4.2` to `ldc.i4.1`.
8. Apply the IL edit.
9. Save the modified module.
10. Run the patched executable and test the control.

---

# Current Status

- [x] `cbDemandVars` made editable.
- [x] Copy/paste workflow enabled for `cbDemandVars`.
- [x] `cbResultItem` identified and patched using the same technique.
- [x] Dropdown height settings left unchanged.
- [ ] Investigate alphabetical sorting of variables.
- [ ] Investigate `UpdateVarsList()` for the variable population logic.
- [ ] Consider additional ComboBoxes that would benefit from manual input.

## Patch Summary

| Control | Original IL | Patched IL | Result |
|---|---|---|---|
| `cbDemandVars` | `ldc.i4.2` | `ldc.i4.1` | Editable demand-variable field |
| `cbResultItem` | `ldc.i4.2` | `ldc.i4.1` | Editable result-item field |

Equivalent conceptual C# change:

```diff
- ComboBoxStyle.DropDownList
+ ComboBoxStyle.DropDown
```
