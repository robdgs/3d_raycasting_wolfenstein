/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:22:17 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:26:20 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_count_chars(const char *s, char c)
{
	int	cnt;

	cnt = 0;
	while (*s)
	{
		if (*s == c)
			cnt++;
		s++;
	}
	return (cnt);
}

int	ft_str_only_whitespace(const char *s)
{
	while (*s)
	{
		if (*s != ' ' && *s != '\t' && *s != '\n' && *s != '\r')
			return (0);
		s++;
	}
	return (1);
}
