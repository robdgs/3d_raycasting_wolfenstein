/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:26:08 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:54:47 by rd-agost         ###   ########.fr       */
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
	char	*tex_path[4];
	int		floor_rgb[3];
	int		ceil_rgb[3];
	char	**grid;
	int		grid_rows;
	int		grid_cols;
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
int			ft_count_chars(const char *s, char c);
int			ft_str_only_whitespace(const char *s);
char		*ft_trim_newline(char *s);
char		*ft_gnl(int fd);
void		ft_free_strlist(t_strlist *lst);
t_strlist	*ft_strlist_append(t_strlist *lst, char *str);
char		**ft_strlist_to_array(t_strlist *lst, int count);
int			ft_list_count(t_strlist *lst);

/* ---- src/parse_scene.c ------------------------------------------------ */
int			ft_parse_scene(t_game *g, const char *path);

/* ---- src/parse_map -------------------------------------------------- */
int			ft_parse_map(t_game *g, t_strlist *map_lines);
int			ft_validate_map(t_game *g);
void		ft_stfind(char **wrk, t_scene *sc, int *strt, int *sr, int *sc_col);
int			ft_check_chars(t_scene *sc, char **work);
void		ft_copy_padded_row(char *dst, char *src, int cols);
char		**ft_alloc_grid_rows(int rows, int cols);
char		**ft_pad_grid(char **src, int rows, int cols);

/* ---- src/init.c ------------------------------------------------------- */
int			ft_init_mlx(t_game *g);
int			ft_load_textures(t_game *g);
void		ft_setup_player(t_game *g);

/* ---- src/render.c ----------------------------------------------------- */
void		ft_render_frame(t_game *g);

/* ---- src/events.c ----------------------------------------------------- */
int			ft_on_keydown(int key, t_game *g);
int			ft_on_keyup(int key, t_game *g);
int			ft_on_close(t_game *g);
int			ft_game_loop(t_game *g);

/* ---- src/cleanup.c ---------------------------------------------------- */
void		ft_cleanup_and_exit(t_game *g, int code);
void		ft_free_scene(t_scene *sc);

/* ---- src/error.c ------------------------------------------------------ */
int			ft_err(const char *msg);

/* ---- src/utils ------------------------------------------------------ */
size_t		ft_strlen(const char *str);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
int			ft_flood(char **grid, int rows, int cols, int r, int c);
void		ft_free_grid(char **grid);
int			ft_is_start(char ch);
int			ft_is_tile(char ch);

#endif
