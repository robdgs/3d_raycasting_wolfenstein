/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:31 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:41:22 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

int	ft_validate_map(t_game *g)
{
	t_scene	*sc;
	char	**work;
	int		starts;
	int		sr;
	int		sc_col;
	int		res;

	sc = &g->scene;
	work = ft_pad_grid(sc->grid, sc->grid_rows, sc->grid_cols);
	if (!work)
		return (ft_err("malloc failed during map validation"));
	ft_stfind(work, sc, &starts, &sr, &sc_col);
	if (ft_check_chars(sc, work) || starts != 1)
	{
		if (starts != 1)
			ft_err("map must have exactly one player start position");
		return (1);
	}
	res = ft_flood(work, sc->grid_rows, sc->grid_cols, sr, sc_col);
	ft_free_grid(work);
	if (res)
		return (ft_err("map is not enclosed by walls"));
	return (0);
}

static int	ft_get_max_cols(t_strlist *map_lines)
{
	int			max_cols;
	t_strlist	*cur;

	max_cols = 0;
	cur = map_lines;
	while (cur)
	{
		if ((int)ft_strlen(cur->str) > max_cols)
			max_cols = (int)ft_strlen(cur->str);
		cur = cur->next;
	}
	return (max_cols);
}

static int	ft_build_grid(t_scene *sc, t_strlist *map_lins, int rws, int mx_cls)
{
	char	**raw;
	int		i;

	raw = ft_strlist_to_array(map_lins, rws);
	if (!raw)
		return (ft_err("malloc failed"));
	sc->grid_rows = rws;
	sc->grid_cols = mx_cls;
	sc->grid = ft_pad_grid(raw, rws, mx_cls);
	i = 0;
	while (raw[i])
		free(raw[i++]);
	free(raw);
	if (!sc->grid)
		return (ft_err("malloc failed building padded grid"));
	return (0);
}

int	ft_parse_map(t_game *g, t_strlist *map_lines)
{
	t_scene		*sc;
	int			rows;
	int			max_cols;

	sc = &g->scene;
	rows = ft_list_count(map_lines);
	if (rows == 0)
		return (ft_err("empty map"));
	max_cols = ft_get_max_cols(map_lines);
	if (max_cols == 0)
		return (ft_err("map has no content"));
	if (ft_build_grid(sc, map_lines, rows, max_cols))
		return (1);
	return (ft_validate_map(g));
}
