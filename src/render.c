/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:58 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 13:17:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

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

static void	put_pixel(t_mlx *m, int x, int y, int color)
{
	int	offset;

	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	offset = y * (m->stride / (m->bpp / 8)) + x;
	((int *)m->buf)[offset] = color;
}

static int	rgb(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

static void	draw_background(t_game *g)
{
	int	ceil_color;
	int	floor_color;
	int	x;
	int	y;

	ceil_color = rgb(g->scene.ceil_rgb[0],
			g->scene.ceil_rgb[1], g->scene.ceil_rgb[2]);
	floor_color = rgb(g->scene.floor_rgb[0],
			g->scene.floor_rgb[1], g->scene.floor_rgb[2]);
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			if (y < WIN_H / 2)
				put_pixel(&g->mlx, x, y, ceil_color);
			else
				put_pixel(&g->mlx, x, y, floor_color);
			x++;
		}
		y++;
	}
}

static t_tex	*pick_texture(t_game *g, double ray_dx, double ray_dy, int side)
{
	if (side == 0)
	{
		if (ray_dx > 0)
			return (&g->textures[DIR_EA]);
		return (&g->textures[DIR_WE]);
	}
	if (ray_dy > 0)
		return (&g->textures[DIR_SO]);
	return (&g->textures[DIR_NO]);
}

static int	sample_tex(t_tex *tex, int tx, int ty)
{
	int	offset;

	if (tx < 0) tx = 0;
	if (ty < 0) ty = 0;
	if (tx >= tex->w) tx = tex->w - 1;
	if (ty >= tex->h) ty = tex->h - 1;
	offset = ty * (tex->stride / (tex->bpp / 8)) + tx;
	return (((int *)tex->data)[offset]);
}

static void	init_ray(t_ray *ray, t_player *pl, int x)
{
	double	cam_x;

	cam_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->dx = pl->dir_x + pl->plane_x * cam_x;
	ray->dy = pl->dir_y + pl->plane_y * cam_x;
}

static void	init_dda_deltas(t_dda *dda, t_ray *ray, t_player *pl)
{
	dda->map_x = (int)pl->px;
	dda->map_y = (int)pl->py;
	dda->delta_x = (ray->dx == 0) ? 1e30 : fabs(1.0 / ray->dx);
	dda->delta_y = (ray->dy == 0) ? 1e30 : fabs(1.0 / ray->dy);
}

static void	init_dda_steps(t_dda *dda, t_ray *ray, t_player *pl)
{
	if (ray->dx < 0)
	{
		dda->step_x = -1;
		dda->side_x = (pl->px - dda->map_x) * dda->delta_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_x = (dda->map_x + 1.0 - pl->px) * dda->delta_x;
	}
	if (ray->dy < 0)
	{
		dda->step_y = -1;
		dda->side_y = (pl->py - dda->map_y) * dda->delta_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_y = (dda->map_y + 1.0 - pl->py) * dda->delta_y;
	}
}

static void	perform_dda(t_dda *dda, t_scene *sc)
{
	dda->side = 0;
	while (1)
	{
		if (dda->side_x < dda->side_y)
		{
			dda->side_x += dda->delta_x;
			dda->map_x += dda->step_x;
			dda->side = 0;
		}
		else
		{
			dda->side_y += dda->delta_y;
			dda->map_y += dda->step_y;
			dda->side = 1;
		}
		if (dda->map_x < 0 || dda->map_y < 0
			|| dda->map_y >= sc->grid_rows || dda->map_x >= sc->grid_cols)
			break ;
		if (sc->grid[dda->map_y][dda->map_x] == '1')
			break ;
	}
}

static void	calc_perp_dist(t_wall *wall, t_dda *dda, t_ray *ray, t_player *pl)
{
	if (dda->side == 0)
		wall->perp_dist = (dda->map_x - pl->px
				+ (1.0 - dda->step_x) / 2.0) / ray->dx;
	else
		wall->perp_dist = (dda->map_y - pl->py
				+ (1.0 - dda->step_y) / 2.0) / ray->dy;
	if (wall->perp_dist <= 0)
		wall->perp_dist = 1e-5;
	wall->line_h = (int)(WIN_H / wall->perp_dist);
	wall->draw_top = WIN_H / 2 - wall->line_h / 2;
	wall->draw_bot = WIN_H / 2 + wall->line_h / 2;
	if (wall->draw_top < 0)
		wall->draw_top = 0;
	if (wall->draw_bot >= WIN_H)
		wall->draw_bot = WIN_H - 1;
}

static void	calc_tex_x(t_wall *wall, t_dda *dda, t_ray *ray,
		t_player *pl, t_tex *tex)
{
	double	wall_x;

	if (dda->side == 0)
		wall_x = pl->py + wall->perp_dist * ray->dy;
	else
		wall_x = pl->px + wall->perp_dist * ray->dx;
	wall_x -= floor(wall_x);
	wall->tex_x = (int)(wall_x * tex->w);
	if ((dda->side == 0 && ray->dx < 0) || (dda->side == 1 && ray->dy > 0))
		wall->tex_x = tex->w - wall->tex_x - 1;
	if (wall->tex_x < 0)
		wall->tex_x = 0;
	if (wall->tex_x >= tex->w)
		wall->tex_x = tex->w - 1;
}

static void	draw_wall_column(t_mlx *mlx, t_wall *wall, t_tex *tex, int x)
{
	double	tex_step;
	double	tex_pos;
	int		tex_y;
	int		y;

	tex_step = (double)tex->h / wall->line_h;
	tex_pos = (wall->draw_top - WIN_H / 2.0 + wall->line_h / 2.0) * tex_step;
	y = wall->draw_top;
	while (y <= wall->draw_bot)
	{
		tex_y = (int)tex_pos & (tex->h - 1);
		tex_pos += tex_step;
		put_pixel(mlx, x, y, sample_tex(tex, wall->tex_x, tex_y));
		y++;
	}
}

static void	cast_column(t_game *g, int x)
{
	t_ray	ray;
	t_dda	dda;
	t_wall	wall;
	t_tex	*tex;

	init_ray(&ray, &g->player, x);
	init_dda_deltas(&dda, &ray, &g->player);
	init_dda_steps(&dda, &ray, &g->player);
	perform_dda(&dda, &g->scene);
	calc_perp_dist(&wall, &dda, &ray, &g->player);
	tex = pick_texture(g, ray.dx, ray.dy, dda.side);
	calc_tex_x(&wall, &dda, &ray, &g->player, tex);
	draw_wall_column(&g->mlx, &wall, tex, x);
}

void	ft_render_frame(t_game *g)
{
	int	x;

	draw_background(g);
	x = 0;
	while (x < WIN_W)
	{
		cast_column(g, x);
		x++;
	}
	mlx_put_image_to_window(g->mlx.conn, g->mlx.win, g->mlx.img, 0, 0);
}
