<div align="center">

# C Drone Fleet Tracker

### Authors: Landon Wardle and Robert Cromer
### October, 2025

A C-based drone fleet management system featuring continuous entry mode and a nearest drone finder.

<!-- Badge row - these render as little pills on GitHub -->
![Language](https://img.shields.io/badge/language-C-00599C?style=for-the-badge&logo=c&logoColor=white)

![Focus](https://img.shields.io/badge/focus-embedded%20systems-2b6cb0?style=for-the-badge)

</div>

---

## Table of Contents

1. [Compilation](#compilation)
2. [Running on Windows](#running-on-windows)
3. [Features and Variations](#features-and-variations)
4. [Data Model](#data-model)
5. [Functions and Files](#functions-and-files)
6. [Commands](#commands)
7. [Example Usage](#example-usage)

---

# Compilation

To compile and run the project, open a terminal in the directory containing `drone_tracker.c`
and make sure the GNU Compiler Collection (`g++`) is installed.

```bash
gcc -std=c11 drone_tracker.c -lm -o drone_tracker
./drone_tracker
```

> [!NOTE]
> The `-lm` flag is **required**. The nearest-drone finder uses `sqrt()` from `<math.h>`,
> and the program **will fail to link** without it.

# Running on Windows

A precompiled **`drone_tracker.exe`** is included, so Windows users can run the program
without installing a compiler. Simply run the application from the project directory.

```powershell
.\drone_tracker.exe
```

> [!NOTE]
> If you would rather rebuild the executable yourself on Windows:
> ```powershell
> gcc -std=c11 drone_tracker.c -lm -o drone_tracker.exe
> ```

# Features and Variations

This project implements two assigned variations on top of the required material:

- **Variation 1C - Continuous Entry Mode:** add drones back-to-back in a single loop until you choose to stop (`add_fleet`).
- **Variation 2B - Nearest Drone Finder:** given an `(X, Y)` point, report the closest drone by distance (`find_nearest_drone`).

Required material includes adding and displaying drones, searching by ID, and computing the fleet's average battery level.

# Data Model

The fleet is stored as a set of **parallel arrays**, where a single index identifies one drone across every array:

| Array | Holds | Notes |
|:------|:------|:------|
| `ids` | Unique drone IDs | Nonzero positive integers; duplicates rejected on entry |
| `models` | Model names | Up to 6 characters |
| `batteries` | Battery levels | Floats from `0.0` inclusive to `100.0` inclusive |
| `positions` | `(X, Y)` coordinates | Two floats per drone |

The fleet holds up to **`MAX_FLEET_SIZE`** drones, which is configured to be 100 by default.

# Functions and Files

### Files:
 - **drone_tracker.c**: The main program. Contains input handling, the drone data model, and the menu loop.
 - **drone_tracker.exe**: Precompiled Windows binary (see [Running on Windows](#running-on-windows)).

### Input & Validation:
 - **clear_input()**: Flushes leftover characters from `stdin` so a bad entry won't trigger an infinite loop.
 - **get_id()**: Prompts for a nonzero positive integer ID, optionally rejecting duplicates via `is_id_dupe()`.
 - **get_char_array() / get_name()**: Read a bounded model name and re-prompt if it exceeds the length limit.
 - **get_battery_level()**: Reads a validated float in the range `0.0` inclusive to `100.0` inclusive.
 - **get_coordinate() / get_position()**: Read labeled `X` / `Y` coordinates into a drone's position.

### Fleet Operations:
 - **add_drone()**: Collects one drone's data and writes it into the parallel arrays.
 - **add_fleet()**: *(Variation 1C)* Repeatedly calls `add_drone()` for continuous netry until the user declines to continue.
 - **search_drone_by_id()**: Linear search for a drone by ID, printing its record if found. Could be improved with a binary search!
 - **calculate_average_battery()**: Averages battery levels across the whole fleet.
 - **calculate_distance() / find_nearest_drone()**: *(Variation 2B)* Compute distance and report the closest drone to a given point.

### Display:
 - **display_table_header(), display_drone(), display_drones()**: Print a single drone or the full fleet in a table.

### Entry Point:
 - **main()**: Presents the command menu and loops until the user decides to exit.

# Commands

| Cmd | Action |
|:---:|:-------|
| `1` | Add a drone |
| `2` | Display all drones |
| `3` | Search for a drone by ID |
| `4` | Show average battery level |
| `5` | Add a fleet (continuous entry - *Variation 1C*) |
| `6` | Find nearest drone (*Variation 2B*) |
| `0` | Exit |

# Example Usage

<details>
<summary><b>Example 1: Add a drone and display the fleet</b></summary>

<br>

**Commands Run:**

| Cmd # | Description |
|:------|:------------|
| 1 | Add a drone (ID `42`, model `Hawk`, battery `88.5`, position `3, 4`) |
| 2 | Display all drones |

**Expected Output:**
The program confirms `Drone Hawk [ID=42] added!`, then prints the table header followed by a single row: ID `42`, model `Hawk`, battery `88.50%`, and the `X` and `Y` columns showing `3.00` and `4.00`.

</details>

<details>
<summary><b>Example 2: Duplicate ID rejection</b></summary>

<br>

**Commands Run:**

| Cmd # | Description |
|:------|:------------|
| 1 | Add a drone with ID `42` |
| 1 | Add another drone, also entering ID `42` |

**Expected Output:**
The first drone is added normally. On the second attempt, entering `42` is rejected with a prompt to enter a different ID, since `is_id_dupe()` flags the collision. Entry continues once a unique ID is provided.

</details>

<details>
<summary><b>Example 3: Find the nearest drone</b></summary>

<br>

**Commands Run:**

| Cmd # | Description |
|:------|:------------|
| 5 | Add a fleet of several drones at different positions |
| 6 | Find nearest drone to a given point `(0, 0)` |

**Expected Output:**
The program prompts for an `X` and `Y` coordinate, computes the Euclidean distance from that point to each drone, and prints the closest one with its distance (for example, `Nearest drone (2.83 units away):`) followed by that drone's table row.

</details>