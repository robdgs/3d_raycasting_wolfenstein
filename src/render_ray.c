/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 17:30:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/04/08 11:06:58 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_init_ray(t_ray *ray, t_player *pl, int x)
{
	double	cam_x;

	cam_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->dx = pl->dir_x + pl->plane_x * cam_x;
	ray->dy = pl->dir_y + pl->plane_y * cam_x;
}

void	ft_init_dda_deltas(t_dda *dda, t_ray *ray, t_player *pl)
{
	dda->map_x = (int)pl->px;
	dda->map_y = (int)pl->py;
	if (ray->dx == 0)
		dda->delta_x = 1e30;
	else
		dda->delta_x = fabs(1.0 / ray->dx);
	if (ray->dy == 0)
		dda->delta_y = 1e30;
	else
		dda->delta_y = fabs(1.0 / ray->dy);
}

//Se il raggio va a destra (ray_dx > 0): side_x = (map_x + 1 − px) × delta_x:
//distanza al bordo destro della cella 
//Se il raggio va a sinistra (ray_dx < 0): side_x = (px − map_x) × delta_x:
//distanza al bordo sinistro Stessa logica per Y.
void	ft_init_dda_steps(t_dda *dda, t_ray *ray, t_player *pl)
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

void	ft_perform_dda(t_dda *dda, t_scene *sc)
{
	double	epsilon;

	epsilon = 1e-9;
	dda->side = 0;
	while (1)
	{
		if (dda->side_x < dda->side_y - epsilon)
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
