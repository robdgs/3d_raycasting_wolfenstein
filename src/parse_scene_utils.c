/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:00:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:13:07 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_line_is_map_row(const char *s)
{
	const char	*p;
	int			has_tile;

	p = s;
	has_tile = 0;
	while (*p)
	{
		if (*p == ' ' || *p == '\t')
		{
			p++;
			continue ;
		}
		if (*p != '0' && *p != '1'
			&& *p != 'N' && *p != 'S' && *p != 'E' && *p != 'W')
			return (0);
		has_tile = 1;
		p++;
	}
	return (has_tile);
}

int	ft_headers_complete(const t_scene *sc)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!sc->tex_path[i])
			return (0);
		i++;
	}
	if (sc->floor_rgb[0] == -1 || sc->ceil_rgb[0] == -1)
		return (0);
	return (1);
}

int	ft_dispatch_header(t_scene *sc, const char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (ft_parse_texture_line(sc, DIR_NO, line + 3));
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (ft_parse_texture_line(sc, DIR_SO, line + 3));
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (ft_parse_texture_line(sc, DIR_EA, line + 3));
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (ft_parse_texture_line(sc, DIR_WE, line + 3));
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		if (sc->floor_rgb[0] != -1)
			return (ft_err("duplicate floor color"));
		return (ft_parse_color_line(sc->floor_rgb, line + 2));
	}
	if (ft_strncmp(line, "C ", 2) == 0)
	{
		if (sc->ceil_rgb[0] != -1)
			return (ft_err("duplicate ceiling color"));
		return (ft_parse_color_line(sc->ceil_rgb, line + 2));
	}
	return (ft_err("unknown scene identifier"));
}

void	ft_init_scene_data(t_scene *sc)
{
	sc->tex_path[0] = NULL;
	sc->tex_path[1] = NULL;
	sc->tex_path[2] = NULL;
	sc->tex_path[3] = NULL;
	sc->floor_rgb[0] = -1;
	sc->floor_rgb[1] = -1;
	sc->floor_rgb[2] = -1;
	sc->ceil_rgb[0] = -1;
	sc->ceil_rgb[1] = -1;
	sc->ceil_rgb[2] = -1;
	sc->grid = NULL;
	sc->grid_rows = 0;
	sc->grid_cols = 0;
}
