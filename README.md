# Minesweeper C++

A classic Minesweeper implementation built from scratch in **C++20** using the **Raylib** graphics library. Designed with a focus on clean composition-based architecture, modern C++ idioms, and zero-overhead abstractions.

---

## Features

- **First-Click Protection:** Hitting a mine on your very first move dynamically relocates the mine to a safe cell and recalculates neighboring counts—guaranteeing you never lose on turn one.
- **Recursive Flood-Fill:** Clicking an empty (`0`) cell automatically cascades outward to reveal all connected safe regions up to their numbered boundaries.
- **Chording Support:** Middle-click or press **Left + Right Click** simultaneously on a revealed number whose surrounding flags match its value to quickly open all remaining unflagged neighbors.
- **Precision Stopwatch & Mine Counter:** Tracks elapsed round time to tenths of a second (`000.0`) using a stateful `std::chrono::steady_clock` timer that freezes upon win or loss, alongside a live remaining-flag counter.
- **Instant Restart:** Press `R` at any time to reset the board, reshuffle mines with a Mersenne Twister PRNG (`std::mt19937`), and start a fresh round.
- **Debug-Only Telemetry:** Uses conditional compilation (`#ifdef _DEBUG`) for console logging and heap allocation tracking, stripping diagnostic overhead completely in Release builds.

---

## Controls

| Input | Action |
| :--- | :--- |
| **Left Click** | Reveal a hidden cell (or chord if clicked on an opened number) |
| **Right Click** | Place or remove a flag (`!`) |
| **Middle Click** or **Left + Right Click** | Chord — reveal surrounding unflagged neighbors |
| **`R` Key** | Restart the game |

---

## Project Structure

```text
Minesweeper_Cpp/
├── Test.cpp        # Entry point (main) and 60 FPS Raylib game loop
├── GameLogic.h     # Core gameplay and rendering classes (BOARD, CELL, GRID, GAME)
├── Engines.h       # PRNG_Engine (std::mt19937) and stateful Timer_Engine (std::chrono)
└── Debug.h         # Debug-mode heap allocation tracker (AllocationMetrics)
```

### Architecture Highlights
- **Composition Over Inheritance:** `GAME` owns `BOARD`, `GRID`, `Timer_Engine`, and `PRNG_Engine` directly as value members rather than relying on deep inheritance hierarchies.
- **Contiguous Grid Storage:** `GRID` stores all `CELL` instances in a single pre-reserved `std::vector<CELL>` (`row * colNum + col` row-major indexing) for cache-friendly traversal.
- **Templated Neighbor Traversal:** A compile-time `NeighbourFunc` template helper encapsulates 8-way Moore neighborhood bounds checking and inlines custom lambdas with zero runtime overhead.

---

## Prerequisites & Building

### Requirements
- A **C++20** compatible compiler (MSVC / Visual Studio 2022+, GCC 11+, or Clang 14+)
- [**Raylib**](https://www.raylib.com/) (v4.5 or newer)

### Building with Visual Studio (Windows)
1. Create an empty C++ Console/Desktop project in Visual Studio and add `Test.cpp`, `GameLogic.h`, `Engines.h`, and `Debug.h`.
2. Set the C++ Language Standard to **ISO C++20 (`/std:c++20`)** under **Project Properties $\rightarrow$ C/C++ $\rightarrow$ Language**.
3. Link **Raylib** (`raylib.lib`, `winmm.lib`, `gdi32.lib`, `opengl32.lib`) via **vcpkg** or your local Raylib `include` and `lib` directories.
4. Select **Debug** or **Release** (`x64`) and press **F5** to build and run.

### Building with g++ (MinGW / Linux)
```bash
g++ -std=c++20 -O2 Test.cpp -o Minesweeper -lraylib -lopengl32 -lgdi32 -lwinmm
./Minesweeper
```
*(On Linux, replace `-lopengl32 -lgdi32 -lwinmm` with `-lGL -lm -lpthread -ldl -lrt -lX11`.)*
