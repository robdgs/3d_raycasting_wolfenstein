/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:17:42 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/26 18:22:17 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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
