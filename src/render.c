/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:58 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:11:35 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	cast_column(t_game *g, int x)
{
	t_ray		ray;
	t_dda		dda;
	t_wall		wall;
	t_tex		*tex;
	t_ray_calc	rc;

	init_ray(&ray, &g->player, x);
	init_dda_deltas(&dda, &ray, &g->player);
	init_dda_steps(&dda, &ray, &g->player);
	perform_dda(&dda, &g->scene);
	calc_perp_dist(&wall, &dda, &ray, &g->player);
	tex = pick_texture(g, ray.dx, ray.dy, dda.side);
	rc.dda = &dda;
	rc.ray = &ray;
	rc.pl = &g->player;
	calc_tex_x(&wall, &rc, tex);
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
