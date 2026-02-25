/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:24 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 13:17:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

static int	ft_is_xpm(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 5)
		return (0);
	return (ft_strcmp(path + len - 4, ".xpm") == 0);
}

static int	ft_parse_texture_line(t_scene *sc, int dir, const char *rest)
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

static int	ft_parse_one_component(const char **p, int *value)
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

static int	ft_parse_color_line(int *rgb, const char *rest)
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

static int	ft_line_is_map_row(const char *s)
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
		/* Any character outside the legal tile set → not a map row */
		if (*p != '0' && *p != '1'
			&& *p != 'N' && *p != 'S' && *p != 'E' && *p != 'W')
			return (0);
		has_tile = 1;
		p++;
	}
	return (has_tile);
}

static int	ft_headers_complete(const t_scene *sc)
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

static int	ft_dispatch_header(t_scene *sc, const char *line)
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

static void	ft_init_scene_data(t_scene *sc)
{
	sc->tex_path[0] = sc->tex_path[1] = NULL;
	sc->tex_path[2] = sc->tex_path[3] = NULL;
	sc->floor_rgb[0] = sc->floor_rgb[1] = sc->floor_rgb[2] = -1;
	sc->ceil_rgb[0] = sc->ceil_rgb[1] = sc->ceil_rgb[2] = -1;
	sc->grid = NULL;
	sc->grid_rows = 0;
	sc->grid_cols = 0;
}

static int	ft_process_line(t_scene *sc, char *line, int *in_map,
		t_strlist **map_lst)
{
	if (!*in_map && ft_str_only_whitespace(line))
		return (0);
	if (!*in_map && ft_line_is_map_row(line))
	{
		if (!ft_headers_complete(sc))
			return (ft_err("map started before all headers were parsed"));
		*in_map = 1;
	}
	if (!*in_map)
		return (ft_dispatch_header(sc, line));
	*map_lst = ft_strlist_append(*map_lst, line);
	if (!*map_lst)
		return (ft_err("malloc failed"));
	return (0);
}

static int	ft_read_scene_file(t_scene *sc, int fd, t_strlist **map_lst)
{
	char	*line;
	int		in_map;
	int		res;

	in_map = 0;
	while (1)
	{
		line = ft_gnl(fd);
		if (!line)
			break ;
		ft_trim_newline(line);
		res = ft_process_line(sc, line, &in_map, map_lst);
		if (!in_map || res)
			free(line);
		if (res)
			return (res);
	}
	if (!in_map || !*map_lst)
		return (ft_err("no map found in scene file"));
	return (0);
}

int	ft_parse_scene(t_game *g, const char *path)
{
	int			fd;
	t_strlist	*map_lst;
	int			res;

	ft_init_scene_data(&g->scene);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ft_err("cannot open scene file"));
	map_lst = NULL;
	res = ft_read_scene_file(&g->scene, fd, &map_lst);
	close(fd);
	if (res)
	{
		ft_free_strlist(map_lst);
		return (res);
	}
	res = ft_parse_map(g, map_lst);
	ft_free_strlist(map_lst);
	return (res);
}
