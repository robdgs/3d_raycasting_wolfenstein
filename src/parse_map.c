/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:31 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/20 21:55:01 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

static int	is_start(char ch)
{
	return (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W');
}

static int	is_tile(char ch)
{
	return (ch == '0' || ch == '1' || ch == ' ' || is_start(ch));
}

static char	**pad_grid(char **src, int rows, int cols)
{
	char	**out;
	int		r;
	size_t	src_len;

	out = malloc(sizeof(char *) * (rows + 1));
	if (!out)
		return (NULL);
	r = 0;
	while (r < rows)
	{
		out[r] = malloc(cols + 1);
		if (!out[r])
		{
			while (--r >= 0)
				free(out[r]);
			free(out);
			return (NULL);
		}
		src_len = strlen(src[r]);
		if ((int)src_len > cols)
			src_len = cols;
		memcpy(out[r], src[r], src_len);
		memset(out[r] + src_len, ' ', cols - (int)src_len);
		out[r][cols] = '\0';
		r++;
	}
	out[rows] = NULL;
	return (out);
}

static int	flood(char **grid, int rows, int cols, int r, int c)
{
	if (r < 0 || r >= rows || c < 0 || c >= cols)
		return (1);
	if (grid[r][c] == '1' || grid[r][c] == 'V')
		return (0);
	if (grid[r][c] == ' ')
		return (1);
	grid[r][c] = 'V';
	return (flood(grid, rows, cols, r - 1, c)
		|| flood(grid, rows, cols, r + 1, c)
		|| flood(grid, rows, cols, r, c - 1)
		|| flood(grid, rows, cols, r, c + 1));
}

int	validate_map(t_game *g)
{
	t_scene	*sc;
	char	**work;
	int		starts;
	int		sr;
	int		sc_col;
	int		r;
	int		c;
	int		res;

	sc = &g->scene;
	work = pad_grid(sc->grid, sc->grid_rows, sc->grid_cols);
	if (!work)
		return (err("malloc failed during map validation"));
	starts = 0;
	sr = 0;
	sc_col = 0;
	r = 0;
	while (r < sc->grid_rows)
	{
		c = 0;
		while (c < sc->grid_cols)
		{
			if (!is_tile(work[r][c]))
			{
				r = sc->grid_rows;
				break ;
			}
			if (is_start(work[r][c]))
			{
				starts++;
				sr = r;
				sc_col = c;
				work[r][c] = '0';
			}
			c++;
		}
		r++;
	}
	r = 0;
	while (r < sc->grid_rows)
	{
		c = 0;
		while (c < sc->grid_cols)
		{
			if (!is_tile(sc->grid[r][c]) && !is_start(sc->grid[r][c]))
			{
				/* free work */
				r = 0;
				while (work[r])
					free(work[r++]);
				free(work);
				return (err("invalid character in map"));
			}
			c++;
		}
		r++;
	}
	if (starts != 1)
	{
		r = 0;
		while (work[r])
			free(work[r++]);
		free(work);
		return (err("map must have exactly one player start position"));
	}
	res = flood(work, sc->grid_rows, sc->grid_cols, sr, sc_col);
	r = 0;
	while (work[r])
		free(work[r++]);
	free(work);
	if (res)
		return (err("map is not enclosed by walls"));
	return (0);
}

int	parse_map(t_game *g, t_strlist *map_lines)
{
	t_scene		*sc;
	t_strlist	*cur;
	int			rows;
	int			max_cols;
	int			i;
	char		**raw;

	sc = &g->scene;
	rows = list_count(map_lines);
	if (rows == 0)
		return (err("empty map"));
	max_cols = 0;
	cur = map_lines;
	while (cur)
	{
		if ((int)strlen(cur->str) > max_cols)
			max_cols = (int)strlen(cur->str);
		cur = cur->next;
	}
	if (max_cols == 0)
		return (err("map has no content"));
	raw = strlist_to_array(map_lines, rows);
	if (!raw)
		return (err("malloc failed"));
	sc->grid_rows = rows;
	sc->grid_cols = max_cols;
	/* Build padded version */
	sc->grid = pad_grid(raw, rows, max_cols);
	i = 0;
	while (raw[i])
		free(raw[i++]);
	free(raw);
	if (!sc->grid)
		return (err("malloc failed building padded grid"));
	return (validate_map(g));
}
