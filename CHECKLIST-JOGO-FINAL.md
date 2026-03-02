# Luzes Apagadas — Updated Game Plan

## Status Overview

After a thorough analysis of the entire codebase, the vast majority of the original plan has already been **implemented and working**. Below is a detailed status of every original task, followed by **the minimal remaining tasks** needed to finish the game.

---

## ✅ Completed Tasks

### Phase 1: Implementation Plan Fixes

| Task | Status | Evidence |
|------|--------|----------|
| **1.1 Wall Flickering Fix** | ✅ Done | [drawFace()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/drawlevel.cpp#L360-L378) uses broad `outside` check: `neighbor != '1' && neighbor != '2' && neighbor != '3'` |
| **1.2 Enemy Safe Zone Avoidance** | ✅ Done | [updateEntities()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/entities.cpp#L55-L66) freezes enemies in safe zones; [chase movement](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/entities.cpp#L109-L116) blocks movement into safe zones per-axis |
| **1.3 Ceiling Texture** | ✅ Done | [desenhaQuadTeto()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/drawlevel.cpp#L135-L150) accepts `texTeto` param, binds texture, has UV coords, color set to white |

### Phase 2: Luzes Apagadas Core Mechanics

| Task | Status | Evidence |
|------|--------|----------|
| **2.1 Battery Collectible + ItemType** | ✅ Done | `ITEM_BATTERY` in [entities.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/core/entities.h), tile `'V'` handled in [level.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/level/level.cpp#L96-L103) |
| **2.2 Battery Count in PlayerState** | ✅ Done | `batteriesCollected` in [game_state.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/core/game_state.h#L20), reset in [gameReset()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#L170), incremented in [entities.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/entities.cpp#L159-L163) |
| **2.3 Gate Exit on Batteries** | ✅ Done | [Door exit check](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#L250-L298) requires both `batteriesCollected >= BATTERIES_REQUIRED` AND `hasLevelKey[currentLevel]` |
| **2.4 Monster Hunts in Darkness Only** | ✅ Done | `playerVisibleToMonster` check in [entities.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/entities.cpp#L69-L72); IDLE/CHASE/ATTACK all respect this flag |
| **2.6 HUD Battery Display** | ✅ Done | [drawStatusBar()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/hud.cpp#L147-L265) shows "BATERIAS X/14" and key icon |
| **2.7 Battery Texture + Draw** | ✅ Done | `texBattery` in [assets.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/utils/assets.cpp#L56-L57), rendered in [drawEntities()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/drawlevel.cpp#L534-L535) |

### Phase 3: Level Design and Light Timing

| Task | Status | Evidence |
|------|--------|----------|
| **3.1 Place Batteries in Maps** | ✅ Done | Level 1: 6V + 1Y, Level 2: 5V + 1Y, Level 3: 4V + 1Y (15 total batteries, 14 required — gives 1 leeway) |
| **3.2 Light Cycle Tuning** | ✅ Done | `LIGHT_CYCLE_ON_SECONDS = 6.0f`, `LIGHT_CYCLE_FLICKER_SECONDS = 2.0f` in [config.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/core/config.h#L24-L25) |

### Phase 4: Audio and Feedback

| Task | Status | Evidence |
|------|--------|----------|
| **4.1 Battery Pickup Sound** | ✅ Done | [audioPlayBatteryPickup()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/audio/audio_system.cpp#395-398) in [audio_system.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/audio/audio_system.h#L81), called from [entities.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/entities.cpp#L162) |
| **4.2 Door Locked Feedback** | ✅ Done | [audioPlayPumpClick()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/audio/audio_system.h#77-79) on door locked with 2s cooldown in [game.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#L266-L270) |

### Phase 5: Gameplay Loop Polish

| Task | Status | Evidence |
|------|--------|----------|
| **5.2 Victory Message** | ✅ Done | "LUZES APAGADAS — VOCE ESCAPOU!" in [gameRender()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#L400) |
| **5.3 Difficulty Tuning Constants** | ✅ Done | `BATTERIES_REQUIRED`, `LIGHT_CYCLE_ON_SECONDS`, `LIGHT_CYCLE_FLICKER_SECONDS`, `SAFE_ZONE_RADIUS` all in [config.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/core/config.h) |

### Extra Features Already Implemented (Beyond Original Plan)

- **Key collectible system** (`ITEM_KEY` / tile `'Y'`) — exit requires both all batteries AND level key
- **Level progression** — 3 levels with auto-load of `maps/level1-3.txt`
- **Key display on HUD** — shows key icon when collected, "?" when missing
- **3D Avatar enemies** (GLB models) 
- **Light cone visualization** — translucent cone + additive ground disc + ring border
- **Flashlight battery drain/recharge** system
- **Melt screen transition** effect

---

## ❌ Remaining Tasks (Minimal — What's Missing)

These are the only items from the original plan that have **NOT** been implemented yet, plus a few new improvements identified during analysis:

### Task R1: On-Screen Hints / Tutorial (Original Task 5.1)

> **Priority:** Medium | **Effort:** Small

Currently no tutorial or hints are shown. The player needs to know the rules.

#### [MODIFY] [hud.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/hud.cpp)

- Add a `drawTutorialHints()` function that shows brief text for the first ~8 seconds:
  - Line 1: `"Fique na luz! O monstro só ataca no escuro."`
  - Line 2: `"Colete baterias e a chave para ativar o elevador."`
- Call it from [hudRenderAll()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/hud.cpp#267-285) with a timer that fades out after 8s
- Use existing `uiDrawStrokeText()` utility

#### [MODIFY] [hud.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/graphics/hud.h)

- Add `float gameTime = 0.0f;` to `HudState` to drive the hint timer

#### [MODIFY] [game.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp)

- Pass `g.time` into `HudState` in [gameRender()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#369-419)

---

### Task R2: Door Locked On-Screen Text (Enhancement to Task 4.2)

> **Priority:** Medium | **Effort:** Small

The door plays a click sound when locked, but there's no visual feedback explaining **why** it's locked.

#### [MODIFY] [game.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp)

- Add a `doorLockedMessageTimer` that is set when door is locked
- Store a message string like `"Preciso de mais baterias"` or `"Preciso da chave"` depending on which requirement is missing

#### [MODIFY] [hud.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/graphics/hud.cpp)

- Add `drawDoorLockedMessage()` that renders centered text when timer > 0

#### [MODIFY] [hud.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/graphics/hud.h)

- Add `float doorLockedTimer = 0.0f;` and `std::string doorLockedMsg;` to `HudState`

---

### Task R3: Darkness Creep Audio Cue (Original Task 4.3)

> **Priority:** Low | **Effort:** Medium

No ambient audio cue when the player is in darkness (not in safe zone, flashlight off).

#### [MODIFY] [audio_system.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/audio/audio_system.h)

- Add `ALuint bufDarkAmbient = 0;` and `ALuint srcDarkAmbient = 0;` to [AudioSystem](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/audio/audio_system.h#20-60)
- Add `float darknessTimer = 0.0f;`

#### [MODIFY] [audio_system.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/audio/audio_system.cpp)

- In [audioInit()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/audio/audio_system.cpp#118-224): load a dark-ambient WAV (`assets/audio/dark_ambient.wav` — needs to be provided)
- In [audioUpdate()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/audio/audio_system.cpp#225-378): if player is in darkness for > 1s, start the loop; if player enters light, stop it

> [!IMPORTANT]
> This task requires a **dark_ambient.wav** audio asset. If you don't have one, we can skip this task or use one of the existing ambient sounds at low volume.

---

### Task R4: Proper Level Transition Reset (Bug Fix)

> **Priority:** High | **Effort:** Small  

When transitioning between levels, `batteriesCollected` is correctly preserved (batteries carry across levels), but on [gameReset()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#161-189) (player restarts the game), batteries and keys are properly reset. However, there's a subtle issue:

#### [MODIFY] [game.cpp](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp)

- In the level transition code (around line 280), re-initialize the audio system correctly for the new level
- Verify that enemy counts are re-created for the new level's enemy sources (currently [audioInit](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/audio/audio_system.cpp#118-224) is called which should handle this)

> [!NOTE]
> After reviewing the code, the level transition looks correct. [audioInit()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/audio/audio_system.cpp#118-224) is called on level change and [gameReset()](file:///d:/UNIFAP/projeto-doom-computacao-grafica/src/core/game.cpp#161-189). This task may already be fine — it just needs **manual testing** to confirm.

---

### Task R5: Build Verification & Compile Clean

> **Priority:** High | **Effort:** Small

Verify the project compiles cleanly with no warnings or errors.

#### Build Command

```bash
cd d:\UNIFAP\projeto-doom-computacao-grafica\build
cmake .. -G "MinGW Makefiles"
cmake --build . 2>&1
```

---

## Implementation Order

| Order | Task | Description | Effort |
|-------|------|-------------|--------|
| 1 | **R5** | Build verification — confirm current state compiles | Small |
| 2 | **R1** | Tutorial/on-screen hints (first 8 seconds) | Small |
| 3 | **R2** | Door locked on-screen message | Small |
| 4 | **R3** | Darkness ambient audio cue (optional — needs asset) | Medium |
| 5 | **R4** | Manual testing: level transitions, battery carry-over, victory | Small |

---

## Verification Plan

### Build Test
1. Run `cmake --build .` from the `build/` directory
2. Confirm zero errors and zero new warnings

### Manual Verification (Play-Testing)
Since this is an OpenGL/GLUT game, automated tests aren't practical. Manual testing steps:

1. **Start the game** → confirm tutorial hints appear for ~8 seconds, then fade
2. **Collect some batteries** → confirm HUD counter updates
3. **Approach the door without enough batteries** → confirm locked sound AND on-screen text
4. **Approach the door without key** → confirm message says "Preciso da chave"
5. **Collect all batteries + key in Level 1** → confirm door opens, level 2 loads
6. **Play through all 3 levels** → confirm victory screen shows "LUZES APAGADAS — VOCE ESCAPOU!"
7. **Die (let monster kill you)** → confirm game over screen appears
8. **Restart after game over** → confirm batteries and keys are reset to 0

> [!TIP]
> You can test quickly by reducing `BATTERIES_REQUIRED` in [config.h](file:///d:/UNIFAP/projeto-doom-computacao-grafica/include/core/config.h) to 1 temporarily.
