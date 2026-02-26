/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:00:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 16:48:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_parse_one_component(const char **p, int *value)
{
	char	*end;
	long	val;

	if (**p == '\0' || **p == '\n')
		return (ft_err("color needs 3 components"));
	val = strtol(*p, &end, 10);
	if (end == *p)
		return (ft_err("invalid color value"));
	if (val < 0 || val > 255)
		return (ft_err("color value out of range [0,255]"));
	*value = (int)val;
	*p = end;
	return (0);
}

int	ft_parse_color_line(int *rgb, const char *rest)
{
	const char	*p;
	int			i;

	p = rest;
	while (*p == ' ' || *p == '\t')
		p++;
	i = 0;
	while (i < 3)
	{
		if (ft_parse_one_component(&p, &rgb[i++]))
			return (1);
		while (*p == ' ' || *p == '\t')
			p++;
		if (i < 3)
		{
			if (*p != ',')
				return (ft_err("color components must be comma-separated"));
			p++;
		}
	}
	if (*p != '\0' && *p != '\n' && *p != ' ' && *p != '\t')
		return (ft_err("extra data after color"));
	return (0);
}
