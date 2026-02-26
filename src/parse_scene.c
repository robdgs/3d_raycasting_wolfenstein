/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:24 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 16:48:10 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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
