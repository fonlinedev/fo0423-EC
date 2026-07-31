# FOnline Engine: Level Cap Expansion & Stackable Perks Feature Documentation

This document provides a comprehensive technical overview and implementation guide for expanding the maximum level cap to **Level 100** and enabling **Stackable Unspent Perks** in FOnline SDK (Version 0506-EC / 2238 / TLA variants).

---

## Technical Overview

### Objectives
1. **Extend Level Cap:** Increase the maximum obtainable character level from 30 to 100.
2. **Stackable Unspent Perks:** Ensure unspent perk points do not expire, overwrite, or reset upon leveling up or hitting level 30.
3. **Admin Telemetry & Debug Commands:** Provide a reliable, crash-safe in-game command (`~run cheats set <level> 0 0`) for Game Masters / Admins to test character progression and perk distribution.

---

## Summary of Code Modifications

### 1. `parameters.fos` (Server-Side Logic)

#### Key Changes:
* Added `#ifdef __SERVER` preprocessor guards to prevent client-side compilation errors where `Critter` is unrecognized.
* Changed `cr.StatBase[ST_UNSPENT_PERKS] = 1;` (assignment/overwrite) to `cr.StatBase[ST_UNSPENT_PERKS] += 1;` (accumulation).
* Removed hardcoded Level 30 perk wipe rules (`if(level==30) cr.StatBase[ST_UNSPENT_PERKS]=0;`).
* Updated level cap check to allow progression up to Level 100.

#### Code Snippet (`changedParam_Experience`):

```
#ifdef __SERVER
void changedParam_Experience(Critter& cr, uint, int oldValue)
{
    int exp = cr.StatBase[ST_EXPERIENCE] - oldValue;
    int levelDiff = 0;

    if(exp > 0)
    {
        exp += exp * ((cr.Perk[PE_SWIFT_LEARNER] + cr.Trait[TRAIT_LONER]) * 10) / 100;
        LogExperience(cr, exp, -2);
        uint level = cr.Stat[ST_LEVEL], oldLevel = level;

        cr.StatBase[ST_EXPERIENCE] = oldValue + exp;

        // Enforce hard level cap at Level 100
        if(level >= 100)
        {
            cr.StatBase[ST_LEVEL] = 100;
            if(_IsFollower(cr)) VerboseExperience(cr, exp, 0);
            return;
        }

        int perkUp = (cr.Trait[TRAIT_SKILLED] != 0 ? 4 : 3);
        while(true)
        {
            if(cr.StatBase[ST_EXPERIENCE] >= NextLevelNeedExp(cr))
            {
                if(level >= 100) break;

                level++;
                cr.StatBase[ST_LEVEL]++;
                AddScore(cr, SCORE_RITCH, 1);

                // Skill Point Allocation
                cr.StatBase[ST_UNSPENT_SKILL_POINTS] += 5 + cr.StatBase[ST_INTELLECT] * 2;
                if(cr.Trait[TRAIT_SKILLED] != 0) cr.StatBase[ST_UNSPENT_SKILL_POINTS] += 5;
                cr.StatBase[ST_UNSPENT_SKILL_POINTS] += cr.Perk[PE_EDUCATED] * 2;
                if(cr.StatBase[ST_UNSPENT_SKILL_POINTS] > 99) cr.StatBase[ST_UNSPENT_SKILL_POINTS] = 99;

                // HP Allocation & Stackable Perk Point Incrementation
                if(level <= 100)
                {
                    cr.StatBase[ST_MAX_LIFE] += cr.StatBase[ST_ENDURANCE] / 2;
                    if((cr.StatBase[ST_ENDURANCE] % 2 == 1) && cr.StatBase[ST_LEVEL] % 2 == 0) 
                        cr.StatBase[ST_MAX_LIFE] += 1;

                    // Increment unspent perks (+= 1 ensures perks stack)
                    if((level % perkUp) == 0)
                    {
                        cr.StatBase[ST_UNSPENT_PERKS] += 1;
                    }
                }
            }
            else break;
        }
        levelDiff = level - oldLevel;
    }

    if(_IsFollower(cr)) VerboseExperience(cr, exp, levelDiff);
}
#endif
```