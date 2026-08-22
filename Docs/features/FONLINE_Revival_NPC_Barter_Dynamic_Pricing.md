# FOnline Revival — NPC Barter & Dynamic Item Pricing

## Status

**Feature:** NPC barter / item pricing  
**Status:** Working prototype — further testing required  
**Last updated:** 2026-08-22

---

## 1. Purpose

This document records the current barter and item-pricing design used by FOnline Revival.

The goal is to support a Fallout 1-style trading system where:

- NPCs can participate in barter without necessarily having a dedicated trading script.
- An NPC's ability to carry items is the practical prerequisite for trading.
- Item base values come from item prototypes and the economy system.
- NPC buying and selling prices are intentionally asymmetric.
- NPCs can offer poor prices, and the player is free to accept or reject those offers.
- An NPC's current money should not be confused with the calculated value of an item.

This feature is important because future changes to the economy, NPC behavior, barter UI, or trader specialization must preserve these design principles unless the system is deliberately redesigned.

---

# 2. Core Design Philosophy

## 2.1 Bag capacity enables trading

The intended design is that an NPC with appropriate bag/container capacity can participate in trading.

A dedicated NPC script that explicitly implements trading should not be required merely for the NPC to appear in barter.

This allows ordinary NPCs to become potential traders without adding a specialized trading script to every NPC.

The NPC's inventory/container capacity is therefore part of the practical trader capability.

---

## 2.2 Item value and NPC wealth are different concepts

The system should distinguish between:

1. **The value of an item**
2. **The offer calculated by the barter system**
3. **The NPC's actual available money**

These are not necessarily the same number.

For example:

```text
Item base value:       $200
NPC calculated offer:   $20
```

This is valid.

The player may decide that accepting $20 for an item worth $200 is a bad deal and refuse the transaction.

The system should not turn the calculated item value into `$0` simply because the NPC does not have enough money to pay the item's full value.

---

# 3. Current Item Price Pipeline

The current economy stores a price for each item PID.

The relevant function is:

```cpp
uint GetItemPrice(uint pid)
{
    return(GetLvar(pid, LVAR_economy_prices));
}
```

The economy initializes stored prices from the item prototype cost.

The important diagnostic test used PID 34 (`.223 FMJ`):

```text
PID=34
ProtoCost=14
StoredPrice=14
```

This established that the economy price storage is working correctly.

Therefore, when `.223 FMJ` subsequently receives a different barter offer, the difference is caused by the barter calculation rather than a bad prototype price or failed economy LVAR.

---

# 4. `.223 FMJ` Test Case

PID:

```cpp
#define PID_223_FMJ (34)
```

The prototype currently reports:

```text
ProtoCost = 14
```

The economy stores:

```text
StoredPrice = 14
```

Therefore the base economic value currently being used for `.223 FMJ` is:

```text
$14 per unit
```

This was considered reasonable because .223 ammunition is relatively difficult to acquire in the intended Fallout 1-style economy.

The base value should not be increased merely because a particular NPC offers a higher or lower barter price.

---

# 5. Barter Price Asymmetry

The current design intentionally allows NPC buying and selling prices to differ significantly.

This is desirable.

For example, during testing with a particular NPC:

```text
NPC buys player's .223:
$24 × 6

NPC sells .223:
$72 × 6
```

Therefore:

```text
Player sells 6:
$24 each

Player buys 6:
$72 each
```

The NPC is charging a substantial markup when selling compared with the amount it offers when buying.

This creates a meaningful trade spread.

It also makes specialized merchants and better trading opportunities useful to the player.

---

# 6. Why Asymmetric Prices Are Desired

The system should NOT attempt to make these prices symmetrical.

A healthy barter economy can intentionally behave like:

```text
NPC purchase price
        <
item's effective market/selling price
        <
NPC selling price
```

This creates:

- economic friction,
- reasons to compare merchants,
- reasons to improve barter/reputation,
- consequences for buying from ordinary NPCs,
- meaningful value in scarce items.

The player should be allowed to accept an unfavorable trade.

---

# 7. Trader-Level Restriction and the `$0` Problem

The original barter calculation contained a trader-level restriction that could return zero when an NPC had no trader level for the item's trading category.

The relevant behavior was effectively:

```cpp
if((trader_item_level == 0 || trader_item_level > ItemLevel(item)) && sell)
{
    return 0;
}
```

This means that an NPC with trader level `0` for an item category could produce:

```text
return 0;
```

even when the item's actual economy value was non-zero.

This caused the barter UI to display `$0`.

For the intended FOnline Revival design, this behavior is too restrictive because an ordinary NPC with bag capacity should still be able to trade.

---

# 8. Trader Modifier

The existing trader modifier system provides a better mechanism for handling differences in trading expertise.

The relevant calculation is:

```cpp
uint GetTraderMod(Critter& trader, uint16 pid)
{
    int appsum = ItemLevel(pid) - GetTraderLevel(trader, TradingItemType(pid));

    return 100 - 20 * ABS(appsum);
}
```

The modifier can therefore reduce the effective price when the NPC is inexperienced with a particular item category.

This is preferable to immediately rejecting the item with a `$0` result.

Conceptually:

```text
NPC has little/no specialization
        ↓
trade still possible
        ↓
poor price
```

rather than:

```text
NPC has no specialization
        ↓
trade impossible
        ↓
$0
```

This fits the intended design better.

---

# 9. Important Distinction: `$0` vs Poor Price

A `$0` result should not automatically mean:

> "The NPC is poor."

The system should distinguish between:

### Poor offer

Example:

```text
Item value = $200
NPC offer = $20
```

This is a valid barter result.

### No valid trade

Example:

```text
NPC cannot trade this item at all
```

This is a separate rule.

### NPC has insufficient cash

This is also a separate concern.

The player's decision to accept a poor offer should not be replaced by an automatic `$0` calculation.

---

# 10. `GetItemCost()` Is the Important Calculation Layer

Both server and client use the native/item-cost calculation through wrappers.

Server:

```cpp
uint item_cost(Item& item, Critter& cr, Critter& npc, bool buy)
{
    return GetItemCost(item, cr, npc, buy);
}
```

Client:

```cpp
uint item_cost(ItemCl& item, CritterCl& chosen, CritterCl& npc, bool sell)
{
    return GetItemCost(item, chosen, npc, sell);
}
```

This means that after the prototype/economy price has been verified, problems with the displayed barter value should be investigated in the `GetItemCost()` path and its associated customization hooks.

Do not immediately modify the economy base price when the barter UI displays an unexpected number.

---

# 11. Barter Transaction Handling

The server-side barter callback is:

```cpp
bool items_barter(
    array<Item@>& saleItems,
    array<uint>& saleItemsCount,
    array<Item@>& buyItems,
    array<uint>& buyItemsCount,
    Critter& player,
    Critter& npc)
```

For traders, it performs reputation/profit processing and logs the transaction.

The relevant transaction values are calculated using:

```cpp
GetItemCost(...)
```

For example:

```cpp
uint value =
    GetItemCost(saleItems[i], player, npc, true) *
    saleItemsCount[i];
```

and:

```cpp
uint value =
    GetItemCost(buyItems[i], player, npc, false) *
    buyItemsCount[i];
```

This reinforces that `GetItemCost()` is central to the final barter values.

---

# 12. Current Successful Diagnostic

A temporary diagnostic was used to verify the economy price for PID 34.

Result:

```text
ECONOMY DEBUG: FORCING InitPrices().
ECONOMY TEST: PID=34
ECONOMY TEST: ProtoCost=14
ECONOMY TEST: StoredPrice=14
ECONOMY DEBUG: InitPrices() FINISHED.
```

This proves:

- PID 34 is valid.
- The `.223 FMJ` prototype exists.
- Prototype cost is `$14`.
- The economy LVAR is storing `$14`.
- The LVAR can be read back correctly.
- The original `$0` barter issue was not caused by the stored economy price.

The temporary forced `InitPrices()` diagnostic should eventually be removed.

`InitPrices()` should not be permanently forced on every startup if the economy event/global is intended to prevent unnecessary reinitialization.

---

# 13. Current Observed Behavior

During the latest test, the barter interface showed the NPC participating in barter even though the NPC did not have a dedicated trading script.

The tested NPC had a bag/inventory capacity.

This is an important confirmation of the desired architecture:

```text
NPC
 ├── bag capacity
 └── no dedicated trade script
          ↓
      barter available
```

The tested `.223 FMJ` transaction produced:

```text
NPC buys:
$24 × 6

NPC sells:
$72 × 6
```

This is currently considered a good result.

---

# 14. Desired Future NPC Trading Model

The intended general model is:

```text
NPC has bag capacity
        ↓
NPC can participate in barter
        ↓
Item has an economy/base value
        ↓
Barter system calculates NPC offer
        ↓
Trader skill/category/reputation can modify offer
        ↓
Player decides whether to accept
```

A dedicated trading script should only be necessary for special NPC behavior, not basic barter capability.

---

# 15. Specialization

The existing trader-level mechanics can still be used.

A specialized merchant can provide better prices for categories in which they have appropriate trader levels.

An ordinary NPC can provide worse prices.

Example conceptual hierarchy:

```text
Ordinary NPC
    ↓
poor/average offer

Specialized merchant
    ↓
better offer

High reputation + good barter
    ↓
potentially better offer
```

This preserves differentiation between NPCs without requiring every trader-capable NPC to have a unique script.

---

# 16. Testing Checklist

Future testing should verify the following.

### Basic trading

- [ ] NPC with bag capacity can barter.
- [ ] NPC without dedicated trading script can barter.
- [ ] Items can be placed on either side of the barter interface.
- [ ] Offer is not automatically `$0` merely because trader level is missing.

### Item prices

- [ ] Prototype cost is correct.
- [ ] Economy LVAR price is correct.
- [ ] `GetItemCost()` returns a non-zero value when appropriate.
- [ ] Scarce items have intentionally chosen base prices.

### Buy/sell spread

- [ ] NPC buying price is lower than NPC selling price where intended.
- [ ] Poor offers remain possible.
- [ ] Player can choose to reject unfavorable trades.
- [ ] Specialized merchants can offer better prices.

### NPC money

- [ ] NPC money/cash capacity does not incorrectly become the item's displayed value.
- [ ] Insufficient NPC money does not cause an otherwise valid calculated offer to become `$0` unless that is explicitly intended by the final transaction rules.
- [ ] Actual payment/transaction limits remain separate from displayed item valuation.

### Regression

- [ ] Existing merchants continue to work.
- [ ] Reputation modifiers still work.
- [ ] Barter skill modifiers still work.
- [ ] Trader category modifiers still work.
- [ ] Server and client calculate consistent prices.
- [ ] Barter transaction logs record the expected values.

---

# 17. Things Not to Change Without Testing

The following are now considered separate from the original `$0` issue and should not be changed casually:

### `.223 FMJ` base price

Current:

```text
$14
```

This is considered reasonable for the current economy.

### Buy/sell asymmetry

The spread between NPC buying and selling prices is intentional.

### Economy LVAR initialization

The diagnostic proved that:

```text
ProtoCost → LVAR_economy_prices
```

is working.

Changing this layer is unlikely to fix a barter calculation problem once the stored value has been verified.

### Trader modifiers

These should be preserved unless testing shows that the resulting offers are economically unreasonable.

---

# 18. Recommended Future Investigation

If a future NPC displays an unexpected `$0`, investigate in this order:

1. **Check item PID.**
2. **Check `ProtoItem.Cost`.**
3. **Check `LVAR_economy_prices`.**
4. **Check whether the NPC is allowed to barter.**
5. **Check `GetItemCost()`.**
6. **Check trader-level/category restrictions.**
7. **Check trader modifier.**
8. **Check barter modifier.**
9. **Check reputation modifier.**
10. **Only then investigate NPC cash/payment limits.**

This order prevents the economy base price from being changed to compensate for a problem that actually belongs to the barter calculation.

---

# 19. Design Summary

The current direction is:

> **Bag capacity enables basic NPC barter. Trader specialization affects the quality of the offer. Item value remains independent from NPC wealth. Buying and selling prices intentionally have a spread. The player is responsible for deciding whether an offer is good or bad.**

For the current `.223 FMJ` test:

```text
PID:                 34
Prototype cost:      $14
Economy price:       $14

Observed NPC buying:
$24 × 6

Observed NPC selling:
$72 × 6
```

The observed result is currently considered a good design outcome.

Further testing is required before treating the barter feature as final.

---

# 20. Historical Debugging Notes

The investigation initially focused on the economy initialization because the barter UI displayed `$0`.

The economy initialization contained a persistent-event condition that could skip `InitPrices()` when `GVAR_economy_event` already existed.

A diagnostic confirmed that:

```text
GVAR_economy_event already exists
InitPrices() SKIPPED
```

A temporary forced call to `InitPrices()` was then used.

The focused PID 34 test subsequently proved:

```text
ProtoCost = 14
StoredPrice = 14
```

This ruled out the economy initialization as the cause of the barter `$0`.

The investigation then moved to the barter cost calculation and trader-level restrictions.

The system was subsequently adjusted/tested so that the target NPC could participate in barter despite not having a dedicated trading script.

The resulting buy/sell values were then observed to be:

```text
Buy from player:  $24 × 6
Sell to player:   $72 × 6
```

This behavior is currently considered desirable and should be used as a reference case during future economy changes.
