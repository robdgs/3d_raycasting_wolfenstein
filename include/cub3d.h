/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:26:08 by rd-agost          #+#    #+#             */
/*   Updated: 2026/04/08 10:29:18 by rd-agost         ###   ########.fr       */
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
/*  Start position info (output from ft_stfind)                              */
/* ======================================================================== */
typedef struct s_start_info
{
	int	count;
	int	row;
	int	col;
}	t_start_info;

/* ======================================================================== */
/*  Grid wrapper for flood fill validation                                   */
/* ======================================================================== */
typedef struct s_grid
{
	char	**data;
	int		rows;
	int		cols;
}	t_grid;

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

/* ======================================================================== */
/*  Internal render structures                                               */
/* ======================================================================== */

typedef struct s_ray
{
	double	dx;
	double	dy;
}	t_ray;

typedef struct s_dda
{
	int		map_x;
	int		map_y;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		step_x;
	int		step_y;
	int		side;
}	t_dda;

typedef struct s_wall
{
	double	perp_dist;
	int		line_h;
	int		draw_top;
	int		draw_bot;
	int		tex_x;
}	t_wall;

typedef struct s_ray_calc
{
	t_dda		*dda;
	t_ray		*ray;
	t_player	*pl;
}	t_ray_calc;

/* ---- src/parse_utils.c ------------------------------------------------ */
char		*ft_trim_newline(char *s);
char		*ft_gnl(int fd);

/* ---- src/parse_utils_two.c -------------------------------------------- */
void		ft_free_strlist(t_strlist *lst);
t_strlist	*ft_strlist_append(t_strlist *lst, char *str);
char		**ft_strlist_to_array(t_strlist *lst, int count);
int			ft_list_count(t_strlist *lst);

/* ---- src/parse_utils_three.c ------------------------------------------ */
int			ft_count_chars(const char *s, char c);
int			ft_str_only_whitespace(const char *s);

/* ---- src/parse_texture.c ---------------------------------------------- */
int			ft_is_xpm(const char *path);
int			ft_parse_texture_line(t_scene *sc, int dir, const char *rest);

/* ---- src/parse_color.c ------------------------------------------------ */
int			ft_parse_one_component(const char **p, int *value);
int			ft_parse_color_line(int *rgb, const char *rest);

/* ---- src/parse_scene_utils.c ------------------------------------------ */
int			ft_line_is_map_row(const char *s);
int			ft_headers_complete(const t_scene *sc);
int			ft_dispatch_header(t_scene *sc, const char *line);
void		ft_init_scene_data(t_scene *sc);

/* ---- src/parse_scene.c ------------------------------------------------ */
int			ft_parse_scene(t_game *g, const char *path);

/* ---- src/parse_map -------------------------------------------------- */
int			ft_parse_map(t_game *g, t_strlist *map_lines);
int			ft_validate_map(t_game *g);
void		ft_stfind(char **wrk, t_scene *sc, t_start_info *info);
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
int			ft_is_wall(t_scene *sc, double x, double y);

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
int			ft_flood(t_grid *grid, int r, int c);
void		ft_free_grid(char **grid);
int			ft_is_start(char ch);
int			ft_is_tile(char ch);
char		*ft_strchr(const char *s, int c);

/* ---- src/render_pixel.c ----------------------------------------------- */
void		ft_put_pixel(t_mlx *m, int x, int y, int color);
int			ft_rgb(int r, int g, int b);
void		ft_draw_background(t_game *g);

/* ---- src/render_texture.c --------------------------------------------- */
t_tex		*ft_pick_texture(t_game *g, double ray_dx, double ray_dy, int side);
int			ft_sample_tex(t_tex *tex, int tx, int ty);

/* ---- src/render_ray.c ------------------------------------------------- */
void		ft_init_ray(t_ray *ray, t_player *pl, int x);
void		ft_init_dda_deltas(t_dda *dda, t_ray *ray, t_player *pl);
void		ft_init_dda_steps(t_dda *dda, t_ray *ray, t_player *pl);
void		ft_perform_dda(t_dda *dda, t_scene *sc);

/* ---- src/render_wall.c ------------------------------------------------ */
void		calc_perp_dist(t_wall *wall, t_dda *dda, t_ray *ray, t_player *pl);
void		ft_calc_tex_x(t_wall *wall, t_ray_calc *rc, t_tex *tex);
void		ft_draw_wall_column(t_mlx *mlx, t_wall *wall, t_tex *tex, int x);

#endif
