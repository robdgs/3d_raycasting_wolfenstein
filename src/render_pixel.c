/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 17:30:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/04/08 10:28:35 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_put_pixel(t_mlx *m, int x, int y, int color)
{
	int	offset;

	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	offset = y * (m->stride / (m->bpp / 8)) + x;
	((int *)m->buf)[offset] = color;
}

int	ft_rgb(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

void	ft_draw_background(t_game *g)
{
	int	ceil_color;
	int	floor_color;
	int	x;
	int	y;

	ceil_color = ft_rgb(g->scene.ceil_rgb[0],
			g->scene.ceil_rgb[1], g->scene.ceil_rgb[2]);
	floor_color = ft_rgb(g->scene.floor_rgb[0],
			g->scene.floor_rgb[1], g->scene.floor_rgb[2]);
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			if (y < WIN_H / 2)
				ft_put_pixel(&g->mlx, x, y, ceil_color);
			else
				ft_put_pixel(&g->mlx, x, y, floor_color);
			x++;
		}
		y++;
	}
}
