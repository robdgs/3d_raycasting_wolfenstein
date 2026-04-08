/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_wall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 17:30:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/04/08 10:36:11 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	calc_perp_dist(t_wall *wall, t_dda *dda, t_ray *ray, t_player *pl)
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

void	ft_calc_tex_x(t_wall *wall, t_ray_calc *rc, t_tex *tex)
{
	double	wall_x;

	if (rc->dda->side == 0)
		wall_x = rc->pl->py + wall->perp_dist * rc->ray->dy;
	else
		wall_x = rc->pl->px + wall->perp_dist * rc->ray->dx;
	wall_x -= floor(wall_x);
	wall->tex_x = (int)(wall_x * tex->w);
	if ((rc->dda->side == 0 && rc->ray->dx < 0)
		|| (rc->dda->side == 1 && rc->ray->dy > 0))
		wall->tex_x = tex->w - wall->tex_x - 1;
	if (wall->tex_x < 0)
		wall->tex_x = 0;
	if (wall->tex_x >= tex->w)
		wall->tex_x = tex->w - 1;
}

void	ft_draw_wall_column(t_mlx *mlx, t_wall *wall, t_tex *tex, int x)
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
		ft_put_pixel(mlx, x, y, ft_sample_tex(tex, wall->tex_x, tex_y));
		y++;
	}
}
