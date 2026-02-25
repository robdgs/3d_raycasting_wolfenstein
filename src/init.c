/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:42 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 13:17:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

static void	ft_set_dir(t_player *pl, char ch)
{
	if (ch == 'N')
	{
		pl->dir_x = 0;
		pl->dir_y = -1;
		pl->plane_x = 0.66;
		pl->plane_y = 0;
	}
	else if (ch == 'S')
	{
		pl->dir_x = 0;
		pl->dir_y = 1;
		pl->plane_x = -0.66;
		pl->plane_y = 0;
	}
	else if (ch == 'E')
	{
		pl->dir_x = 1;
		pl->dir_y = 0;
		pl->plane_x = 0;
		pl->plane_y = 0.66;
	}
	else
	{
		pl->dir_x = -1;
		pl->dir_y = 0;
		pl->plane_x = 0;
		pl->plane_y = -0.66;
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
