/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:00:00 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 16:48:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	ft_is_xpm(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 5)
		return (0);
	return (ft_strcmp(path + len - 4, ".xpm") == 0);
}

int	ft_parse_texture_line(t_scene *sc, int dir, const char *rest)
{
	const char	*p;
	char		*path;

	if (sc->tex_path[dir])
		return (ft_err("duplicate texture identifier"));
	p = rest;
	while (*p == ' ' || *p == '\t')
		p++;
	path = strdup(p);
	if (!path)
		return (ft_err("malloc failed"));
	if (!ft_is_xpm(path))
	{
		free(path);
		return (ft_err("texture path must end with .xpm"));
	}
	sc->tex_path[dir] = path;
	return (0);
}
