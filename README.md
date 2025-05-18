# Treasure Hub

Welcome to **Treasure Hub**! This interactive, shell-based system helps you manage treasure hunts,
add and view treasures, monitor activity, and calculate per-user scores for each hunt.

## Key Components

- **treasure_manager**: Command-line utility to create, list, and manage hunts and treasures.
- **treasure_hub**: Interactive console that runs a background monitor to execute commands without exiting.
- **score_calculator**: Helper program that computes total scores for each user in a given hunt.

## Prerequisites

- POSIX-compatible operating system (Linux, macOS)
- C compiler with C99 support (e.g. `gcc`)

## Building from source

Run these commands from the project root to compile all executables:

```bash
# Build the treasure_manager tool
gcc -o treasure_manager \
    src/treasure_manager/*.c \
    src/helpers.c

# Build the interactive treasure_hub interface
gcc -o treasure_hub \
    src/treasure_hub/*.c \
    src/helpers.c

# Build the score_calculator helper
gcc -o score_calculator \
    src/score_calculator/score_calculator.c \
    src/helpers.c
```

Make sure the resulting executables (`treasure_manager`, `treasure_hub`, and `score_calculator`)
are in your `PATH` or in the same directory you run them from.

## Usage

### treasure_manager (command-line)

```bash
# List all hunts
./treasure_manager list_hunts

# Create a new hunt and add treasures
./treasure_manager add <hunt_id>

# List treasures in a hunt
./treasure_manager list <hunt_id>

# View details for a specific treasure
./treasure_manager view <hunt_id> <treasure_id>

# Remove a treasure or entire hunt
./treasure_manager remove_treasure <hunt_id> <treasure_id>
./treasure_manager remove_hunt <hunt_id>
```

### treasure_hub (interactive console)

```bash
$ ./treasure_hub
Welcome to treasure_hub. Type commands (help to list commands, exit to quit).
> start_monitor               # Start the background monitor
> list_hunts                  # Show all hunts (monitor must be running)
> list_treasures <hunt_id>    # Show treasures for a specific hunt
> calculate_score             # Compute and display per-user scores for every hunt
> stop_monitor                # Stop the background monitor
> exit                        # Exit the application (monitor must be stopped)
```

Run `help` inside `treasure_hub` at any time to see all available commands.

### score_calculator (helper)

```bash
# Compute scores for a single hunt (used internally by treasure_hub)
./score_calculator <hunt_id>
```

Outputs each username and their total score for the specified hunt.
