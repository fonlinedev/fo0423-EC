# Release Notes — Worldmap Caravan Encounter Odds Fix

**Date:** August 1, 2026  
**Module:** `worldmap.fos` (`FindEncounter`)  
**Status:** Successfully Resolved & Deployed  

---

## Overview

This release addresses an issue where worldmap encounter calculations for standard player groups and caravans were using mismatched or hardcoded static odds, causing caravan encounter chances to bottleneck consistently (typically between 22%–25%). 

By correctly passing the evaluated `caravan` state into the zone's chance-evaluation function, encounter odds now dynamically reflect whether a group is travelling as a standard party or a caravan.

---

## Key Changes & Fixes

1. **Early Caravan Context Declaration (`IsCaravanLeader`)**
   * Moved the caravan evaluation logic (`bool caravan = IsCaravanLeader(leader)`) to the beginning of the `FindEncounter` function execution block.
   * This ensures the caravan status flag is globally scoped and available for all early validation checks, special encounter lookups, and log routines.

2. **Dynamic `zone.GetChance(caravan)` Integration**
   * Replaced unparameterized `zone.GetChance()` calls with `zone.GetChance(caravan)` throughout the encounter check, special encounter handling, and debugging admin logs.
   * Standard solo/normal groups now correctly pull their lower, proper non-caravan encounter rates, while caravans correctly maintain their dedicated modifiers.

3. **Codebase Clean-up & Syntax Stability**
   * Resolved all preceding compiler issues related to mismatched scoping, variable re-declarations, missing function closure braces, and syntax pollution.

---

## Verification & Results

* **Server Compilation:** The `worldmap` module and all dependent sub-modules (`main`, `globalmap_group`, `trader`, `guard`, `caravans`, etc.) now build and bind successfully without errors.
* **In-Game Behavior:** Players traveling independently report noticeably lower and more balanced encounter odds, while caravans retain their reliable specialized encounter scaling.
