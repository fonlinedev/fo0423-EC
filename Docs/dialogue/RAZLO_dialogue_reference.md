# RAZLO — Fallout 1 Dialogue Reference

## 0. CHARACTER DESCRIPTION

```text
100
"You see Razlo."
```

Character role established by the information section:

```text
1109
"That is me. I am the doctor for Shady Sands."
```

---

# 1. MAIN DIALOGUE / ENTRY

```text
TALK TO RAZLO
|
+-- [INITIAL / NORMAL ENTRY]
|     |
|     +-- 120
|     |   "I'm Razlo. Can I help you?"
|     |      |
|     |      +-- 121
|     |      |   "Not right now, but it was nice meeting you."
|     |      |      |
|     |      |      +-- [END]
|     |      |
|     |      +-- 122
|     |      |   "I need healing."
|     |      |      |
|     |      |      +-- HEALING BRANCH
|     |      |
|     |      +-- [OTHER QUEST / SPECIAL BRANCHES]
|     |
|     +-- 104
|         "You look distraught. Are you in need of assistance?"
|            |
|            +-- 105
|            |   "Tandi has been kidnapped."
|            |      |
|            |      +-- TANDI BRANCH
|            |
|            +-- [OTHER RESPONSE]
|
+-- [RETURN VISIT]
|     |
|     +-- 131
|     |   "Hello, again. How can I be of service?"
|     |      |
|     |      +-- 132
|     |      |   "Hello again. Just stopping by."
|     |      |      |
|     |      |      +-- 135
|     |      |          "It was nice seeing you again. Have a good day."
|     |      |             |
|     |      |             +-- [END]
|     |      |
|     |      +-- 133
|     |          "I need healing."
|     |             |
|     |             +-- HEALING BRANCH
|     |
|     +-- 136
|         "Of course. It's you again. Now what?"
|            |
|            +-- 137
|            |   "Nothing."
|            |      |
|            |      +-- [END]
|            |
|            +-- 138
|                "I need healing."
|                   |
|                   +-- HEALING BRANCH
|
+-- [SUSPICIOUS / NEGATIVE ENTRY]
|     |
|     +-- 128
|         "Hmmm, I don't like your looks. I hope that you have come
|          for business, and will be moving on shortly . . ."
|            |
|            +-- 129
|            |   "No need to get defensive, I'm just looking around"
|            |
|            +-- 130
|            |   "Help, I need some healing."
|            |
|            +-- [END / MOVE ON]
|
+-- [LATE / SPECIAL ENTRY]
      |
      +-- 163
          "It's late. Come back in the morning."
             |
             +-- 164
             |   "But I need your help."
             |      |
             |      +-- 168
             |          "Hnn?! What is that? Great, I get awakened by the great
             |           mumbling beast of the north. Come back in the morning."
             |
             +-- 165
             |   "I'm trying to kill some radscorpions. Do you have anything
             |    to help?"
             |      |
             |      +-- RADSCORPION BRANCH
             |
             +-- 166
             |   "I'm looking for Tandi. Can you help?"
             |      |
             |      +-- TANDI BRANCH
             |
             +-- 167
                 "Hnn."
                    |
                    +-- 168
```

---

# 2. TANDI — INITIAL RESPONSE

## NODE 105

```text
105
"Tandi has been kidnapped."
```

Possible responses:

```text
105
|
+-- 107
|   "Tandi is gone? That is a terrible thing. I hope they find her."
|      |
|      +-- [END / SYMPATHY]
|
+-- 108
|   "Hmm, Tandi is gone? I find this hard to believe, but maybe
|    she ran away with Seth. I know that they have been eyeing
|    each other lately. Aradesh was always a little bit of a
|    prude, perhaps it was too much for her finally."
|      |
|      +-- 109
|      |   "Hmm, I didn't think of that."
|      |      |
|      |      +-- [END / CONTINUE]
|      |
|      +-- 110
|          "I don't think so, I think that she was kidnapped."
|             |
|             +-- 112
|                 "How awful! If that is the case, I hope she is not hurt.
|                  Who is looking for her?"
|                    |
|                    +-- 113
|                    |   "Almost everyone."
|                    |      |
|                    |      +-- 115
|                    |          "I hope she is found before it is too late."
|                    |
|                    +-- 114
|                        "Many people, including myself."
|                           |
|                           +-- 116
|                               "If she is hurt, it could be bad. Here, take these . . .
|                                I hope that you won't need them."
|                                  |
|                                  +-- [GIVE / PROVIDE ITEMS]
|
+-- 111
    "Perhaps, I'll go talk to some other people."
       |
       +-- [END]
```

---

# 3. TANDI — ALTERNATE REACTION

```text
108
"Hmm, Tandi is gone? I find this hard to believe, but maybe
she ran away with Seth. I know that they have been eyeing
each other lately. Aradesh was always a little bit of a
prude, perhaps it was too much for her finally."

    |
    +-- 109
    |   "Hmm, I didn't think of that."
    |      |
    |      +-- [END / CONTINUE]
    |
    +-- 110
        "I don't think so, I think that she was kidnapped."
           |
           +-- 112
               "How awful! If that is the case, I hope she is not hurt.
                Who is looking for her?"
```

Duplicate / alternate player lines:

```text
194
"I don't think so, I think that she was kidnapped."

195
"Perhaps, I'll go talk to some other people."
```

These appear to be alternate/reused versions of the same Tandi branch and should be checked against the script for their exact conditions.

---

# 4. TANDI — PLAYER IS SEARCHING

```text
112
"How awful! If that is the case, I hope she is not hurt.
Who is looking for her?"

    |
    +-- 113
    |   "Almost everyone."
    |      |
    |      +-- 115
    |          "I hope she is found before it is too late."
    |
    +-- 114
        "Many people, including myself."
           |
           +-- 116
               "If she is hurt, it could be bad. Here, take these . . .
                I hope that you won't need them."
```

The `116` branch appears to be the **support/help branch**, where Razlo gives the player something useful.

---

# 5. TANDI — AFTER RESCUE

## NODE 118

```text
118
"I heard that you helped save Tandi. Congratulations
and thank you. Anytime you need healing, come here."

    |
    +-- [END / NORMAL HEALING ACCESS]
```

This represents the **post-Tandi-rescue state**.

---

# 6. TANDI — IF TANDI IS DEAD / LOST

## NODE 119

```text
119
"My grief knows no end. Tandi, the only child of my
good friend, is gone. I cannot work right now."

    |
    +-- [NO NORMAL HEALING]
    +-- [END]
```

This is a major state-dependent branch:

```text
TANDI STATE
|
+-- TANDI MISSING
|     → Razlo discusses kidnapping
|
+-- TANDI RESCUED
|     → 118
|     → Razlo is available for healing
|
+-- TANDI DEAD / LOST
      → 119
      → Razlo refuses / cannot work
```

---

# 7. HEALING — MAIN ENTRY

```text
122
"I need healing."

    ↓

[HEALING CHECKS]
```

Possible introductory responses:

```text
124
"Hmmm. I think I've seen Brahmin with more brain cells
then you. Well, let's see if you're wounded."

125
"Well, you seem fine to me. Perhaps later, then?"

126
"Let's see here."

127
"I have already given you as much as I can. Good luck."

130
"Help, I need some healing."

133
"I need healing."

138
"I need healing."

143
"I need healing."

169
"Are you hurt?"
```

The exact selection among these lines depends on the game/script state and should be verified in `RAZLO.FOS` / the corresponding NPC script.

---

# 8. HEALING — PLAYER IS NOT WOUNDED

```text
169
"Are you hurt?"

    |
    +-- 170
    |   "Yes."
    |      |
    |      +-- [HEALING CHECK]
    |
    +-- 171
        "No."
           |
           +-- 172
               "You will be, if you keep me from my sleep. Now come
                back in the morning."
```

Another non-wounded response:

```text
125
"Well, you seem fine to me. Perhaps later, then."

    ↓

[END]
```

And:

```text
175
"You're barely hurt. I suggest a good nights sleep and
some rest; you will heal naturally over time."

    ↓

[END]
```

Alternative:

```text
176
"You're barely hurt, you wimp. Come back in the morning."

    ↓

[END]
```

---

# 9. HEALING — MINOR INJURY

```text
175
"You're barely hurt. I suggest a good nights sleep and
some rest; you will heal naturally over time."

    ↓

[END]
```

Alternative:

```text
176
"You're barely hurt, you wimp. Come back in the morning."

    ↓

[END]
```

---

# 10. HEALING — $25

```text
177
"Ouch, that does look a little painful. That will be $25."

    |
    +-- 178
    |   "OK, here you go."
    |      |
    |      +-- [PAY $25]
    |      +-- [HEAL]
    |
    +-- 179
        "I don't have enough money."
           |
           +-- [CANNOT PAY]
```

---

# 11. HEALING — $50

```text
180
"Oh Great Brahmin! That is very, very serious. It will
cost $50."

    |
    +-- 181
    |   "OK, here you go."
    |      |
    |      +-- [PAY $50]
    |      +-- [HEAL]
    |
    +-- 182
        "I don't have enough money."
           |
           +-- [CANNOT PAY]
```

---

# 12. HEALING — $100

```text
183
"How can you still be walking?! This looks extremely
serious. We'll need some special prayers for this one
and a lot of luck. Not to mention $100."

    |
    +-- 184
    |   "OK, here you go."
    |      |
    |      +-- [PAY $100]
    |      +-- [HEAL]
    |
    +-- 185
        "I don't have enough money."
           |
           +-- [CANNOT PAY]
```

---

# 13. HEALING — NOT ENOUGH MONEY

```text
186
"You don't have enough money! I'm sorry, but my work ethic
doesn't permit me to heal for free, you know. Come back
when you have enough money."

    |
    +-- 187
    |   "Can you take something in trade?"
    |      |
    |      +-- [TRADE / SCRIPT CHECK]
    |
    +-- 188
        "Thanks for nothing."
           |
           +-- [END]
```

---

# 14. HEALING — OTHER MEDICAL RESPONSES

```text
127
"I have already given you as much as I can. Good luck."

    ↓

[END]


149
"Here, let's see."

    ↓

150
"Yep, just as I thought. Let me get the cure."

    ↓

151
"You seem fine, now. Be careful in the future."

    ↓

[END]


196
"This will take a little bit of time. Come, relax in
the back. You might have a slight headache from the
junja juice, as well. Come, come . . ."

    ↓

[HEALING PROCESS]
```

---

# 15. RADSCORPION INFORMATION

## NODE 145

```text
145
"What do you know about Radscorpions?"

    ↓

153
"Not too much. They seem to be extremely large versions
of the north American Emperor scorpion. Contrary to my
medical knowledge, their poison has grown more potent,
not diluted, as I would expect. Seth has been hunting them,
which helps some of my tests."

    |
    +-- 154
    |   "And how such a large
    |    creature can even be possible by natural evolution,
    |    or even radiation induced mutation is beyond me."
    |      |
    |      +-- 155
    |          "Thanks."
    |
    +-- 156
        "You seem well educated. What are you doing here?"
           |
           +-- 160
               "Doc Morbid in Junktown taught me some. And just
                because we live in stone huts, does not mean we are
                less intelligent or educated. Have a nice day."
```

---

# 16. RADSCORPION POISON

## NODE 191

```text
191
"I need something for their poison."

    ↓

192
"If I had a sample of their poison, I could create
a cure."
```

This establishes the core quest condition:

```text
PLAYER NEEDS ANTIDOTE
        ↓
RAZLO NEEDS RADSCORPION POISON SAMPLE
        ↓
PLAYER MUST PROVIDE SAMPLE
        ↓
RAZLO CAN CREATE CURE
```

---

# 17. RADSCORPION POISON SAMPLE

## NODE 146

```text
146
"I have a sample of the Radscorpion poison, can you do
something with it?"

    ↓

149
"Here, let's see."

    ↓

150
"Yep, just as I thought. Let me get the cure."

    ↓

151
"You seem fine, now. Be careful in the future."

    ↓

[ANTIDOTE CREATED / HEALING RESULT]
```

---

# 18. RADSCORPION POISON — CANNOT CURE

## NODE 148

```text
148
"Sorry, I can't cure any poisons. I don't have the
proper ingredients to make a decent anti-venom."

    ↓

[END / NEEDS BETTER MATERIAL]
```

---

# 19. RADSCORPION SAC

## NODE 158

```text
158
"They seem to be sensitive to light. Nocturnal. If I
could get a sample of their poison, especially the
venom production sac located in their tail, I could
create an anti-venom."

    |
    +-- 159
        "Thanks."
           |
           +-- [END]
```

## NODE 161

```text
161
"Let me see the sac! Yes, I can do something with
this."

    ↓

162
"Here, take this as a free sample. It seems to work
well against radscorpion stings, of course, but it
shows much potential as a general anti-venom and
poison cure."

    ↓

[ANTIDOTE / MEDICAL ITEM RESULT]
```

---

# 20. RADSCORPION QUEST — PLAYER WANTS HELP KILLING THEM

```text
165
"I'm trying to kill some radscorpions. Do you have anything
to help?"

    ↓

190
"Why would I be able to help you kill radscorpions?
I'm a doctor, not a warrior. For that you should see
Aradesh, or even Tandi, or her boyfriend, Seth. Leave
me."
```

Alternative useful branch:

```text
165
"I'm trying to kill some radscorpions. Do you have anything
to help?"

    ↓

191
"I need something for their poison."

    ↓

192
"If I had a sample of their poison, I could create
a cure."
```

The exact path between these nodes is script-controlled.

---

# 21. TANDI INFORMATION — SPECIAL / LATE RESPONSE

```text
166
"I'm looking for Tandi. Can you help?"

    ↓

[TANDI INFORMATION BRANCH]
```

Possible late-night response:

```text
168
"Hnn?! What is that? Great, I get awakened by the great
mumbling beast of the north. Come back in the morning."

    ↓

[END]
```

---

# 22. LATE-NIGHT DIALOGUE

```text
163
"It's late. Come back in the morning."

    |
    +-- 164
    |   "But I need your help."
    |      |
    |      +-- 168
    |          "Hnn?! What is that? Great, I get awakened by the great
    |           mumbling beast of the north. Come back in the morning."
    |             |
    |             +-- [END]
    |
    +-- 165
    |   "I'm trying to kill some radscorpions. Do you have anything
    |    to help?"
    |      |
    |      +-- [RADSCORPION BRANCH]
    |
    +-- 166
    |   "I'm looking for Tandi. Can you help?"
    |      |
    |      +-- [TANDI BRANCH]
    |
    +-- 167
        "Hnn."
           |
           +-- 168
```

---

# 23. LOW-INTELLIGENCE / SPECIAL RESPONSES

```text
106
"Hng, Tanny boo."

    ↓

107 / 108
[ALTERNATE TANDI RESPONSE]


123
"Hnngh!"

    ↓

124
"Hmmm. I think I've seen Brahmin with more brain cells
then you. Well, let's see if you're wounded."


134
"Ug!"

    ↓

135
"It was nice seeing you again. Have a good day."


139
"Luki!"

    ↓

140
"Of course you do."


147
"Luki!"

    ↓

148
"Sorry, I can't cure any poisons. I don't have the
proper ingredients to make a decent anti-venom."


167
"Hnn."

    ↓

168
"Hnn?! What is that? Great, I get awakened by the great
mumbling beast of the north. Come back in the morning."
```

---

# 24. POST-ANTIDOTE RESULT

```text
300
"You gain 250 experience points for helping make the antidote."

    ↓

[+250 XP]
```

This is an explicit result/message associated with the antidote quest.

---

# 25. RADSCORPION SAC / GIFT BRANCH

```text
301
"K, me giv?"

    |
    +-- 302
    |   "Nunga."
    |
    +-- 303
    |   "Scorpin, Scorpin?"
    |
    +-- 304
        "My giv dis?"
           |
           +-- 305
               "How about a Radscorpion tail instead?"
                  |
                  +-- 306
                      "Let me see the sac! Yes, I can do something with
                       this, as well as heal you for this great gift."
                         |
                         +-- 307
                             "This will take a little bit of time. Come, relax in
                              the back, while I work with the sac. You might have a slight
                              headache from the junja juice, as well. Come, come . . ."
                                |
                                +-- [ANTIDOTE / HEALING / QUEST RESULT]
```

---

# 26. ITEM / THEFT REACTION

```text
308
"Please do not take my things."

    ↓

[STOP / NEGATIVE REACTION]
```

---

# 27. GENERIC GOODBYE / RETURN

```text
121
"Not right now, but it was nice meeting you."

    ↓

[END]


132
"Hello again. Just stopping by."

    ↓

135
"It was nice seeing you again. Have a good day."

    ↓

[END]


137
"Nothing."

    ↓

[END]


142
"No."

    ↓

[END]


155
"Thanks."

    ↓

[END]


159
"Thanks."

    ↓

[END]


188
"Thanks for nothing."

    ↓

[END]
```

---

# 28. INFORMATION KEYWORDS

```text
1000  Khans
1001  Garl
1002  Junktown
1003  Shady
1004  Hub
1005  Water
1006  Tandi
1007  Aradesh
1008  Seth
1009  Razlo
1010  Sands
```

---

# 29. INFORMATION ANSWERS

## KHANS

```text
1000
"Khans"

    ↓

1100
"They are a lawless group who pretend to be some people from the
far past."
```

## GARL

```text
1001
"Garl"

    ↓

1101
"He leads the Khans.  He is one of the most dangerous men that
has ever set eyes on Shady Sands.  He leads the raider clans with a
heavy fist."
```

## JUNKTOWN

```text
1002
"Junktown"

    ↓

1102
"It's 4-6 days to the southwest.  Good trading occasionally, but
more often than not, they try to stiff us."
```

## SHADY

```text
1003
"Shady"

    ↓

1103
"You're standing in it."
```

## HUB

```text
1004
"Hub"

    ↓

1104
"Much farther to the south than Junktown.  The Hub is the largest
city I've heard about.  Must be a thousand people there, at least."
```

## WATER

```text
1005
"Water"

    ↓

1105
"Sorry, I can't help you."
```

## TANDI

```text
1006
"Tandi"

    ↓

1106
"She is the daughter of Aradesh.  Some day she could lead us, but
she must learn to temper her ways."
```

## ARADESH

```text
1007
"Aradesh"

    ↓

1107
"He is the leader of our community, and a true hero.  You can always
find him at work in the communal hall."
```

## SETH

```text
1008
"Seth"

    ↓

1108
"He works with Aradesh to make sure we are safe."
```

## RAZLO

```text
1009
"Razlo"

    ↓

1109
"That is me. I am the doctor for Shady Sands."
```

## SANDS

```text
1010
"Sands"

    ↓

1110
"You're standing in it."
```

---

# 30. INFORMATION / UNKNOWN RESPONSES

```text
266
"I don't know."

267
"I can't really say I have heard of that."

268
"Hm, no. No, I have not heard of that."
```

These are generic information responses and may be used by the script for topics not specifically handled.

---

# 31. COMPLETE QUEST-STATE VIEW

```text
RAZLO
|
+-- [FIRST VISIT]
|     |
|     +-- Normal introduction
|     +-- Tandi discussion
|     +-- Healing
|
+-- [RETURN VISIT]
|     |
|     +-- "Hello, again. How can I be of service?"
|     +-- Healing
|     +-- Goodbye
|
+-- [TANDI MISSING]
|     |
|     +-- Player asks about Tandi
|     |      |
|     |      +-- Sympathy
|     |      +-- Seth theory
|     |      +-- Confirm kidnapping
|     |             |
|     |             +-- Razlo wishes her found
|     |             +-- Gives useful items
|     |
|     +-- Late-night Tandi question
|            |
|            +-- Come back in morning
|
+-- [TANDI RESCUED]
|     |
|     +-- 118
|          "I heard that you helped save Tandi..."
|             |
|             +-- Razlo available for healing
|
+-- [TANDI DEAD / LOST]
|     |
|     +-- 119
|          "My grief knows no end..."
|             |
|             +-- Razlo cannot work
|
+-- [RADSCORPION PROBLEM]
|     |
|     +-- Ask about Radscorpions
|     |      |
|     |      +-- Scientific explanation
|     |
|     +-- Need poison cure
|     |      |
|     |      +-- Need poison sample
|     |      |
|     |      +-- Need venom sac
|     |
|     +-- Ask for help fighting Radscorpions
|            |
|            +-- See Aradesh / Tandi / Seth
|
+-- [RADSCORPION SAMPLE PROVIDED]
|     |
|     +-- Examine sample
|     +-- Make cure
|     +-- Heal / antidote
|     +-- +250 XP
|
+-- [LATE NIGHT]
|     |
|     +-- Come back in morning
|
+-- [SPECIAL]
      |
      +-- Low intelligence responses
      +-- Theft response
      +-- Generic information
```

---

# 32. HEALING STATE MACHINE

```text
PLAYER: "I need healing."
        |
        v
   [CHECK PLAYER]
        |
        +-- NOT HURT
        |     |
        |     +-- 125
        |     |   "Well, you seem fine to me. Perhaps later, then?"
        |     |
        |     +-- 175
        |         "You're barely hurt..."
        |
        +-- LIGHTLY HURT
        |     |
        |     +-- 175
        |     +-- 176
        |
        +-- MODERATELY HURT
        |     |
        |     +-- 177
        |         "$25"
        |           |
        |           +-- enough money → pay / heal
        |           +-- not enough → 179
        |
        +-- SERIOUSLY HURT
        |     |
        |     +-- 180
        |         "$50"
        |           |
        |           +-- enough money → pay / heal
        |           +-- not enough → 182
        |
        +-- EXTREMELY HURT
              |
              +-- 183
                  "$100"
                    |
                    +-- enough money → pay / heal
                    +-- not enough → 185
```

---

# 33. RADSCORPION ANTIDOTE STATE MACHINE

```text
PLAYER
|
+-- "What do you know about Radscorpions?"
|       |
|       +-- 153
|       +-- 154
|
+-- "I need something for their poison."
|       |
|       +-- 192
|           "If I had a sample of their poison, I could create a cure."
|
+-- "I have a sample of the Radscorpion poison..."
|       |
|       +-- 149
|       +-- 150
|       +-- 151
|
+-- "I have the venom sac"
        |
        +-- 161
        +-- 162
        |
        +-- [ANTIDOTE]
        +-- 300
            "+250 XP"
```

---

# 34. SCRIPT CHECKS TO VERIFY AGAINST RAZLO SCRIPT

```text
ENTRY
    - first visit vs repeat visit
    - suspicious / hostile entry
    - late-night entry
    - exact condition for 120
    - exact condition for 131
    - exact condition for 136

TANDI
    - Tandi missing
    - Tandi rescued
    - Tandi dead
    - exact branch that gives items at 116
    - exact branch that produces 118
    - exact branch that produces 119

HEALING
    - player HP / injury level
    - exact thresholds for $25 / $50 / $100
    - exact healing amount
    - money requirement
    - trade possibility
    - previous healing / healing limit
    - whether Razlo can heal after Tandi death
    - late-night restriction

RADSCORPIONS
    - Radscorpion quest active
    - poison sample possession
    - venom sac possession
    - antidote creation
    - whether poison sample is consumed
    - whether venom sac is consumed
    - healing effect
    - +250 XP result

DIALOGUE
    - exact conditions selecting 145 / 146 / 148
    - exact conditions selecting 158 / 161 / 162
    - exact conditions selecting 190 / 191 / 192
    - exact node routing for duplicated Tandi responses 194 / 195
    - low-INT requirements for 106 / 123 / 134 / 139 / 147 / 167
    - late-night check leading to 163

ITEMS / EFFECTS
    - item given at 116
    - item given at 162
    - item involved in 301-307
    - money removed by healing
    - healing result
    - XP reward 300
```

---

# 35. COMPLETE TEXTUAL NODE INDEX

```text
100  Razlo description
101  Can't help
102  Can't or won't?
103  Turns away
104  Offers assistance
105  Tandi kidnapped
106  Low-INT Tandi response
107  Tandi sympathy
108  Seth elopement theory
109  Didn't think of that
110  Tandi kidnapped response
111  Talk to other people
112  Who is looking for Tandi?
113  Almost everyone
114  Many people, including myself
115  Hope she is found
116  Gives items
117  Hope Tandi returns
118  Tandi rescued
119  Tandi dead / grief
120  Razlo introduction
121  Nice meeting you
122  Need healing
123  Low-INT healing
124  Insult / examine wounds
125  Not wounded
126  Examine wounds
127  Healing limit
128  Suspicious entry
129  Just looking
130  Need healing
131  Return greeting
132  Just stopping by
133  Need healing
134  Low-INT return
135  Goodbye
136  Of course, you again
137  Nothing
138  Need healing
139  Low-INT
140  Of course you do
141  Can I help?
142  No
143  Need healing
144  Poisoned
145  Ask about Radscorpions
146  Have poison sample
147  Low-INT
148  Cannot cure poison
149  Examine sample
150  Make cure
151  Seems fine
152  Come back healthy
153  Radscorpion scientific information
154  Evolution / radiation
155  Thanks
156  Ask about Razlo's education
157  Anything else?
158  Venom sac information
159  Thanks
160  Doc Morbid / education
161  Venom sac provided
162  Antidote / poison cure
163  Late / come back morning
164  Need help
165  Killing Radscorpions
166  Looking for Tandi
167  Low-INT
168  Come back morning
169  Are you hurt?
170  Yes
171  No
172  Come back morning
173  Great Brahmin / serious reaction
174  I did
175  Barely hurt
176  Barely hurt / insult
177  $25
178  Pay $25
179  Not enough $25
180  $50
181  Pay $50
182  Not enough $50
183  $100
184  Pay $100
185  Not enough $100
186  Not enough money / no free healing
187  Trade?
188  Thanks for nothing
189  Sorry, not today
190  Doctor, not warrior
191  Need poison cure
192  Need poison sample
194  Tandi kidnapped
195  Talk to others
196  Healing process
197  Healing limit

300  +250 XP for antidote
301  K, me giv?
302  Nunga
303  Scorpin?
304  My giv dis?
305  Radscorpion tail instead?
306  Examine sac / heal
307  Work with sac / heal
308  Do not take my things

1000  Khans
1001  Garl
1002  Junktown
1003  Shady
1004  Hub
1005  Water
1006  Tandi
1007  Aradesh
1008  Seth
1009  Razlo
1010  Sands

1100  Khans answer
1101  Garl answer
1102  Junktown answer
1103  Shady answer
1104  Hub answer
1105  Water answer
1106  Tandi answer
1107  Aradesh answer
1108  Seth answer
1109  Razlo answer
1110  Sands answer
```

---

# 36. ORIGINAL MESSAGE BANK — COMPLETE

```text
{100}{}{You see Razlo.}
{101}{}{I'm sorry but I can't help you.}
{102}{}{Can't or won't?}
{103}{}{Razlo looks at you for a moment, and then turns away.}
{104}{}{You look distraught. Are you in need of assistance?}
{105}{}{Tandi has been kidnapped.}
{106}{}{Hng, Tanny boo.}
{107}{}{Tandi is gone? That is a terrible thing. I hope they find her.}
{108}{}{Hmm, Tandi is gone? I find this hard to believe, but maybe
 she ran away with Seth. I know that they have been eyeing
 each other lately. Aradesh was always a little bit of a
 prude, perhaps it was too much for her finally.}
{109}{}{Hmm, I didn't think of that.}
{110}{}{I don't think so, I think that she was kidnapped.}
{111}{}{Perhaps, I'll go talk to some other people.}
{112}{}{How awful! If that is the case, I hope she is not hurt.
 Who is looking for her?}
{113}{}{Almost everyone.}
{114}{}{Many people, including myself.}
{115}{}{I hope she is found before it is too late.}
{116}{}{If she is hurt, it could be bad. Here, take these . . .
 I hope that you won't need them.}
{117}{}{I hope Tandi gets back soon. Aradesh must be feeling
 terrible right now.}
{118}{}{I heard that you helped save Tandi. Congratulations
 and thank you. Anytime you need healing, come here.}
{119}{}{My grief knows no end. Tandi, the only child of my
 good friend, is gone. I cannot work right now.}
{120}{}{I'm Razlo. Can I help you?}
{121}{}{Not right now, but it was nice meeting you.}
{122}{}{I need healing.}
{123}{}{Hnngh!}
{124}{}{Hmmm. I think I've seen Brahmin with more brain cells
 then you. Well, let's see if you're wounded.}
{125}{}{Well, you seem fine to me. Perhaps later, then?}
{126}{}{Let's see here.}
{127}{}{I have already given you as much as I can. Good luck.}
{128}{}{Hmmm, I don't like your looks. I hope that you have come
 for business, and will be moving on shortly . . .}
{129}{}{No need to get defensive, I'm just looking around}
{130}{}{Help, I need some healing.}
{131}{}{Hello, again. How can I be of service?}
{132}{}{Hello again. Just stopping by.}
{133}{}{I need healing.}
{134}{}{Ug!}
{135}{}{It was nice seeing you again. Have a good day.}
{136}{}{Of course. It's you again. Now what?}
{137}{}{Nothing.}
{138}{}{I need healing.}
{139}{}{Luki!}
{140}{}{Of course you do.}
{141}{}{Can I help you?}
{142}{}{No.}
{143}{}{I need healing.}
{144}{}{I've been poisoned!}
{145}{}{What do you know about Radscorpions?}
{146}{}{I have a sample of the Radscorpion poison, can you do
 something with it?}
{147}{}{Luki!}
{148}{}{Sorry, I can't cure any poisons. I don't have the
 proper ingredients to make a decent anti-venom.}
{149}{}{Here, let's see.}
{150}{}{Yep, just as I thought. Let me get the cure.}
{151}{}{You seem fine, now. Be careful in the future.}
{152}{}{Stop by again, but stay healthy next time.}
{153}{}{Not too much. They seem to be extremely large versions
 of the north American Emperor scorpion. Contrary to my
 medical knowledge, their poison has grown more potent,
 not diluted, as I would expect. Seth has been hunting them, which helps
 some of my tests.}
{154}{}{And how such a large
 creature can even be possible by natural evolution,
 or even radiation induced mutation is beyond me.}
{155}{}{Thanks.}
{156}{}{You seem well educated. What are you doing here?}
{157}{}{Anything else?}
{158}{}{They seem to be sensitive to light. Nocturnal. If I
 could get a sample of their poison, especially the
 venom production sac located in their tail, I could
 create an anti-venom.}
{159}{}{Thanks.}
{160}{}{Doc Morbid in Junktown taught me some. And just
 because we live in stone huts, does not mean we are
 less intelligent or educated. Have a nice day.}
{161}{}{Let me see the sac! Yes, I can do something with
 this.}
{162}{}{Here, take this as a free sample. It seems to work
 well against radscorpion stings, of course, but it
 shows much potential as a general anti-venom and
 poison cure.}
{163}{}{It's late. Come back in the morning.}
{164}{}{But I need your help.}
{165}{}{I'm trying to kill some radscorpions. Do you have anything
 to help?}
{166}{}{I'm looking for Tandi. Can you help?}
{167}{}{Hnn.}
{168}{}{Hnn?! What is that? Great, I get awakened by the great
 mumbling beast of the north. Come back in the morning.}
{169}{}{Are you hurt?}
{170}{}{Yes.}
{171}{}{No.}
{172}{}{You will be, if you keep me from my sleep. Now come
 back in the morning.}
{173}{}{Great Brahmin! Why didn't you say so?}
{174}{}{Uhhh. I did . . .}
{175}{}{You're barely hurt. I suggest a good nights sleep and
 some rest; you will heal naturally over time.}
{176}{}{You're barely hurt, you wimp. Come back in the morning.}
{177}{}{Ouch, that does look a little painful. That will be $25.}
{178}{}{OK, here you go.}
{179}{}{I don't have enough money.}
{180}{}{Oh Great Brahmin! That is very, very serious. It will
 cost $50.}
{181}{}{OK, here you go.}
{182}{}{I don't have enough money.}
{183}{}{How can you still be walking?! This looks extremely
 serious. We'll need some special prayers for this one
 and a lot of luck. Not to mention $100.}
{184}{}{OK, here you go.}
{185}{}{I don't have enough money.}
{186}{}{You don't have enough money! I'm sorry, but my work ethic
 doesn't permit me to heal for free, you know. Come back
 when you have enough money.}
{187}{}{Can you take something in trade?}
{188}{}{Thanks for nothing.}
{189}{}{Sorry, not today.}
{190}{}{Why would I be able to help you kill radscorpions?
 I'm a doctor, not a warrior. For that you should see
 Aradesh, or even Tandi, or her boyfriend, Seth. Leave
 me.}
{191}{}{I need something for their poison.}
{192}{}{If I had a sample of their poison, I could create
 a cure.}
{194}{}{I don't think so, I think that she was kidnapped.}
{195}{}{Perhaps, I'll go talk to some other people.}
{196}{}{This will take a little bit of time. Come, relax in
 the back. You might have a slight headache from the
 junja juice, as well. Come, come . . .}
{197}{}{I have already given you as much as I can. Good luck.}
{300}{}{You gain 250 experience points for helping make the antidote.}
{301}{}{K, me giv?}
{302}{}{Nunga.}
{303}{}{Scorpin, Scorpin?}
{304}{}{My giv dis?}
{305}{}{How about a Radscorpion tail instead?}
{306}{}{Let me see the sac! Yes, I can do something with
 this, as well as heal you for this great gift.}
{307}{}{This will take a little bit of time. Come, relax in
 the back, while I work with the sac. You might have a slight headache from the
 junja juice, as well. Come, come . . .}
{308}{}{Please do not take my things.}


{1000}{}{Khans}
{1001}{}{Garl}
{1002}{}{Junktown}
{1003}{}{Shady}
{1004}{}{Hub}
{1005}{}{Water}
{1006}{}{Tandi}
{1007}{}{Aradesh}
{1008}{}{Seth}
{1009}{}{Razlo}
{1010}{}{Sands}

{1100}{}{They are a lawless group who pretend to be some people from the
 far past.}
{1101}{}{He leads the Khans.  He is one of the most dangerous men that
 has ever set eyes on Shady Sands.  He leads the raider clans with a
 heavy fist.}
{1102}{}{It's 4-6 days to the southwest.  Good trading occasionally, but
 more often than not, they try to stiff us.}
{1103}{}{You're standing in it.}
{1104}{}{Much farther to the south than Junktown.  The Hub is the largest
 city I've heard about.  Must be a thousand people there, at least.}
{1105}{}{Sorry, I can't help you.}
{1106}{}{She is the daughter of Aradesh.  Some day she could lead us, but
 she must learn to temper her ways.}
{1107}{}{He is the leader of our community, and a true hero.  You can always
 find him at work in the communal hall.}
{1108}{}{He works with Aradesh to make sure we are safe.}
{1109}{}{That is me. I am the doctor for Shady Sands.}
{1110}{}{You're standing in it.}
```
