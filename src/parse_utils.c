/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:52 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:26:20 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static char	*ft_gnl_eof(char *buf)
{
	char	*line;

	if (buf[0] == '\0')
		return (NULL);
	line = strdup(buf);
	buf[0] = '\0';
	return (line);
}

static char	*ft_gnl_read_loop(int fd, char *buf)
{
	char		rbuf[GNL_BUFSIZE + 1];
	ssize_t		n;
	size_t		used;
	size_t		copy_n;

	while (!strchr(buf, '\n'))
	{
		n = read(fd, rbuf, GNL_BUFSIZE);
		if (n <= 0)
			return (ft_gnl_eof(buf));
		rbuf[n] = '\0';
		used = ft_strlen(buf);
		copy_n = (size_t)n;
		if (copy_n > GNL_BUFSIZE - used)
			copy_n = GNL_BUFSIZE - used;
		memcpy(buf + used, rbuf, copy_n);
		buf[used + copy_n] = '\0';
	}
	return (buf);
}

static char	*ft_gnl_extract(char *buf)
{
	char	*nl;
	char	*line;
	size_t	len;

	nl = strchr(buf, '\n');
	len = (size_t)(nl - buf) + 1;
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	memcpy(line, buf, len);
	line[len] = '\0';
	memmove(buf, nl + 1, ft_strlen(nl + 1) + 1);
	return (line);
}

char	*ft_gnl(int fd)
{
	static char	buf[GNL_BUFSIZE + 1];
	char		*result;

	result = ft_gnl_read_loop(fd, buf);
	if (!result)
		return (NULL);
	return (ft_gnl_extract(buf));
}

char	*ft_trim_newline(char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
	{
		s[len - 1] = '\0';
		len--;
	}
	return (s);
}
