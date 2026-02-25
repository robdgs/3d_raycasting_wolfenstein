/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:13 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:41:44 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

static void	ft_rotate(t_player *pl, double angle)
{
	double	cos_a;
	double	sin_a;
	double	old_dx;
	double	old_dy;
	double	old_px;
	double	old_py;

	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dx = pl->dir_x;
	old_dy = pl->dir_y;
	old_px = pl->plane_x;
	old_py = pl->plane_y;
	pl->dir_x   = old_dx * cos_a - old_dy * sin_a;
	pl->dir_y   = old_dx * sin_a + old_dy * cos_a;
	pl->plane_x = old_px * cos_a - old_py * sin_a;
	pl->plane_y = old_px * sin_a + old_py * cos_a;
}

static void	ft_try_move(t_game *g, double dx, double dy)
{
	t_player	*pl;
	t_scene		*sc;
	double		nx;
	double		ny;

	pl = &g->player;
	sc = &g->scene;
	nx = pl->px + dx;
	ny = pl->py + dy;
	if ((int)nx >= 0 && (int)nx < sc->grid_cols
		&& (int)pl->py >= 0 && (int)pl->py < sc->grid_rows
		&& sc->grid[(int)pl->py][(int)nx] != '1')
		pl->px = nx;
	if ((int)pl->px >= 0 && (int)pl->px < sc->grid_cols
		&& (int)ny >= 0 && (int)ny < sc->grid_rows
		&& sc->grid[(int)ny][(int)pl->px] != '1')
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
