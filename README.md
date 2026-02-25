# cub3D — Mandatory Part

A minimal first-person raycaster in C, inspired by Wolfenstein 3D, built for the 42 school curriculum.

**Code Quality:** All functions strictly adhere to norminette limits (≤25 lines, ≤5 parameters, ≤5 variables per function).

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

## Building

The project requires the [minilibx-linux](https://github.com/42Paris/minilibx-linux)
library.  Download it and place it in a `minilibx-linux/` directory 
# get minilibx-linux
git clone https://github.com/42Paris/minilibx-linux.git

# build
make

# run
./cub3D maps/valid/test.cub
```

---

## .cub scene file format

```
NO ./path/to/north.xpm
SO ./path/to/south.xpm
EA ./path/to/east.xpm
WE ./path/to/west.xpm

F R,G,B         # floor colour  (0-255 each)
C R,G,B         # ceiling colour

111111
100N01
100001
111111
```

Map tiles: `1` = wall, `0` = empty space, `N/S/E/W` = player start.
The map must be the last element in the file and fully enclosed by walls.

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

Place four 64×64 (or any power-of-two) XPM textures and reference them in the scene file:

```
textures/
  north.xpm
  south.xpm
  east.xpm
  west.xpm
```
