/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:07:05 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:27:49 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_check_chars(t_scene *sc, char **work)
{
	int	r;
	int	c;

	r = 0;
	while (r < sc->grid_rows)
	{
		c = 0;
		while (c < sc->grid_cols)
		{
			if (!ft_is_tile(sc->grid[r][c]) && !ft_is_start(sc->grid[r][c]))
			{
				ft_free_grid(work);
				return (ft_err("invalid character in map"));
			}
			c++;
		}
		r++;
	}
	return (0);
}

void	ft_stfind(char **wrk, t_scene *sc, int *strts, int *sr, int *sc_col)
{
	int	r;
	int	c;

	*strts = 0;
	r = 0;
	while (r < sc->grid_rows)
	{
		c = 0;
		while (c < sc->grid_cols)
		{
			if (ft_is_start(wrk[r][c]))
			{
				(*strts)++;
				*sr = r;
				*sc_col = c;
				wrk[r][c] = '0';
			}
			c++;
		}
		r++;
	}
}

void	ft_copy_padded_row(char *dst, char *src, int cols)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if ((int)src_len > cols)
		src_len = cols;
	ft_memcpy(dst, src, src_len);
	ft_memset(dst + src_len, ' ', cols - (int)src_len);
	dst[cols] = '\0';
}

char	**ft_alloc_grid_rows(int rows, int cols)
{
	char	**out;
	int		r;

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
		r++;
	}
	out[rows] = NULL;
	return (out);
}

char	**ft_pad_grid(char **src, int rows, int cols)
{
	char	**out;
	int		r;

	out = ft_alloc_grid_rows(rows, cols);
	if (!out)
		return (NULL);
	r = 0;
	while (r < rows)
	{
		ft_copy_padded_row(out[r], src[r], cols);
		r++;
	}
	return (out);
}
