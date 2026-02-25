/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rd-agost <rd-agost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 21:49:56 by rd-agost          #+#    #+#             */
/*   Updated: 2026/02/25 12:48:42 by rd-agost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

int	ft_err(const char *msg)
{
	if (write(2, "Error\n", 6) < 0)
		return (1);
	if (msg)
	{
		if (write(2, msg, ft_strlen(msg)) < 0)
			return (1);
		if (write(2, "\n", 1) < 0)
			return (1);
	}
	return (1);
}
