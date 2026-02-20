/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:26:08 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/20 21:26:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>

# include "../minilibx-linux/mlx.h"

/* ---- Window / render config -------------------------------------------- */
# define WIN_W       1280
# define WIN_H       720
# define WIN_TITLE   "cub3D"

/* ---- Movement / rotation speeds --------------------------------------- */
# define MOVE_SPD    0.05
# define ROT_SPD     0.03

/* ---- Map tile codes --------------------------------------------------- */
# define TILE_EMPTY  '0'
# define TILE_WALL   '1'

/* ---- Cardinal direction indices --------------------------------------- */
# define DIR_NO  0
# define DIR_SO  1
# define DIR_EA  2
# define DIR_WE  3

/* ---- Key codes -------------------------------------------------------- */
# define KEY_W        119
# define KEY_A        97
# define KEY_S        115
# define KEY_D        100
# define KEY_LEFT     65361
# define KEY_RIGHT    65363
# define KEY_ESC      65307

/* ---- GNL buffer size -------------------------------------------------- */
# define GNL_BUFSIZE 4096

/* ======================================================================== */
/*  Simple dynamic string list (used for file lines / map rows)             */
/* ======================================================================== */
typedef struct s_strlist
{
	char				*str;
	struct s_strlist	*next;
}	t_strlist;

/* ======================================================================== */
/*  XPM texture loaded into mlx image                                        */
/* ======================================================================== */
typedef struct s_tex
{
	void	*img;
	char	*data;
	int		w;
	int		h;
	int		bpp;
	int		stride;
	int		endian;
}	t_tex;

/* ======================================================================== */
/*  Scene description parsed from .cub file                                  */
/* ======================================================================== */
typedef struct s_scene
{
	char	*tex_path[4];   /* NO SO EA WE */
	int		floor_rgb[3];
	int		ceil_rgb[3];
	char	**grid;         /* raw map grid (null-terminated array of rows)  */
	int		grid_rows;
	int		grid_cols;      /* width of the widest row (padded with spaces)  */
}	t_scene;

/* ======================================================================== */
/*  Player state                                                              */
/* ======================================================================== */
typedef struct s_player
{
	double	px;
	double	py;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

/* ======================================================================== */
/*  Input bitmask                                                             */
/* ======================================================================== */
typedef struct s_input
{
	bool	w;
	bool	s;
	bool	a;
	bool	d;
	bool	left;
	bool	right;
}	t_input;

/* ======================================================================== */
/*  MLX / image state                                                         */
/* ======================================================================== */
typedef struct s_mlx
{
	void	*conn;
	void	*win;
	void	*img;
	char	*buf;
	int		bpp;
	int		stride;
	int		endian;
}	t_mlx;

/* ======================================================================== */
/*  Master game state                                                         */
/* ======================================================================== */
typedef struct s_game
{
	t_scene		scene;
	t_player	player;
	t_input		input;
	t_mlx		mlx;
	t_tex		textures[4];
}	t_game;

/* ---- src/parse_utils.c ------------------------------------------------ */
int			count_chars(const char *s, char c);
int			str_only_whitespace(const char *s);
char		*trim_newline(char *s);
char		*gnl(int fd);
void		free_strlist(t_strlist *lst);
t_strlist	*strlist_append(t_strlist *lst, char *str);
char		**strlist_to_array(t_strlist *lst, int count);
int			list_count(t_strlist *lst);

/* ---- src/parse_scene.c ------------------------------------------------ */
int			parse_scene(t_game *g, const char *path);

/* ---- src/parse_map.c -------------------------------------------------- */
int			parse_map(t_game *g, t_strlist *map_lines);
int			validate_map(t_game *g);

/* ---- src/init.c ------------------------------------------------------- */
int			init_mlx(t_game *g);
int			load_textures(t_game *g);
void		setup_player(t_game *g);

/* ---- src/render.c ----------------------------------------------------- */
void		render_frame(t_game *g);

/* ---- src/events.c ----------------------------------------------------- */
int			on_keydown(int key, t_game *g);
int			on_keyup(int key, t_game *g);
int			on_close(t_game *g);
int			game_loop(t_game *g);

/* ---- src/cleanup.c ---------------------------------------------------- */
void		cleanup_and_exit(t_game *g, int code);
void		free_scene(t_scene *sc);

/* ---- src/error.c ------------------------------------------------------ */
int			err(const char *msg);

#endif
