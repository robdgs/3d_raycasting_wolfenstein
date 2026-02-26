/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:13 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:29:59 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	ft_rotate_vec(double *x, double *y, double cos_a, double sin_a)
{
	double	old_x;
	double	old_y;

	old_x = *x;
	old_y = *y;
	*x = old_x * cos_a - old_y * sin_a;
	*y = old_x * sin_a + old_y * cos_a;
}

static void	ft_rotate(t_player *pl, double angle)
{
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	ft_rotate_vec(&pl->dir_x, &pl->dir_y, cos_a, sin_a);
	ft_rotate_vec(&pl->plane_x, &pl->plane_y, cos_a, sin_a);
}

static int	ft_check_collision(t_scene *sc, double x, double y)
{
	double	margin;

	margin = 0.2;
	if (ft_is_wall(sc, x - margin, y - margin))
		return (1);
	if (ft_is_wall(sc, x + margin, y - margin))
		return (1);
	if (ft_is_wall(sc, x - margin, y + margin))
		return (1);
	if (ft_is_wall(sc, x + margin, y + margin))
		return (1);
	return (0);
}

static void	ft_try_move(t_game *g, double dx, double dy)
{
	t_player	*pl;
	double		nx;
	double		ny;

	pl = &g->player;
	nx = pl->px + dx;
	ny = pl->py + dy;
	if (!ft_check_collision(&g->scene, nx, ny))
	{
		pl->px = nx;
		pl->py = ny;
	}
	else if (!ft_check_collision(&g->scene, nx, pl->py))
		pl->px = nx;
	else if (!ft_check_collision(&g->scene, pl->px, ny))
		pl->py = ny;
}

int	ft_game_loop(t_game *g)
{
	t_player	*pl;

	pl = &g->player;
	if (g->input.w)
		ft_try_move(g, pl->dir_x * MOVE_SPD, pl->dir_y * MOVE_SPD);
	if (g->input.s)
		ft_try_move(g, -pl->dir_x * MOVE_SPD, -pl->dir_y * MOVE_SPD);
	if (g->input.a)
		ft_try_move(g, pl->dir_y * MOVE_SPD, -pl->dir_x * MOVE_SPD);
	if (g->input.d)
		ft_try_move(g, -pl->dir_y * MOVE_SPD, pl->dir_x * MOVE_SPD);
	if (g->input.left)
		ft_rotate(pl, -ROT_SPD);
	if (g->input.right)
		ft_rotate(pl, ROT_SPD);
	ft_render_frame(g);
	return (0);
}
