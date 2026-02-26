/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:31 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:17:26 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	ft_check_map_chars(t_game *g, char **work, int *starts)
{
	t_start_info	info;
	t_grid			grid;

	ft_stfind(work, &g->scene, &info);
	*starts = info.count;
	if (ft_check_chars(&g->scene, work) || *starts != 1)
	{
		if (*starts != 1)
			ft_err("map must have exactly one player start position");
		return (1);
	}
	grid.data = work;
	grid.rows = g->scene.grid_rows;
	grid.cols = g->scene.grid_cols;
	return (ft_flood(&grid, info.row, info.col));
}

int	ft_validate_map(t_game *g)
{
	char	**work;
	int		starts;
	int		res;

	work = ft_pad_grid(g->scene.grid, g->scene.grid_rows, g->scene.grid_cols);
	if (!work)
		return (ft_err("malloc failed during map validation"));
	res = ft_check_map_chars(g, work, &starts);
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
