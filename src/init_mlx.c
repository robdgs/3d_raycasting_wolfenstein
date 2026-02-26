/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 16:40:11 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 16:40:46 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_init_mlx(t_game *g)
{
	t_mlx	*m;

	m = &g->mlx;
	m->conn = mlx_init();
	if (!m->conn)
		return (ft_err("mlx_init failed"));
	m->win = mlx_new_window(m->conn, WIN_W, WIN_H, WIN_TITLE);
	if (!m->win)
		return (ft_err("mlx_new_window failed"));
	m->img = mlx_new_image(m->conn, WIN_W, WIN_H);
	if (!m->img)
		return (ft_err("mlx_new_image failed"));
	m->buf = mlx_get_data_addr(m->img, &m->bpp, &m->stride, &m->endian);
	if (!m->buf)
		return (ft_err("mlx_get_data_addr failed"));
	return (0);
}
