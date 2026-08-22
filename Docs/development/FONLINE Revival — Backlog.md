# FONLINE Revival — Backlog

## Shady Sands — Second Stage

### 1. Fix Razlo's Shelf Reputation Restriction

- [ ] Review Razlo's shelf/container interaction.
- [ ] Prevent players from accessing the shelf when they do not have sufficient reputation with Shady Sands.
- [ ] Define the required reputation threshold.
- [ ] Ensure players with sufficient reputation retain normal access.
- [ ] Verify that the restriction works both through direct interaction and any alternate interaction path.

### 2. Fix NPC Trade Item Values

- [ ] Investigate why most items currently have a value of `0` when traded to NPCs.
- [ ] Restore/fix NPC buy values for applicable items.
- [ ] Verify that players can sell usable items to NPCs.
- [ ] Ensure NPCs have enough money available for early-game purchases.
- [ ] Verify that players can accumulate enough money to recruit Ian at the beginning of the game.
- [ ] Test buying and selling with the relevant Shady Sands merchants.
- [ ] Check that item values are consistent with the intended Fallout 1 economy.

### 3. Start of Tandi Quest — Raiders Camp

**Prerequisite:** Player has completed the Radscorpion Cave.

- [ ] Add the initial Tandi quest interaction with Aradesh.
- [ ] Make Aradesh offer the quest only after the Radscorpion Cave has been completed.
- [ ] Implement the quest start/state transition.
- [ ] Begin revealing the Raiders' Camp to the player once the quest starts.
- [ ] Verify the Raiders' Camp becomes available/revealed through the intended world-map logic.
- [ ] Ensure the quest cannot incorrectly start before the Radscorpion Cave is completed.
- [ ] Test the complete progression:
  - [ ] Complete Radscorpion Cave.
  - [ ] Return to Shady Sands.
  - [ ] Speak with Aradesh.
  - [ ] Receive the Tandi quest.
  - [ ] Raiders' Camp is revealed.
  - [ ] Continue toward the Raiders' Camp.

## Priority

1. **High** — Fix NPC trade item values.
2. **High** — Implement Tandi quest start and Raiders' Camp reveal.
3. **Medium** — Fix Razlo's shelf reputation restriction.

## Notes

This backlog covers the **second stage of Shady Sands development** and should be updated as each feature is implemented and tested.
