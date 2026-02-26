/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:42 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:17:01 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	ft_set_dir(t_player *pl, char ch)
{
	pl->dir_x = (ch == 'E') - (ch == 'W');
	pl->dir_y = (ch == 'S') - (ch == 'N');
	if (ch == 'N' || ch == 'S')
	{
		pl->plane_x = (ch == 'N') * 0.66 - (ch == 'S') * 0.66;
		pl->plane_y = 0;
	}
	else
	{
		pl->plane_x = 0;
		pl->plane_y = (ch == 'E') * 0.66 - (ch == 'W') * 0.66;
	}
}

static int	ft_find_start(t_scene *sc, t_player *pl)
{
	int		r;
	int		c;
	char	ch;

	r = 0;
	while (r < sc->grid_rows)
	{
		c = 0;
		while (c < sc->grid_cols)
		{
			ch = sc->grid[r][c];
			if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
			{
				pl->px = c + 0.5;
				pl->py = r + 0.5;
				ft_set_dir(pl, ch);
				sc->grid[r][c] = '0';
				return (1);
			}
			c++;
		}
		r++;
	}
	return (0);
}

void	ft_setup_player(t_game *g)
{
	ft_find_start(&g->scene, &g->player);
}

static int	ft_load_one_tex(t_game *g, t_tex *tex, const char *path)
{
	tex->img = mlx_xpm_file_to_image(g->mlx.conn, (char *)path,
			&tex->w, &tex->h);
	if (!tex->img)
	{
		ft_err(path);
		return (ft_err("failed to load texture"));
	}
	tex->data = mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->stride, &tex->endian);
	if (!tex->data)
		return (ft_err("failed to get texture data"));
	return (0);
}

int	ft_load_textures(t_game *g)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (ft_load_one_tex(g, &g->textures[i], g->scene.tex_path[i]))
			return (1);
		i++;
	}
	return (0);
}
