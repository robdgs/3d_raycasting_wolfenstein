/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:13 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/20 21:53:37 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

int	on_close(t_game *g)
{
	cleanup_and_exit(g, 0);
	return (0);
}

int	on_keydown(int key, t_game *g)
{
	if (key == KEY_ESC)
		cleanup_and_exit(g, 0);
	if (key == KEY_W)     g->input.w     = true;
	if (key == KEY_S)     g->input.s     = true;
	if (key == KEY_A)     g->input.a     = true;
	if (key == KEY_D)     g->input.d     = true;
	if (key == KEY_LEFT)  g->input.left  = true;
	if (key == KEY_RIGHT) g->input.right = true;
	return (0);
}

int	on_keyup(int key, t_game *g)
{
	if (key == KEY_W)     g->input.w     = false;
	if (key == KEY_S)     g->input.s     = false;
	if (key == KEY_A)     g->input.a     = false;
	if (key == KEY_D)     g->input.d     = false;
	if (key == KEY_LEFT)  g->input.left  = false;
	if (key == KEY_RIGHT) g->input.right = false;
	return (0);
}

static void	rotate(t_player *pl, double angle)
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

static void	try_move(t_game *g, double dx, double dy)
{
	t_player	*pl;
	t_scene		*sc;
	double		nx;
	double		ny;

	pl = &g->player;
	sc = &g->scene;
	nx = pl->px + dx;
	ny = pl->py + dy;
	/* Slide along walls by checking axes separately */
	if ((int)nx >= 0 && (int)nx < sc->grid_cols
		&& (int)pl->py >= 0 && (int)pl->py < sc->grid_rows
		&& sc->grid[(int)pl->py][(int)nx] != '1')
		pl->px = nx;
	if ((int)pl->px >= 0 && (int)pl->px < sc->grid_cols
		&& (int)ny >= 0 && (int)ny < sc->grid_rows
		&& sc->grid[(int)ny][(int)pl->px] != '1')
		pl->py = ny;
}

int	game_loop(t_game *g)
{
	t_player	*pl;

	pl = &g->player;
	if (g->input.w)
		try_move(g, pl->dir_x * MOVE_SPD, pl->dir_y * MOVE_SPD);
	if (g->input.s)
		try_move(g, -pl->dir_x * MOVE_SPD, -pl->dir_y * MOVE_SPD);
	if (g->input.a)
		try_move(g, pl->dir_y * MOVE_SPD, -pl->dir_x * MOVE_SPD);
	if (g->input.d)
		try_move(g, -pl->dir_y * MOVE_SPD, pl->dir_x * MOVE_SPD);
	if (g->input.left)
		rotate(pl, -ROT_SPD);
	if (g->input.right)
		rotate(pl, ROT_SPD);
	render_frame(g);
	return (0);
}
