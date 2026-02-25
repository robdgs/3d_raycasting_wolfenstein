/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:37 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:02:39 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	ft_check_extension(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 5)
		return (0);
	return (ft_strcmp(path + len - 4, ".cub") == 0);
}

static void	ft_zero_game(t_game *g)
{
	ft_memset(g, 0, sizeof(t_game));
	g->scene.floor_rgb[0] = -1;
	g->scene.ceil_rgb[0] = -1;
}

static void	ft_init_and_run(t_game *g)
{
	if (ft_init_mlx(g))
	{
		ft_free_scene(&g->scene);
		exit(1);
	}
	if (ft_load_textures(g))
		ft_cleanup_and_exit(g, 1);
	ft_setup_player(g);
	mlx_hook(g->mlx.win, 2, 1L << 0, (void *)ft_on_keydown, g);
	mlx_hook(g->mlx.win, 3, 1L << 1, (void *)ft_on_keyup, g);
	mlx_hook(g->mlx.win, 17, 0, (void *)ft_on_close, g);
	mlx_loop_hook(g->mlx.conn, (void *)ft_game_loop, g);
	mlx_loop(g->mlx.conn);
	ft_cleanup_and_exit(g, 0);
}

int	main(int argc, char **argv)
{
	t_game	g;

	if (argc != 2)
	{
		ft_err("usage: ./cub3D <scene.cub>");
		return (1);
	}
	if (!ft_check_extension(argv[1]))
	{
		ft_err("scene file must have a .cub extension");
		return (1);
	}
	ft_zero_game(&g);
	if (ft_parse_scene(&g, argv[1]))
	{
		ft_free_scene(&g.scene);
		return (1);
	}
	ft_init_and_run(&g);
	return (0);
}
