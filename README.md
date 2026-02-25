# cub3D — Mandatory Part

A minimal first-person raycaster in C, inspired by Wolfenstein 3D, built for the 42 school curriculum.

---

## Architecture

| File | Responsibility |
|------|---------------|
| `src/main.c` | Entry point, argument validation, mlx hooks setup |
| `src/parse_utils.c` | Custom GNL (`ft_gnl`), string helpers, linked-list utilities |
| `src/parse_scene.c` | `.cub` file parser (textures, colors, dispatches to map parser) |
| `src/parse_map.c` | Map grid builder, validation orchestration |
| `src/parse_map_helpers.c` | Grid padding, character validation, player position finder |
| `src/init.c` | mlx window/image setup, XPM texture loading, player initialization |
| `src/render.c` | DDA raycaster, textured wall columns, ceiling/floor fill |
| `src/events.c` | Movement, rotation logic, game loop |
| `src/events_two.c` | Key-press/release handlers, window close handler |
| `src/utils.c` | String utilities (`ft_strlen`, `ft_strcmp`, `ft_strncmp`, `ft_memset`, `ft_memcpy`) |
| `src/utils_two.c` | Map utilities (`ft_flood`, `ft_free_grid`, `ft_is_start`, `ft_is_tile`) |
| `src/cleanup.c` | Resource teardown and exit |
| `src/error.c` | Uniform error printer (`ft_err`) |
| `include/cub3d.h` | All structs, constants, function prototypes |

**Note:** All functions use the `ft_` prefix convention for consistency.

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
