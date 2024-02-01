/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_atof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:26:34 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/01 16:39:29 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_decimal(char *nbr)
{
	int	dot;
	int	decimal;

	dot = 0;
	decimal = 0;
	while (nbr)
	{
		if (!ft_isdigit(*nbr) && *nbr != '.')
			return (1);
		if (*nbr == '.')
			if (++dot && dot > 1)
				return (1);
		if (dot && ++decimal > 1)
			return (1);
		nbr++;
	}
	return (0);
}

float	ft_atof(int *fail, char *nbr)
{
	char	**decimal;
	int		res;
	int		power;
	int		left_len;

	if (check_decimal(nbr))
	{
		fail = 1;
		return (-1);
	}
	power = 1;
	res = 0;
	decimal = ft_split(nbr, '.');
	if (decimal)
		return (-1);
	left_len = ft_strlen(decimal[0]);
	while (left_len)
	{
		res += decimal[0][left_len - 1] * power;
		power *= 10;
		left_len--;
	}
	res += decimal[1][0] * -10;
	return (ft_clear_ds(decimal), res);
}
