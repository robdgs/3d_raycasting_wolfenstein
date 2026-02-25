/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:05:23 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:53:37 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_on_close(t_game *g)
{
	ft_cleanup_and_exit(g, 0);
	return (0);
}

int	ft_on_keydown(int key, t_game *g)
{
	if (key == KEY_ESC)
		ft_cleanup_and_exit(g, 0);
	if (key == KEY_W)     g->input.w     = true;
	if (key == KEY_S)     g->input.s     = true;
	if (key == KEY_A)     g->input.a     = true;
	if (key == KEY_D)     g->input.d     = true;
	if (key == KEY_LEFT)  g->input.left  = true;
	if (key == KEY_RIGHT) g->input.right = true;
	return (0);
}

int	ft_on_keyup(int key, t_game *g)
{
	if (key == KEY_W)     g->input.w     = false;
	if (key == KEY_S)     g->input.s     = false;
	if (key == KEY_A)     g->input.a     = false;
	if (key == KEY_D)     g->input.d     = false;
	if (key == KEY_LEFT)  g->input.left  = false;
	if (key == KEY_RIGHT) g->input.right = false;
	return (0);
}
