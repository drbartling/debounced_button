# Debounce Timing Diagrams

All diagrams use `debounce_limit = 8`. Each tick represents one call to `BUTTON_Debounce()`.
`IO_PRESSED = 0` (LOW), `IO_RELEASED = 1` (HIGH).

---

## Initial Press from Uninitialized State

After initialization the button is in `BUTTON_INITIALIZED` state. The first time
`debounce_limit` consecutive identical readings are seen, the state latches and
the edge is reported as `EDGE_INITIAL`.

```mermaid
---
displayMode: compact
---
timingDiagram
    section Raw IO
        IO_Reading : high : 0
                   : low  : 1
                   : low  : 2
                   : low  : 3
                   : low  : 4
                   : low  : 5
                   : low  : 6
                   : low  : 7
                   : low  : 8
                   : low  : 9

    section Button State
        DEB_State : INITIALIZED : 0
                  : INITIALIZED : 7
                  : LOW         : 8
                  : LOW         : 9

    section Edge
        DEB_Edge : NONE         : 0
                 : NONE         : 7
                 : EDGE_INITIAL : 8
                 : EDGE_NONE    : 9
```

**What's happening:**

- Ticks 0–7: readings differ from `BUTTON_INITIALIZED`, so `debounce_count` increments each call.
- Tick 8: `debounce_count` reaches `debounce_limit` → state latches to `LOW`, edge set to `EDGE_INITIAL` (special value used when transitioning out of the initialized state), count resets.
- Tick 9: reading matches state → count stays 0, edge clears to `EDGE_NONE`.

---

## Press Then Release (Clean Signal)

A clean press followed by a clean release with no noise.

```mermaid
---
displayMode: compact
---
timingDiagram
    section Raw IO
        IO_Reading : low  : 0
                   : low  : 7
                   : low  : 8
                   : high : 9
                   : high : 16
                   : high : 17
                   : high : 18

    section Button State
        DEB_State : INITIALIZED : 0
                  : INITIALIZED : 7
                  : LOW         : 8
                  : LOW         : 16
                  : HIGH        : 17
                  : HIGH        : 18

    section Edge
        DEB_Edge : NONE         : 0
                 : NONE         : 7
                 : EDGE_INITIAL : 8
                 : EDGE_NONE    : 9
                 : EDGE_NONE    : 16
                 : EDGE_RISING  : 17
                 : EDGE_NONE    : 18
```

**What's happening:**

- Ticks 0–7: 8× LOW readings accumulate the debounce count.
- Tick 8: state latches `LOW`, edge = `EDGE_INITIAL` (first transition out of initialized state).
- Tick 9: reading now matches state → edge clears to `EDGE_NONE`, count resets.
- Ticks 9–16: 8× HIGH readings accumulate the debounce count.
- Tick 17: state latches `HIGH`, edge = `EDGE_RISING`.
- Tick 18: reading matches state → edge clears to `EDGE_NONE`.

---

## Bouncy / Noisy Signal (Rejected Transitions)

A single stray sample in the opposite direction resets the debounce counter without
changing the output state.

```mermaid
---
displayMode: compact
---
timingDiagram
    section Raw IO
        IO_Reading : low  : 0
                   : low  : 7
                   : low  : 8
                   : high : 9
                   : high : 10
                   : high : 11
                   : high : 12
                   : low  : 13
                   : high : 14
                   : high : 15
                   : high : 16
                   : high : 17
                   : high : 18
                   : high : 19
                   : high : 20
                   : high : 21
                   : high : 22

    section Button State
        DEB_State : INITIALIZED : 0
                  : INITIALIZED : 7
                  : LOW         : 8
                  : LOW         : 12
                  : LOW         : 13
                  : LOW         : 21
                  : HIGH        : 22

    section Edge
        DEB_Edge : NONE         : 0
                 : NONE         : 7
                 : EDGE_INITIAL : 8
                 : EDGE_NONE    : 9
                 : EDGE_NONE    : 12
                 : EDGE_NONE    : 13
                 : EDGE_NONE    : 21
                 : EDGE_RISING  : 22
```

**What's happening:**

- Ticks 0–8: button latches `LOW` as before.
- Ticks 9–12: 4× HIGH readings, `debounce_count` climbs to 4.
- Tick 13: a single LOW (bounce) — reading matches current state (`LOW`), so `debounce_count` resets to 0. State unchanged, edge stays `EDGE_NONE`.
- Ticks 14–21: 8 more consecutive HIGH readings are needed to re-accumulate the count.
- Tick 22: state finally latches `HIGH`, edge = `EDGE_RISING`.

---

## Edge Lifecycle Summary

```mermaid
---
displayMode: compact
---
timingDiagram
    section Edge Value
        DEB_Edge : EDGE_NONE    : 0
                 : EDGE_INITIAL : 1
                 : EDGE_NONE    : 2
                 : EDGE_NONE    : 3
                 : EDGE_RISING  : 4
                 : EDGE_NONE    : 5
                 : EDGE_NONE    : 6
                 : EDGE_FALLING : 7
                 : EDGE_NONE    : 8
```

| Edge value    | Meaning                                                                 |
|---------------|-------------------------------------------------------------------------|
| `EDGE_INITIAL`| State transitioned out of `BUTTON_INITIALIZED` for the first time       |
| `EDGE_RISING` | State transitioned from `LOW` → `HIGH` (button released, active-low)   |
| `EDGE_FALLING`| State transitioned from `HIGH` → `LOW` (button pressed, active-low)    |
| `EDGE_NONE`   | No transition this tick; always set on the call immediately after a latch |
```

Now let me save the file:
