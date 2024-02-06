/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_atof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:26:34 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/06 16:11:55 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	check_input(char *nbr)
{
	int	dot;
	int	decimal;

	dot = 0;
	decimal = 0;
	if ((nbr[0] != '-' && !ft_isdigit(nbr[0])) || nbr[0] == '.')
		return (1);
	nbr++;
	while (*nbr)
	{
		if (!ft_isdigit(*nbr) && *nbr != '.')
			return (1);
		else if (*nbr == '.')
		{
			if (++dot && dot > 1)
				return (1);
		}
		else if (dot && ++decimal > 1)
			return (1);
		nbr++;
	}
	return (0);
}

float	check_sign(char *nbr)
{
	if (nbr[0] == '-')
		return (-1.0);
	else
		return (1.0);
}

int	ft_atof(char *nbr, float *res)
{
	char	**decimal;
	float	sign;
	float	power;
	int		left_len;

	if (check_input(nbr))
		return (1);
	*res = 0.0;
	power = 1.0;
	sign = check_sign(nbr);
	decimal = ft_split(nbr, '.');
	if (!decimal)
		return (1);
	left_len = ft_strlen(decimal[0]);
	while (left_len > (sign == -1))
	{
		*res += (decimal[0][left_len - 1] - '0') * power;
		power *= 10.0;
		left_len--;
	}
	if (decimal[1])
		*res += (decimal[1][0] - '0') / 10.0f;
	*res *= sign;
	return (ft_clear_ds(decimal), 0);
}
