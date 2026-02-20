/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:37 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/20 21:54:19 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

static int	check_extension(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = strlen(path);
	if (len < 5)
		return (0);
	return (strcmp(path + len - 4, ".cub") == 0);
}

static void	zero_game(t_game *g)
{
	memset(g, 0, sizeof(t_game));
	g->scene.floor_rgb[0] = -1;
	g->scene.ceil_rgb[0] = -1;
}

int	main(int argc, char **argv)
{
	t_game	g;

	if (argc != 2)
	{
		err("usage: ./cub3D <scene.cub>");
		return (1);
	}
	if (!check_extension(argv[1]))
	{
		err("scene file must have a .cub extension");
		return (1);
	}
	zero_game(&g);
	if (parse_scene(&g, argv[1]))
	{
		free_scene(&g.scene);
		return (1);
	}
	if (init_mlx(&g))
	{
		free_scene(&g.scene);
		return (1);
	}
	if (load_textures(&g))
	{
		cleanup_and_exit(&g, 1);
	}
	setup_player(&g);
	mlx_hook(g.mlx.win, 2,  1L << 0,  (void *)on_keydown, &g);
	mlx_hook(g.mlx.win, 3,  1L << 1,  (void *)on_keyup,   &g);
	mlx_hook(g.mlx.win, 17, 0,        (void *)on_close,   &g);
	mlx_loop_hook(g.mlx.conn, (void *)game_loop, &g);
	mlx_loop(g.mlx.conn);
	cleanup_and_exit(&g, 0);
	return (0);
}
