*This project has been created as part of the 42 curriculum by rd-agost.*

# cub3D

A minimalistic first-person 3D raycaster in C, inspired by Wolfenstein 3D, built for the 42 school curriculum.

## Description

cub3D is a first-person 3D maze explorer written in C, inspired by the iconic Wolfenstein 3D (1992). The goal is to implement a real-time raycasting engine from scratch using the MiniLibX graphics library.

The program reads a `.cub` scene file that defines wall textures, floor/ceiling colors, and a 2D map layout, then renders the maze in a 3D perspective that the player can navigate in real time. The core rendering technique is the **DDA (Digital Differential Analysis)** raycasting algorithm: for each vertical column of the screen, a ray is cast from the player's position into the map grid. The algorithm steps through the grid cell by cell until it hits a wall, calculates the perpendicular distance to avoid fisheye distortion, and draws a textured vertical slice whose height is inversely proportional to that distance.

Key technical aspects of the project include:
- A complete `.cub` file parser with validation of textures, colors, map characters, and map enclosure (via recursive flood fill)
- Textured walls with a different XPM texture for each cardinal direction (N, S, E, W)
- Smooth WASD + arrow key controls with per-axis sliding collision detection
- Strict compliance with the 42 norminette coding standard (≤25 lines per function, ≤5 variables, ≤5 parameters)

---

## Features

- ⚡ **Real-time raycasting** — DDA algorithm for efficient wall detection
- 🎨 **Textured walls** — Different textures for each cardinal direction
- 🎮 **Smooth controls** — WASD movement with arrow key rotation
- 🗺️ **Scene parser** — Custom `.cub` file format with validation
- 🔍 **Map validation** — Ensures closed boundaries and valid player position
- 🎯 **Norminette compliant** — Clean, modular, readable code

---

## Architecture

| File | Responsibility |
|------|---------------|
| `src/main.c` | Entry point, argument validation, mlx hooks setup |
| `src/parse_utils.c` | Custom GNL (`ft_gnl`), string helpers, linked-list utilities |
| `src/parse_scene.c` | `.cub` file parser (textures, colors, map detection). Uses helper functions for initialization, line processing, and file reading |
| `src/parse_map.c` | Map grid builder and validation orchestration with character checking helpers |
| `src/parse_map_helpers.c` | Grid padding, character validation, player position finder (modular DRY approach) |
| `src/init.c` | mlx window/image setup, XPM texture loading. Player initialization split into direction setter and position finder |
| `src/render.c` | DDA raycaster with modular sub-functions: ray initialization, DDA steps, wall distance calculation, texture mapping, column drawing |
| `src/events.c` | Movement, rotation logic (vector rotation helper), game loop |
| `src/events_two.c` | Key-press/release handlers, window close handler |
| `src/utils.c` | String utilities (`ft_strlen`, `ft_strcmp`, `ft_strncmp`, `ft_memset`, `ft_memcpy`) |
| `src/utils_two.c` | Map utilities (`ft_flood`, `ft_free_grid`, `ft_is_start`, `ft_is_tile`) |
| `src/utils_three.c` | Additional string utilities (`ft_strchr`) |
| `src/cleanup.c` | Resource teardown and exit |
| `src/error.c` | Uniform error printer (`ft_err`) |
| `include/cub3d.h` | All structs (including raycast helper structs: `t_ray`, `t_dda`, `t_wall`), constants, function prototypes |

**Note:** All functions use the `ft_` prefix convention for consistency.

### Code Structure Principles

- **Modular Design:** Complex functions are decomposed into focused helper functions
- **DRY Principle:** Repeated logic extracted into reusable components
- **Clean Abstractions:** Local structs (`t_ray`, `t_dda`, `t_wall`) organize related data in render pipeline
- **Norminette Compliance:** Strict adherence to line/variable/parameter limits ensures readable, maintainable code

---

## Instructions

### Prerequisites

- A C compiler (gcc or cc)
- GNU Make
- X11 development libraries (`libx11-dev`, `libxext-dev`) on Linux
- [MiniLibX for Linux](https://github.com/42Paris/minilibx-linux)

### Compilation

```bash
# Clone the repository and enter it
git clone <repository-url> cub3d
cd cub3d

# Clone MiniLibX if not already present
git clone https://github.com/42Paris/minilibx-linux.git

# Build the project (MiniLibX is compiled automatically)
make
```

### Execution

```bash
./cub3d maps/valid/test.cub
```

The argument must be a path to a valid `.cub` scene file. The program will open a window and render the maze.

### Controls

| Key | Action |
|-----|--------|
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| `←` / `→` | Rotate camera |
| `ESC` | Quit |
| Window ✕ | Quit |

### Make Targets

| Command | Description |
|---------|-------------|
| `make` | Build the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and binary |
| `make re` | Full recompile |

---

## Scene File Format (.cub)

Scene files define the textures, colors, and map layout for the game.

### Format Specification

```
NO ./textures/north.xpm
SO ./textures/south.xpm
EA ./textures/east.xpm
WE ./textures/west.xpm

F 134,136,151
C 24,30,59

111111111111111
100000000000001
10000N000000001
100000000000001
111101111110001
100000000000001
100000000000001
111111111111111
```

### Elements

| Element | Description |
|---------|-------------|
| `NO` | Path to north wall texture (XPM format) |
| `SO` | Path to south wall texture (XPM format) |
| `EA` | Path to east wall texture (XPM format) |
| `WE` | Path to west wall texture (XPM format) |
| `F` | Floor color in RGB format (0–255 each) |
| `C` | Ceiling color in RGB format (0–255 each) |

### Map Rules

- **Tiles:**
  - `1` = Wall
  - `0` = Empty walkable space
  - `N/S/E/W` = Player starting position and direction
- **Requirements:**
  - Map must be enclosed by walls (`1`s)
  - Must contain exactly one player start position
  - Map must be the last element in the file
  - Invalid maps will be rejected with error messages

Valid maps are provided in `maps/valid/`, and invalid test cases in `maps/invalid/`.

---

## Technical Details

### Raycasting Algorithm

This project uses the **DDA (Digital Differential Analysis)** algorithm for raycasting:

1. For each screen column, cast a ray from the player's position
2. Step through the grid until hitting a wall
3. Calculate perpendicular wall distance (to avoid fisheye effect)
4. Determine wall height based on distance
5. Map texture coordinates and render the vertical slice

**Field of View:** 66° (plane magnitude of 0.66)

For a detailed explanation of the raycasting implementation, see [RAYCASTING_EXPLANATION.txt](RAYCASTING_EXPLANATION.txt).

### Memory Management

- All dynamically allocated memory is properly freed
- Textures and MLX resources are cleaned up on exit
- Comprehensive error handling with cleanup on failure

---

## Project Structure

```
cub3d/
├── include/
│   └── cub3d.h                # Structures, constants, function prototypes
├── src/
│   ├── main.c                 # Entry point and initialization
│   ├── parse_scene.c          # Scene file parser
│   ├── parse_scene_utils.c    # Scene parsing helper functions
│   ├── parse_map.c            # Map grid builder and validation
│   ├── parse_map_helpers.c    # Grid utilities and character validation
│   ├── parse_texture.c        # Texture path parsing
│   ├── parse_color.c          # RGB color parsing
│   ├── parse_utils.c          # Custom GNL and string utilities
│   ├── parse_utils_two.c      # Additional parsing helpers
│   ├── parse_utils_three.c    # More parsing utilities
│   ├── init.c                 # Player and game initialization
│   ├── init_mlx.c             # MLX window and texture setup
│   ├── render.c               # Main rendering loop
│   ├── render_ray.c           # Ray casting calculations
│   ├── render_wall.c          # Wall rendering and height calculation
│   ├── render_texture.c       # Texture mapping
│   ├── render_pixel.c         # Pixel manipulation
│   ├── events.c               # Movement and rotation logic
│   ├── events_two.c           # Key press/release and window handlers
│   ├── utils.c                # String utilities (strlen, strcmp, etc.)
│   ├── utils_two.c            # Map utilities (flood fill, grid operations)
│   ├── utils_three.c          # Additional utilities (strchr)
│   ├── cleanup.c              # Resource cleanup and exit
│   └── error.c                # Error handling and messages
├── maps/
│   ├── valid/                 # Valid test maps
│   │   └── test.cub
│   └── invalid/               # Invalid maps for testing
│       ├── missing_tex.cub
│       └── open_map.cub
├── textures/                  # Wall texture files (XPM)
│   ├── north.xpm
│   ├── south.xpm
│   ├── east.xpm
│   └── west.xpm
├── minilibx-linux/            # Graphics library
├── obj/                       # Object files (generated)
├── Makefile
├── README.md
└── RAYCASTING_EXPLANATION.txt # Detailed technical documentation
```

---

## Resources

### References

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) — the primary reference for the DDA raycasting algorithm and texture mapping used in this project
- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx) — API reference for the graphics library
- [Wolfenstein 3D (Wikipedia)](https://en.wikipedia.org/wiki/Wolfenstein_3D) — historical context on the game that pioneered this technique

### Use of AI

AI (ChatGPT / Claude) was used as a support tool during the development of this project for the following tasks:

- **Debugging assistance**: help identifying edge cases in the map parser and understanding specific compiler warnings
- **Concept explanation**: clarifying the mathematical foundations of the DDA algorithm, perpendicular distance calculation, and camera plane geometry
- **README drafting**: help structuring and writing this documentation

All code was written, understood, and tested manually. AI was not used to generate the codebase itself.

---

## License

This project is part of the 42 school curriculum.