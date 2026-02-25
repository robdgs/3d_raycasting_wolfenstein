/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:58 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:53:37 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

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

static void	cast_column(t_game *g, int x)
{
	t_player	*pl;
	t_scene		*sc;
	double		cam_x;
	double		ray_dx;
	double		ray_dy;
	int			map_x;
	int			map_y;
	double		delta_x;
	double		delta_y;
	double		side_x;
	double		side_y;
	int			step_x;
	int			step_y;
	int			side;
	double		perp_dist;
	int			line_h;
	int			draw_top;
	int			draw_bot;
	double		wall_x;
	int			tex_x;
	t_tex		*tex;
	double		tex_step;
	double		tex_pos;
	int			tex_y;
	int			y;

	pl = &g->player;
	sc = &g->scene;
	cam_x = 2.0 * x / (double)WIN_W - 1.0;
	ray_dx = pl->dir_x + pl->plane_x * cam_x;
	ray_dy = pl->dir_y + pl->plane_y * cam_x;
	map_x = (int)pl->px;
	map_y = (int)pl->py;
	delta_x = (ray_dx == 0) ? 1e30 : fabs(1.0 / ray_dx);
	delta_y = (ray_dy == 0) ? 1e30 : fabs(1.0 / ray_dy);
	if (ray_dx < 0)
	{
		step_x = -1;
		side_x = (pl->px - map_x) * delta_x;
	}
	else
	{
		step_x = 1;
		side_x = (map_x + 1.0 - pl->px) * delta_x;
	}
	if (ray_dy < 0)
	{
		step_y = -1;
		side_y = (pl->py - map_y) * delta_y;
	}
	else
	{
		step_y = 1;
		side_y = (map_y + 1.0 - pl->py) * delta_y;
	}
	side = 0;
	while (1)
	{
		if (side_x < side_y)
		{
			side_x += delta_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_y += delta_y;
			map_y += step_y;
			side = 1;
		}
		if (map_x < 0 || map_y < 0
			|| map_y >= sc->grid_rows || map_x >= sc->grid_cols)
			break ;
		if (sc->grid[map_y][map_x] == '1')
			break ;
	}
	if (side == 0)
		perp_dist = (map_x - pl->px + (1.0 - step_x) / 2.0) / ray_dx;
	else
		perp_dist = (map_y - pl->py + (1.0 - step_y) / 2.0) / ray_dy;
	if (perp_dist <= 0)
		perp_dist = 1e-5;
	line_h = (int)(WIN_H / perp_dist);
	draw_top = WIN_H / 2 - line_h / 2;
	draw_bot = WIN_H / 2 + line_h / 2;
	if (draw_top < 0) draw_top = 0;
	if (draw_bot >= WIN_H) draw_bot = WIN_H - 1;
	tex = pick_texture(g, ray_dx, ray_dy, side);
	if (side == 0)
		wall_x = pl->py + perp_dist * ray_dy;
	else
		wall_x = pl->px + perp_dist * ray_dx;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * tex->w);
	if ((side == 0 && ray_dx < 0) || (side == 1 && ray_dy > 0))
		tex_x = tex->w - tex_x - 1;
	if (tex_x < 0) tex_x = 0;
	if (tex_x >= tex->w) tex_x = tex->w - 1;
	tex_step = (double)tex->h / line_h;
	tex_pos = (draw_top - WIN_H / 2.0 + line_h / 2.0) * tex_step;
	y = draw_top;
	while (y <= draw_bot)
	{
		tex_y = (int)tex_pos & (tex->h - 1);
		tex_pos += tex_step;
		put_pixel(&g->mlx, x, y, sample_tex(tex, tex_x, tex_y));
		y++;
	}
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
