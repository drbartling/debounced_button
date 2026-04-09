# Debounce Timing Diagrams

Each tick represents one call to `BUTTON_Debounce()`.
`IO_PRESSED = 0` (LOW), `IO_RELEASED = 1` (HIGH).

```typst
#import "@preview/cetz:0.4.1": canvas, draw

// ─── Layout constants ────────────────────────────────────────────────────────
#let tick-w   = 0.72  // horizontal width per tick (cm)
#let row-h    = 0.9   // vertical spacing between row baselines
#let sig-high = 0.58  // y of HIGH level within a row
#let sig-low  = 0.14  // y of LOW  level within a row
#let bus-top  = 0.64  // y of top  bus rail within a row
#let bus-bot  = 0.10  // y of bottom bus rail within a row
#let bus-xw   = 0.17  // half-width of the X-cross transition marker
#let lbl-w    = 2.5   // width reserved for left-side signal labels
#let n-ticks  = 15    // number of tick intervals shown

// ─── draw-digital ────────────────────────────────────────────────────────────
// Draws a clean square-wave trace.
// `transitions` is an array of (tick, level) pairs meaning "from this tick
// onwards the signal is at this level". Pairs must be sorted by tick.
#let draw-digital(y-base, transitions, color: black) = {
  let n = transitions.len()
  let pts = ()
  for i in range(n) {
    let tick = transitions.at(i).at(0)
    let lvl  = transitions.at(i).at(1)
    let x    = lbl-w + tick * tick-w
    let yy   = y-base + (if lvl == 1 { sig-high } else { sig-low })
    if i == 0 {
      pts.push((x, yy))
    } else {
      let prev-lvl = transitions.at(i - 1).at(1)
      let prev-yy  = y-base + (if prev-lvl == 1 { sig-high } else { sig-low })
      pts.push((x, prev-yy))
      pts.push((x, yy))
    }
  }
  let last-lvl = transitions.at(n - 1).at(1)
  let last-yy  = y-base + (if last-lvl == 1 { sig-high } else { sig-low })
  pts.push((lbl-w + n-ticks * tick-w, last-yy))
  draw.line(..pts, stroke: color + 1.5pt)
}

// ─── draw-bus ────────────────────────────────────────────────────────────────
// Draws an oscilloscope-style "bus decode" band.
// `segments` is an array of (t0, t1, label) triples.
#let draw-bus(y-base, segments, color: blue.darken(25%)) = {
  for seg in segments {
    let t0  = seg.at(0)
    let t1  = seg.at(1)
    let lbl = seg.at(2)
    if t0 == t1 { continue }
    let x0 = lbl-w + t0 * tick-w
    let x1 = lbl-w + t1 * tick-w
    let yt = y-base + bus-top
    let yb = y-base + bus-bot
    let ym = (yt + yb) / 2
    let xw = bus-xw

    draw.line((x0 + xw, yt), (x1 - xw, yt), stroke: color + 1.3pt)
    draw.line((x0 + xw, yb), (x1 - xw, yb), stroke: color + 1.3pt)
    draw.line((x0,      ym), (x0 + xw, yt), stroke: color + 1.3pt)
    draw.line((x0,      ym), (x0 + xw, yb), stroke: color + 1.3pt)
    draw.line((x1 - xw, yt), (x1,      ym), stroke: color + 1.3pt)
    draw.line((x1 - xw, yb), (x1,      ym), stroke: color + 1.3pt)

    draw.content(
      ((x0 + x1) / 2, ym),
      text(size: 6.5pt, fill: color, lbl),
      anchor: "center",
    )
  }
}

// ─── draw-ticks ──────────────────────────────────────────────────────────────
#let draw-ticks(y-top, bot-y) = {
  for t in range(n-ticks + 1) {
    let x = lbl-w + t * tick-w
    draw.content(
      (x, y-top),
      text(size: 7pt, fill: luma(120), str(t)),
      anchor: "south",
    )
    draw.line(
      (x, y-top - 0.18),
      (x, bot-y),
      stroke: (paint: luma(210), thickness: 0.4pt, dash: "dashed"),
    )
  }
}

// ─── draw-label ──────────────────────────────────────────────────────────────
#let draw-label(y-base, lbl) = {
  draw.content(
    (lbl-w - 0.18, y-base + (sig-high + sig-low) / 2),
    align(right, text(size: 7.5pt, lbl)),
    anchor: "east",
  )
}

// ─── draw-row-sep ────────────────────────────────────────────────────────────
#let draw-row-sep(y) = {
  draw.line(
    (0.1, y),
    (lbl-w + n-ticks * tick-w, y),
    stroke: (paint: luma(195), thickness: 0.4pt),
  )
}

// ─── draw-section-box ────────────────────────────────────────────────────────
#let draw-section-box(y-top, y-bot) = {
  draw.rect(
    (0.0, y-bot),
    (lbl-w + n-ticks * tick-w, y-top),
    stroke: (paint: luma(160), thickness: 0.7pt),
    fill: none,
  )
}

// ─── draw-highlight ──────────────────────────────────────────────────────────
#let draw-highlight(t0, t1, y-top, y-bot, color: yellow.lighten(70%)) = {
  draw.rect(
    (lbl-w + t0 * tick-w, y-bot),
    (lbl-w + t1 * tick-w, y-top),
    fill: color,
    stroke: none,
  )
}

// ─── section-rows ────────────────────────────────────────────────────────────
// Returns (row0-y, row1-y, row2-y, row3-y, section-bot-y) top-down.
// Rows: 0=Raw IO, 1=Debounce Count, 2=Button State, 3=Edge
#let section-rows(y-top-inner) = {
  let r0 = y-top-inner - row-h * 0.95
  let r1 = r0 - row-h
  let r2 = r1 - row-h
  let r3 = r2 - row-h
  (r0, r1, r2, r3, r3 - 0.28)
}

// ─── scenario-bot ────────────────────────────────────────────────────────────
// Pure function: returns the bottom y coordinate for a scenario starting at
// y-title, without doing any drawing. Use this outside the canvas to pre-compute
// y positions before passing them into draw-scenario.
#let scenario-bot(y-title) = {
  let tick-y = y-title - 0.45
  let inner  = tick-y  - 0.22
  let rows   = section-rows(inner)
  rows.at(4)
}

// ─── draw-scenario ───────────────────────────────────────────────────────────
// Draws a complete scenario block (box, title, ticks, labels, waveforms).
// `highlights` is an array of (t0, t1, color) triples.
// Does NOT return a value — call scenario-bot() outside the canvas to get y-bot.
#let draw-scenario(y-title, title, raw-io, count, state, edge, highlights) = {
  let tick-y = y-title - 0.45
  let inner  = tick-y  - 0.22
  let rows   = section-rows(inner)
  let y-bot  = rows.at(4)

  for h in highlights {
    draw-highlight(h.at(0), h.at(1), inner, y-bot, color: h.at(2))
  }

  draw-section-box(y-title + 0.05, y-bot)

  draw.content(
    ((lbl-w + n-ticks * tick-w) / 2, y-title),
    text(size: 8.5pt, weight: "bold", title),
    anchor: "south",
  )

  draw-ticks(tick-y, y-bot)

  draw-row-sep(inner)
  for i in range(3) {
    draw-row-sep(rows.at(i) - 0.28)
  }

  draw-label(rows.at(0), "Raw IO")
  draw-label(rows.at(1), "Debounce\nCount")
  draw-label(rows.at(2), "Button\nState")
  draw-label(rows.at(3), "Edge")

  draw-digital(rows.at(0), raw-io)
  draw-bus(rows.at(1), count)
  draw-bus(rows.at(2), state)
  draw-bus(rows.at(3), edge)
}

// ══════════════════════════════════════════════════════════════════════════════
//  SIGNAL DATA  (debounce_limit = 4)
// ══════════════════════════════════════════════════════════════════════════════

// ── Scenario 1 — Clean press then release ────────────────────────────────────
//
//  tick  Raw IO  count  Button State   Edge
//   0    HIGH      0    INITIALIZED    NONE
//   1    LOW       1    INITIALIZED    NONE
//   2    LOW       2    INITIALIZED    NONE
//   3    LOW       3    INITIALIZED    NONE
//   4    LOW     →0     LOW            INITIAL  ← latch (EDGE_INITIAL)
//   5    LOW       0    LOW            NONE
//   6    HIGH      1    LOW            NONE
//   7    HIGH      2    LOW            NONE
//   8    HIGH      3    LOW            NONE
//   9    HIGH    →0     HIGH           RISING   ← latch (EDGE_RISING)
//  10    HIGH      0    HIGH           NONE
//  ...

#let s1-raw   = ((0,1),(1,0),(6,1))
#let s1-count = (
  (0,1,"0"),(1,2,"1"),(2,3,"2"),(3,4,"3"),
  (4,5,"0"),(5,6,"0"),(6,7,"1"),(7,8,"2"),(8,9,"3"),
  (9,15,"0"),
)
#let s1-state = ((0,4,"INITIALIZED"),(4,9,"LOW"),(9,15,"HIGH"))
#let s1-edge  = (
  (0,4,"NONE"),(4,5,"INITIAL"),(5,9,"NONE"),(9,10,"RISING"),(10,15,"NONE"),
)
#let s1-hi = (
  (4, 5, yellow.lighten(60%)),
  (9, 10, yellow.lighten(60%)),
)

// ── Scenario 2 — Bouncy release ──────────────────────────────────────────────
//
//  tick  Raw IO  count  Button State   Edge
//   0    HIGH      0    INITIALIZED    NONE
//   1    LOW       1    INITIALIZED    NONE
//   2    LOW       2    INITIALIZED    NONE
//   3    LOW       3    INITIALIZED    NONE
//   4    LOW     →0     LOW            INITIAL  ← latch
//   5    HIGH      1    LOW            NONE
//   6    HIGH      2    LOW            NONE
//   7    HIGH      3    LOW            NONE
//   8    LOW     →0     LOW            NONE     ← bounce! counter resets
//   9    HIGH      1    LOW            NONE
//  10    HIGH      2    LOW            NONE
//  11    HIGH      3    LOW            NONE
//  12    HIGH    →0     HIGH           RISING   ← latch
//  13    HIGH      0    HIGH           NONE
//  ...

#let s2-raw   = ((0,1),(1,0),(5,1),(8,0),(9,1))
#let s2-count = (
  (0,1,"0"),(1,2,"1"),(2,3,"2"),(3,4,"3"),
  (4,5,"0"),(5,6,"1"),(6,7,"2"),(7,8,"3"),
  (8,9,"0"),(9,10,"1"),(10,11,"2"),(11,12,"3"),
  (12,15,"0"),
)
#let s2-state = ((0,4,"INITIALIZED"),(4,12,"LOW"),(12,15,"HIGH"))
#let s2-edge  = (
  (0,4,"NONE"),(4,5,"INITIAL"),(5,12,"NONE"),(12,13,"RISING"),(13,15,"NONE"),
)
#let s2-hi = (
  (4,  5,  yellow.lighten(60%)),
  (8,  9,  orange.lighten(70%)),
  (12, 13, yellow.lighten(60%)),
)

// ══════════════════════════════════════════════════════════════════════════════
//  RENDER
// ══════════════════════════════════════════════════════════════════════════════

#let s1-title-y = 0.0
#let s2-title-y = scenario-bot(s1-title-y) - 0.55

#align(center)[
  #canvas(length: 1cm, {

    draw-scenario(
      s1-title-y,
      "Scenario 1 — Clean Press then Release  (debounce_limit = 4)",
      s1-raw, s1-count, s1-state, s1-edge, s1-hi,
    )

    draw-scenario(
      s2-title-y,
      "Scenario 2 — Bouncy Release  (debounce_limit = 4,  bounce resets counter at tick 8)",
      s2-raw, s2-count, s2-state, s2-edge, s2-hi,
    )
  })
]

#v(0.4em)
#align(center)[
  #text(size: 7.5pt, fill: luma(80))[
    #box(width: 10pt, height: 8pt, fill: yellow.lighten(50%), baseline: 30%) #h(3pt)
    Latch event — debounce_count reaches debounce_limit, state and edge update
    #h(14pt)
    #box(width: 10pt, height: 8pt, fill: orange.lighten(60%), baseline: 30%) #h(3pt)
    Bounce event — mismatched reading matches current state, counter resets to 0
  ]
]
```

---

## How it works

Each call to `BUTTON_Debounce(reading, button)`:

1. If `reading` **matches** the current `state` → reset `debounce_count` to 0.
2. If `reading` **differs** from the current `state` → increment `debounce_count`.
3. Once `debounce_count` reaches `debounce_limit`:
   - `state` latches to the new value.
   - `edge` is set to `EDGE_INITIAL` (if coming out of `BUTTON_INITIALIZED`),
     `EDGE_FALLING` (HIGH→LOW), or `EDGE_RISING` (LOW→HIGH).
   - `debounce_count` resets to 0.
4. On any call that does **not** latch → `edge` is set to `EDGE_NONE`.

The key insight shown in Scenario 2: a single noise spike that matches the *current*
state resets the counter to zero, so the full `debounce_limit` of clean consecutive
readings is always required before a transition is accepted.

---

## Edge Value Reference

| Edge value     | Meaning                                                                       |
|----------------|-------------------------------------------------------------------------------|
| `EDGE_INITIAL` | First latch out of `BUTTON_INITIALIZED` (button state was never known before) |
| `EDGE_RISING`  | State transitioned LOW → HIGH (button released on an active-low circuit)      |
| `EDGE_FALLING` | State transitioned HIGH → LOW (button pressed on an active-low circuit)       |
| `EDGE_NONE`    | No transition this tick; set on every call that does not produce a latch      |

> **Note:** An edge value is valid for exactly one call to `BUTTON_Debounce()`. On the
> very next call, if the reading still matches the new state, `debounce_count` stays 0
> and `edge` is immediately cleared back to `EDGE_NONE`.
