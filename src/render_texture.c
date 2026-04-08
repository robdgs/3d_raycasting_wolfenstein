/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 17:30:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/04/08 10:29:32 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_tex	*ft_pick_texture(t_game *g, double ray_dx, double ray_dy, int side)
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

int	ft_sample_tex(t_tex *tex, int tx, int ty)
{
	int	offset;

	if (tx < 0)
		tx = 0;
	if (ty < 0)
		ty = 0;
	if (tx >= tex->w)
		tx = tex->w - 1;
	if (ty >= tex->h)
		ty = tex->h - 1;
	offset = ty * (tex->stride / (tex->bpp / 8)) + tx;
	return (((int *)tex->data)[offset]);
}
