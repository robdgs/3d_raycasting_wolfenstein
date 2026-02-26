# cub3D

A minimalistic first-person 3D raycaster in C, inspired by Wolfenstein 3D, built for the 42 school curriculum.

This project implements a real-time raycasting engine using the MiniLibX graphics library. Navigate through textured mazes with smooth movement and rotation controls.

**Code Quality:** All functions strictly adhere to norminette standards (≤25 lines, ≤5 parameters, ≤5 variables per function).

---

## Features

- ⚡ **Real-time raycasting** - DDA algorithm for efficient wall detection
- 🎨 **Textured walls** - Different textures for each cardinal direction
- 🎮 **Smooth controls** - WASD movement with arrow key rotation
- 🗺️ **Scene parser** - Custom `.cub` file format with validation
- 🔍 **Map validation** - Ensures closed boundaries and valid player position
- 🎯 **Norminette compliant** - Clean, modular, readable code

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

## Building & Running

### Prerequisites

- GCC compiler
- Make
- X11 development libraries (on Linux)
- [MiniLibX for Linux](https://github.com/42Paris/minilibx-linux)

### Installation

```bash
# Clone the repository
git clone <repository-url> cub3d
cd cub3d

# Get MiniLibX (if not already present)
git clone https://github.com/42Paris/minilibx-linux.git

# Build the project
make

# Run with a map file
./cub3d maps/valid/test.cub
```

### Make Commands

| Command | Description |
|---------|-------------|
| `make` | Compile the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executable |
| `make re` | Recompile the project from scratch |

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
| `F` | Floor color in RGB format (0-255 each) |
| `C` | Ceiling color in RGB format (0-255 each) |

### Map Rules

- **Tiles:**
  - `1` = Wall
  - `0` = Empty walkable space
  - `N/S/E/W` = Player starting position and direction
- **Requirements:**
  - Map must be enclosed by walls (1's)
  - Must contain exactly one player start position
  - Map must be the last element in the file
  - Invalid maps will be rejected with error messages

### Example Maps

Valid maps are provided in `maps/valid/`, and invalid test cases in `maps/invalid/`.

---

## Controls

| Key | Action |
|-----|--------|
| W / S | Move forward / backward |
| A / D | Strafe left / right |
| ← / → | Rotate camera |
| ESC | Quit |
| Window ✕ | Quit |

---

## Texture layout

Place four 64×64 (or any power-of-two) XPM textures and reference them in the scene file.

The textures directory structure:

```
textures/
  north.xpm   # North-facing wall texture
  south.xpm   # South-facing wall texture
  east.xpm    # East-facing wall texture
  west.xpm    # West-facing wall texture
```

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

### Code Organization

The codebase is organized following strict norminette guidelines:

- **Helper Structures:** Local structs (`t_ray`, `t_dda`, `t_wall`) in the render pipeline organize related data
- **Function Decomposition:** Complex logic split into focused, reusable functions
- **DRY Principle:** No code duplication; shared logic extracted into utilities
- **Clear Separation:** Parsing, rendering, events, and utilities in separate modules

### Memory Management

- All dynamically allocated memory is properly freed
- Textures and MLX resources are cleaned up on exit
- Comprehensive error handling with cleanup on failure

---

## Project Structure

```
cub3d/
├── include/
│   └── cub3d.h              # Structures, constants, function prototypes
├── src/
│   ├── main.c               # Entry point and initialization
│   ├── parse_scene.c        # Scene file parser
│   ├── parse_map.c          # Map grid builder and validation
│   ├── parse_map_helpers.c  # Grid utilities and character validation
│   ├── parse_texture.c      # Texture path parsing
│   ├── parse_color.c        # RGB color parsing
│   ├── parse_utils*.c       # Parsing utilities (GNL, string helpers)
│   ├── init.c               # MLX initialization
│   ├── init_mlx.c           # Window and texture setup
│   ├── render*.c            # Raycasting and rendering pipeline
│   ├── events*.c            # Keyboard and window events
│   ├── utils*.c             # General utilities
│   ├── cleanup.c            # Resource cleanup
│   └── error.c              # Error handling
├── maps/
│   ├── valid/               # Valid test maps
│   └── invalid/             # Invalid maps for testing
├── textures/                # Wall texture files (XPM)
├── minilibx-linux/          # Graphics library
├── Makefile
└── README.md
```

---

## Resources & References

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) - Comprehensive raycasting guide
- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx) - MLX library reference
- [Wolfenstein 3D](https://en.wikipedia.org/wiki/Wolfenstein_3D) - The game that inspired this project

---

## License

This project is part of the 42 school curriculum.
