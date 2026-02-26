/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 12:15:37 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:11:35 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_flood(t_grid *grid, int r, int c)
{
	if (r < 0 || r >= grid->rows || c < 0 || c >= grid->cols)
		return (1);
	if (grid->data[r][c] == '1' || grid->data[r][c] == 'V')
		return (0);
	if (grid->data[r][c] == ' ')
		return (1);
	grid->data[r][c] = 'V';
	return (ft_flood(grid, r - 1, c)
		|| ft_flood(grid, r + 1, c)
		|| ft_flood(grid, r, c - 1)
		|| ft_flood(grid, r, c + 1));
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
