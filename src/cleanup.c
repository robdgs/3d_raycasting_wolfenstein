/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:49:50 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:03:13 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

void	ft_free_scene(t_scene *sc)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (sc->tex_path[i])
		{
			free(sc->tex_path[i]);
			sc->tex_path[i] = NULL;
		}
		i++;
	}
	if (sc->grid)
	{
		i = 0;
		while (sc->grid[i])
		{
			free(sc->grid[i]);
			sc->grid[i] = NULL;
			i++;
		}
		free(sc->grid);
		sc->grid = NULL;
	}
}

static void	ft_free_mlx(t_game *g)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (g->textures[i].img)
		{
			mlx_destroy_image(g->mlx.conn, g->textures[i].img);
			g->textures[i].img = NULL;
		}
		i++;
	}
	if (g->mlx.img)
	{
		mlx_destroy_image(g->mlx.conn, g->mlx.img);
		g->mlx.img = NULL;
	}
	if (g->mlx.win)
	{
		mlx_destroy_window(g->mlx.conn, g->mlx.win);
		g->mlx.win = NULL;
	}
	if (g->mlx.conn)
	{
		mlx_destroy_display(g->mlx.conn);
		free(g->mlx.conn);
		g->mlx.conn = NULL;
	}
}

void	ft_cleanup_and_exit(t_game *g, int code)
{
	ft_free_mlx(g);
	ft_free_scene(&g->scene);
	exit(code);
}
