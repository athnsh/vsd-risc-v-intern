# Register Map

## Overview

The Timer IP is accessed through a 32-bit memory-mapped register interface. All registers are word-aligned and support 32-bit accesses.

**Base Address:** Assigned by the SoC integration.

| Offset | Register | Access | Description |
|--------|----------|--------|-------------|
| `0x00` | CTRL | R/W | Timer control register |
| `0x04` | LOAD | R/W | Countdown load value |
| `0x08` | VALUE | R | Current countdown value |
| `0x0C` | STATUS | R/W | Timeout status register |

---

# CTRL Register (0x00)

Controls timer operation.

| Bits | Name | Access | Reset | Description |
|------|------|--------|-------|-------------|
| 0 | EN | R/W | 0 | Enables the timer |
| 1 | MODE | R/W | 0 | `0` = One-shot, `1` = Periodic |
| 2 | PRESC_EN | R/W | 0 | Enables the prescaler |
| 7:3 | Reserved | - | 0 | Reserved |
| 15:8 | PRESC_DIV | R/W | 0 | Prescaler divide value |
| 31:16 | Reserved | - | 0 | Reserved |

### Register Behavior

- Setting **EN** from `0 → 1` loads the VALUE register from LOAD.
- MODE selects one-shot or periodic operation.
- PRESC_EN enables the programmable prescaler.
- PRESC_DIV determines the prescaler period.

---

# LOAD Register (0x04)

Stores the initial countdown value.

| Bits | Name | Access | Reset | Description |
|------|------|--------|-------|-------------|
| 31:0 | LOAD | R/W | 0 | Countdown start value |

### Register Behavior

- Software writes the desired countdown value.
- VALUE is loaded from LOAD whenever:
  - EN transitions from `0` to `1`
  - Periodic mode reloads after timeout

---

# VALUE Register (0x08)

Current countdown value.

| Bits | Name | Access | Reset | Description |
|------|------|--------|-------|-------------|
| 31:0 | VALUE | Read Only | 0 | Current timer count |

### Register Behavior

- Automatically loaded from LOAD when the timer starts.
- Decrements every clock cycle.
- If the prescaler is enabled, decrements once every `(PRESC_DIV + 1)` clock cycles.
- Read-only from software.

---

# STATUS Register (0x0C)

Contains the timeout status.

| Bits | Name | Access | Reset | Description |
|------|------|--------|-------|-------------|
| 0 | TIMEOUT | R/W (W1C) | 0 | Timeout flag |
| 31:1 | Reserved | - | 0 | Reserved |

### Register Behavior

- Set automatically when the countdown reaches zero.
- Cleared by writing a `1` to bit 0.
- Reading returns the current timeout status.

---

# Register Access Sequence

## One-Shot Mode

1. Write LOAD.
2. Configure CTRL.
3. Set EN.
4. Poll STATUS.
5. Clear TIMEOUT.
6. Restart by setting EN again if required.

---

## Periodic Mode

1. Write LOAD.
2. Configure CTRL with MODE = 1.
3. Set EN.
4. Poll STATUS.
5. Clear TIMEOUT after each timeout event.
6. VALUE automatically reloads from LOAD.

---

# Reset Values

| Register | Reset Value |
|----------|-------------|
| CTRL | `0x00000000` |
| LOAD | `0x00000000` |
| VALUE | `0x00000000` |
| STATUS | `0x00000000` |

---

# Register Summary

| Register | Read | Write | Description |
|----------|------|-------|-------------|
| CTRL | ✓ | ✓ | Timer configuration |
| LOAD | ✓ | ✓ | Initial countdown value |
| VALUE | ✓ | ✗ | Current countdown value |
| STATUS | ✓ | ✓ (Write-1-to-Clear) | Timeout flag |