# Release Notes - Healing & Drug System Adjustments

## Overview
This release introduces important balance adjustments and bug fixes to the character natural regeneration and medical item systems (Stimpaks and Super Stimpaks) to properly support high maximum health pools.

---

## Changes & Improvements

### 1. Natural Healing Timeout Optimization
* **File Affected:** `_defines.fos`
* **Modification:** Reduced the global natural healing cooldown interval (`HEALING_TIMEOUT`).
* **Details:** 
  * Previously, the timeout was fixed at a high duration (`REAL_SECOND(60)` standard / `REAL_SECOND(30)` with Fast Metabolism), causing excessive waiting periods between healing ticks for characters with large health pools.
  * Updated to a much more responsive interval (`REAL_SECOND(45)` standard / `REAL_SECOND(15)` with Fast Metabolism) to ensure natural regeneration feels smooth and active.

### 2. Dynamic Stimpak & Super Stimpak Scaling
* **File Affected:** `drugs.fos`
* **Modification:** Overrode flat-rate healing inside the `ProcessDrug` function for medical items during their instant effect stage (`stage == 0`).
* **Details:**
  * **Regular Stimpaks (`PID_STIMPAK`):** No longer rely on static low-value configuration tables. They now dynamically restore **one-third (1/3) of the character's maximum health pool** (`ST_MAX_LIFE`), ensuring scaling effectiveness at higher levels.
  * **Super Stimpaks (`PID_SUPER_STIMPAK`):** Re-engineered to completely restore the character to **full health** instantly by calculating the exact HP deficit up to `ST_MAX_LIFE`.

---

## Summary of Benefits
* Character survival mechanics are now balanced around characters with 500+ HP.
* Natural recovery provides consistent, regular health pacing.
* Medical injectables (Stimpaks/Super Stimpaks) have distinct, meaningful roles matching high-tier gameplay expectations.
