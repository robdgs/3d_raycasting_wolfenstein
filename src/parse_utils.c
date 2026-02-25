/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:50:52 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:52:47 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

char	*ft_gnl(int fd)
{
	static char	buf[GNL_BUFSIZE + 1];
	char		rbuf[GNL_BUFSIZE + 1];
	char		*nl;
	char		*line;
	ssize_t		n;
	size_t		used;
	size_t		copy_n;

	nl = strchr(buf, '\n');
	while (!nl)
	{
		n = read(fd, rbuf, GNL_BUFSIZE);
		if (n <= 0)
		{
			if (buf[0] == '\0')
				return (NULL);
			line = strdup(buf);
			buf[0] = '\0';
			return (line);
		}
		rbuf[n] = '\0';
		used = ft_strlen(buf);
		copy_n = (size_t)n < GNL_BUFSIZE - used ? (size_t)n : GNL_BUFSIZE - used;
		memcpy(buf + used, rbuf, copy_n);
		buf[used + copy_n] = '\0';
		nl = strchr(buf, '\n');
	}
	used = (size_t)(nl - buf) + 1;
	line = malloc(used + 1);
	if (!line)
		return (NULL);
	memcpy(line, buf, used);
	line[used] = '\0';
	memmove(buf, nl + 1, ft_strlen(nl + 1) + 1);
	return (line);
}

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

void	ft_free_strlist(t_strlist *lst)
{
	t_strlist	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->str);
		free(lst);
		lst = tmp;
	}
}

t_strlist	*ft_strlist_append(t_strlist *lst, char *str)
{
	t_strlist	*node;
	t_strlist	*cur;

	node = malloc(sizeof(t_strlist));
	if (!node)
		return (lst);
	node->str = str;
	node->next = NULL;
	if (!lst)
		return (node);
	cur = lst;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
	return (lst);
}

int	ft_list_count(t_strlist *lst)
{
	int	n;

	n = 0;
	while (lst)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

char	**ft_strlist_to_array(t_strlist *lst, int count)
{
	char		**arr;
	t_strlist	*cur;
	int			i;

	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	cur = lst;
	i = 0;
	while (cur)
	{
		arr[i++] = cur->str;
		cur->str = NULL;
		cur = cur->next;
	}
	arr[i] = NULL;
	return (arr);
}
