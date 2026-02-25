/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:15:37 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:55:30 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_flood(char **grid, int rows, int cols, int r, int c)
{
	if (r < 0 || r >= rows || c < 0 || c >= cols)
		return (1);
	if (grid[r][c] == '1' || grid[r][c] == 'V')
		return (0);
	if (grid[r][c] == ' ')
		return (1);
	grid[r][c] = 'V';
	return (ft_flood(grid, rows, cols, r - 1, c)
		|| ft_flood(grid, rows, cols, r + 1, c)
		|| ft_flood(grid, rows, cols, r, c - 1)
		|| ft_flood(grid, rows, cols, r, c + 1));
}

void	ft_free_grid(char **grid)
{
	int	i;

	i = 0;
	while (grid[i])
		free(grid[i++]);
	free(grid);
}

int	ft_is_start(char ch)
{
	return (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W');
}

int	ft_is_tile(char ch)
{
	return (ch == '0' || ch == '1' || ch == ' ' || ft_is_start(ch));
}
