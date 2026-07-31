# Water Merchant Repeatable Quest (`water_merchant`)

## Overview
The **Water Merchant Delivery System** is a dynamic, repeatable quest line. Players accept a water distribution assignment from a Quest Giver, deliver water shipments to three distinct town bar/shop owners (Den, Broken Hills, San Francisco), and return to the Quest Giver for a final reward of caps and scaling XP.

---

## Technical Specifications

### Key Mechanics & Features
* **Dynamic XP Scaling:** Delivery and quest completion rewards scale automatically based on the player's current level threshold (using a custom formula calculating $10\%$, $5\%$, or $1\%$ of the XP needed for the next level).
* **Repeatable Cycle:** Players can complete the quest, return to the quest giver, and restart the delivery loop continuously.
* **Smart Dialogue Hooks:** Delivery choices automatically disappear once completed for that specific run, preventing duplicate rewards or dialogue loops.

### Character Variable Mapping (`ST_VAR`)
The quest tracks progress directly on the player critter using internal player variable slots:

| Define Name | Variable Index | Description | Values |
| :--- | :--- | :--- | :--- |
| `LVAR_water_quest_state` | `ST_VAR0` | Master Quest State | `0`: Not Started, `1`: In Progress, `2`: All Deliveries Complete, `3`: Finished / Ready for Repeat |
| `LVAR_water_den_done` | `ST_VAR1` | Den Delivery Status | `0`: Pending, `1`: Delivered |
| `LVAR_water_bh_done` | `ST_VAR2` | Broken Hills Status | `0`: Pending, `1`: Delivered |
| `LVAR_water_sf_done` | `ST_VAR3` | San Francisco Status| `0`: Pending, `1`: Delivered |

---

## Server Configuration Guide

### 1. Script File Setup
1. Save the AngelScript logic to `scripts/water_merchant.fos`.
2. Open `scripts/scripts.cfg` and register the script module:
   ```text
   @ server module water_merchant
   ```

### 2. Dialogue Assignment (`.fodlg`)
Link the dialogue script functions in your target NPC `.fodlg` files:

#### A. Frankie (Den Target NPC - `den_frankie.fodlg`)
Ensure the entry line in Node `2` checks `d_CanDeliverDen`, and the exit from Node `17` triggers `r_DeliverDen`.

#### B. Quest Giver NPC (`water_merchant_giver.fodlg`)
Assign these script calls to your quest giver NPC dialogue nodes:
* **Check Start Eligibility:** `D _script water_merchant@d_QuestNotStarted 0`
* **Accept Quest:** `R _script water_merchant@r_StartQuest 0`
* **Check Quest In Progress:** `D _script water_merchant@d_QuestInProgress 0`
* **Check Ready to Complete:** `D _script water_merchant@d_QuestReadyToComplete 0`
* **Claim Final Reward:** `R _script water_merchant@r_CompleteQuest 0`

### C. Test it

Requires Admin Access

In-game, set your quest state to in-progress:
`~runscript water_merchant set_var 0 1 0

Mark the first two locations done:

Den
`~runscript water_merchant set_var 1 1 0

Broken Hills
`~runscript water_merchant set_var 2 1 0

Mark the final location done:
San Francisco
`~runscript water_merchant set_var 3 1 0

Run 
`~runscript water_merchant check_vars 0 0 0
State will automatically show as 2 (Ready to claim final reward)!

`~runscript water_merchant reset_vars 0 0 0
---

## Full Source Code

### `scripts/water_merchant.fos`

```cpp
#include "_macros.fos"
#include "_defines.fos"

// Character variables assigned to unused ST_VAR slots
#define LVAR_water_quest_state      (ST_VAR0)
#define LVAR_water_den_done         (ST_VAR1)
#define LVAR_water_bh_done          (ST_VAR2)
#define LVAR_water_sf_done          (ST_VAR3)

// Formula context helper: XP required for current level threshold
uint GetXPRequiredForLevel(uint level)
{
    if(level <= 1) return 0;
    return (level * (level - 1) / 2) * 1000;
}

uint GetXPForNextLevel(uint level)
{
    uint currentLevelXP = GetXPRequiredForLevel(level);
    uint nextLevelXP    = GetXPRequiredForLevel(level + 1);
    return nextLevelXP - currentLevelXP;
}

// Helper to calculate delivery XP (10% < lvl 30, 5% < lvl 40, 1% >= lvl 40)
void GrantDeliveryXP(Critter& player)
{
    uint level = player.Stat[ST_LEVEL];
    uint xpRequired = GetXPForNextLevel(level);
    uint xpReward = 0;

    if(level < 30)
    {
        xpReward = xpRequired / 10;
    }
    else if(level < 40)
    {
        xpReward = xpRequired / 20;
    }
    else
    {
        xpReward = xpRequired / 100;
    }

    if(xpReward < 50) xpReward = 50;
    player.StatBase[ST_EXPERIENCE] += xpReward;
}

// Check if player has completed all three deliveries
void CheckQuestProgress(Critter& player)
{
    if(player.Param[LVAR_water_den_done] == 1 && 
       player.Param[LVAR_water_bh_done] == 1 && 
       player.Param[LVAR_water_sf_done] == 1)
    {
        player.ParamBase[LVAR_water_quest_state] = 2; // Ready to claim final reward
    }
}

// ==========================================
// QUEST GIVER FUNCTIONS
// ==========================================

bool d_QuestNotStarted(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 0 || player.Param[LVAR_water_quest_state] == 3);
}

bool d_QuestInProgress(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 1);
}

bool d_QuestReadyToComplete(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 2);
}

bool d_QuestCompleted(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 3);
}

// Results (r_)
void r_StartQuest(Critter& player, Critter@ npc)
{
    player.ParamBase[LVAR_water_quest_state] = 1;
    player.ParamBase[LVAR_water_den_done]    = 0;
    player.ParamBase[LVAR_water_bh_done]     = 0;
    player.ParamBase[LVAR_water_sf_done]     = 0;
}

void r_CompleteQuest(Critter& player, Critter@ npc)
{
    if(player.Param[LVAR_water_quest_state] != 2) return;

    uint level = player.Stat[ST_LEVEL];
    uint xpRequired = GetXPForNextLevel(level);
    uint xpReward = 0;
    uint capsReward = 0;

    if(level < 30)
    {
        xpReward = xpRequired / 10;
        capsReward = 3000;
    }
    else if(level < 40)
    {
        xpReward = xpRequired / 20;
        capsReward = 2500;
    }
    else
    {
        xpReward = xpRequired / 100;
        capsReward = 2000;
    }

    if(xpReward < 100) xpReward = 100;

    player.StatBase[ST_EXPERIENCE] += xpReward;
    player.AddItem(PID_BOTTLE_CAPS, capsReward);

    player.ParamBase[LVAR_water_quest_state] = 3;

    if(@npc != null)
    {
        npc.Say(SAY_NORM_ON_HEAD, "Excellent work! Here is your pay and final reward.");
    }
}

// ==========================================
// DELIVERY TARGET FUNCTIONS
// ==========================================

bool d_CanDeliverDen(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 1 && player.Param[LVAR_water_den_done] == 0);
}

void r_DeliverDen(Critter& player, Critter@ npc)
{
    if(!d_CanDeliverDen(player, npc)) return;

    player.ParamBase[LVAR_water_den_done] = 1;
    GrantDeliveryXP(player);
    CheckQuestProgress(player);
}

bool d_CanDeliverBH(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 1 && player.Param[LVAR_water_bh_done] == 0);
}

void r_DeliverBH(Critter& player, Critter@ npc)
{
    if(!d_CanDeliverBH(player, npc)) return;

    player.ParamBase[LVAR_water_bh_done] = 1;
    GrantDeliveryXP(player);
    CheckQuestProgress(player);
}

bool d_CanDeliverSF(Critter& player, Critter@ npc)
{
    return (player.Param[LVAR_water_quest_state] == 1 && player.Param[LVAR_water_sf_done] == 0);
}

void r_DeliverSF(Critter& player, Critter@ npc)
{
    if(!d_CanDeliverSF(player, npc)) return;

    player.ParamBase[LVAR_water_sf_done] = 1;
    GrantDeliveryXP(player);
    CheckQuestProgress(player);
}
```

---

### Target NPC Dialogue: `dialogs/den_frankie.fodlg`

```text
[comment]

[data]
lang=engl

[dialog]
&
1 1000 None 0 #
10 1010 D _param p KARMA_SLAVER > 0 #
11 1020 D _param p ST_CHARISMA > 99 #
2 1030 @
2 2000 None 0 #
3 2010 #
3 2020 #
4 2030 #
17 2060 D _script water_merchant@d_CanDeliverDen 0 #
12 2050 D _var p q_klam_bob_thehole = 1 #
0 2040 @
3 3000 None 0 #
5 3010 #
9 3020 #
8 3030 #
0 3040 @
4 4000 None 0 #
5 4010 #
0 4020 @
5 5000 None 0 #
6 5010 D _item p PID_BOTTLE_CAPS > 9 R _item p PID_BOOZE + 1 R _script dialog@r_RemoveMoney 1 10 #
3 5020 #
7 5030 #
0 5040 @
6 6000 None 0 #
3 6010 #
0 6020 @
7 7000 None 0 #
6 7010 D _item p PID_BOTTLE_CAPS > 9 R _item p PID_BOOZE + 1 R _script dialog@r_RemoveMoney 1 10 #
3 7020 #
0 7030 @
8 8000 None 0 #
7 8010 #
3 8020 #
0 8030 @
9 9000 None 0 #
0 9010 #
0 9020 @
10 10000 None 0 #
17 10030 D _script water_merchant@d_CanDeliverDen 0 #
3 10010 #
0 10020 @
11 11000 None 0 @
12 12000 None 0 #
13 12010 @
13 13000 None 0 #
14 13010 @
14 14000 None 0 #
15 14010 @
15 15000 None 0 #
16 15010 R _item p PID_BOTTLE_CAPS + 20 @
16 16000 None 0 #
0 16010 R _var p q_klam_bob_thehole = 2 @
17 17000 None 0 #
3 17010 R _script water_merchant@r_DeliverDen 0 #
0 17020 R _script water_merchant@r_DeliverDen 0 &

[engl]
{100}{}{Frankie}
{200}{}{You see the owner of The Hole.}
{200}{}{You see Frankie, owner of The Hole.}
{210}{}{This is Frankie. He owns this joint.}
{210}{}{This appears to be the owner of this place.}
{1000}{}{Predialogue installations}
{1010}{}{Slaver}
{1020}{}{Ugly}
{1030}{}{normal}
{2000}{}{I’m Frankie and yer standing in 'The Hole.' Best damn bar for a hundred miles! Everyone crawls into The Hole when it's time to let loose. Now don't you be causing too much trouble, y’hear?}
{2010}{}{I hear ya.}
{2020}{}{Yeah, okay. Whatever.}
{2030}{}{I see why you call this place "The Hole."}
{2040}{}{Nothing today, bye.}
{2050}{}{Whiskey Bob sent me. I've got a message for you.}
{2060}{}{[Deliver Water] Here is your water delivery from the Water Merchants.}
{3000}{}{What’s yer pleasure?}
{3010}{}{Whiskey.}
{3020}{}{Sex.}
{3030}{}{Info.}
{3040}{}{Nothing today, bye.}
{4000}{}{Hey! You better shut your hole!}
{4010}{}{Just do your job and get me a drink.}
{4020}{}{I’ll just leave instead.}
{5000}{}{Whiskey is $10 a shot.}
{5010}{}{Here’s your money.}
{5020}{}{Can't afford it. Can I get something else?}
{5030}{}{Why does your whiskey cost so much more than Becky’s?}
{5040}{}{No thanks. Bye.}
{6000}{}{Anything else?}
{6010}{}{Yes.}
{6020}{}{Nope, thanks.}
{7000}{}{Those New Reno families charge an arm and a leg for their booze. I don’t know how she does it!}
{7010}{}{Too bad. I'll take a drink anyway.}
{7020}{}{I wanted something else.}
{7030}{}{I guess I'll just go buy whiskey from her. bye.}
{8000}{}{Information I do not have.}
{8010}{}{Really? How about why Becky’s drinks are so much cheaper than yours?}
{8020}{}{I thought bartenders knew everything. I want something else.}
{8030}{}{Then I’m in the wrong place. Bye.}
{9000}{}{Go talk to Sheila. Hey, Sheila, you got a customer!}
{9010}{}{Wait, that’s not what I wa.. nevermind.}
{9020}{}{Thanks. Bye.}
{10000}{}{Good day, slaver.}
{10010}{}{Right.}
{10020}{}{Sure is. I'm going to go enjoy it. Bye.}
{10030}{}{[Deliver Water] Here is your water delivery from the Water Merchants.}
{11000}{}{Get lost, you ugly sonovbitch.@@You're scaring off all my business.@@I don't care if you got the money, get the hell out.}
{12000}{}{Oh yeah? What's that old bastard want?}
{12010}{}{Uh, he wanted me to tell you the next batch of hooch is ready.}
{13000}{}{It is? Shit, that's great news. I'll send a runner over to pick it up. Thanks, pal.}
{13010}{}{Wait, there was one other thing.}
{14000}{}{There was? What?}
{14010}{}{He's, uh, charging double this time.}
{15000}{}{(Frankie's face reddens) That stingy piece of shit! He tries this every goddamn time. I give that old fucker the most business he's ever had and he... shit. I need a drink.}
{15010}{}{Can I go now? Is that it? }
{16000}{}{No! Not it ain't! Not by a long shot. You get back to Bob, tell him I'll be picking up, but at the goddamn rate we agreed on! (He sighs) Here's a couple bucks for your trouble. Don't keep me waiting.}
{16010}{}{Alright, sure thing Frankie.}
{17000}{}{Finally! We were running low on fresh water around here. Thanks, pal.}
{17010}{}{Glad to help. I had a few other questions.}
{17020}{}{No problem. Goodbye.}
```

---

### Quest Giver Dialogue: `dialogs/water_merchant_giver.fodlg`

```text
[comment]

[data]
lang=engl

[dialog]
&
1 1000 None 0 #
2 1010 D _script water_merchant@d_QuestNotStarted 0 #
10 1020 D _script water_merchant@d_QuestInProgress 0 #
20 1030 D _script water_merchant@d_QuestReadyToComplete 0 @
2 2000 None 0 #
3 2010 #
0 2020 @
3 3000 None 0 #
4 3010 R _script water_merchant@r_StartQuest 0 #
0 3020 @
4 4000 None 0 #
0 4010 @
10 5000 None 0 #
11 5010 #
0 5020 @
11 6000 None 0 #
0 6010 @
20 7000 None 0 #
21 7010 R _script water_merchant@r_CompleteQuest 0 @
21 8000 None 0 #
0 8010 &

[engl]
{100}{}{Master Merchant}
{200}{}{You see the Water Merchants' Distribution Manager.}
{210}{}{He looks busy managing manifests and water shipments across the wasteland.}
{1000}{}{Predialogue installations}
{1010}{}{Quest Not Started or Repeatable}
{1010}{}{Quest In Progress}
{1030}{}{Quest Ready To Complete}
{2000}{}{Welcome to the Water Merchants Hub. We run water supply caravans to settlements all across the region. Are you looking for work?}
{2010}{}{I'm looking for a job. Got any work available?}
{2020}{}{Just browsing. Bye.}
{3000}{}{We have three major settlements running low on purified water shipments: The Den, Broken Hills, and San Francisco. I need someone to deliver water packages to the local bar owners and suppliers in those towns.}
{3010}{}{Sounds simple enough. I'll take the job.}
{3020}{}{I'm not interested right now. Farewell.}
{4000}{}{Excellent. Head to Frankie at The Hole in the Den, the bartender in Broken Hills, and the supplier in San Francisco. Once all three deliveries are completed, report back to me for your caps and pay.}
{4010}{}{I'll get to it right away.}
{5000}{}{How are those deliveries coming along?}
{5010}{}{Where did you say I needed to deliver the water again?}
{5020}{}{Still working on it. Goodbye.}
{6000}{}{You need to deliver water shipments to Frankie in The Den, the bar owner in Broken Hills, and the supplier in San Francisco. Return to me once all three have received their shipments.}
{6010}{}{Got it. Back to work then.}
{7000}{}{Ah, you're back! Were you able to complete all three water deliveries?}
{7010}{}{Yes, all three shipments have been delivered successfully.}
{8000}{}{Outstanding work! The water route is secured for now. Here is your payment and extra compensation for your efforts. If you ever need more work, come check back with me anytime.}
{8010}{}{Thanks! I'll keep that in mind.}
```
