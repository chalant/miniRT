/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_atof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:26:34 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/12 16:52:33 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	check_input(char *nbr, int decimals)
{
	int	dot;
	int	decimal;

	if ((!nbr || (nbr[0] != '-' && !ft_isdigit(nbr[0]))) ||
		nbr[0] == '.' || nbr[ft_strlen(nbr) - 1] == '.')
		return (1);
	dot = 0;
	decimal = 0;
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
		else if (dot && ++decimal > decimals)
			return (1);
		nbr++;
	}
	return (0);
}

void	process_left(char *left, float *res)
{
	int		len;
	float	power;

	power = 1.0f;
	len = ft_strlen(left);
	while (len > (left[0] == '-'))
	{
		*res += (left[len - 1] - '0') * power;
		power *= 10.0f;
		len--;
	}
}

void	process_right(char *right, float *res)
{
	int		i;
	float	power;

	i = -1;
	power = 10.0f;
	while (right && right[++i])
	{
		*res += (right[i] - '0') / power;
		power *= 10.0f;
	}
}

int	ft_atof(char *nbr, float *res)
{
	char	**tab;
	float	sign;

	if (check_input(nbr, 2))
		return (1);
	tab = ft_split(nbr, '.');
	if (!tab)
		return (1);
	*res = 0.0;
	process_left(tab[0], &*res);
	process_right(tab[1], &*res);
	sign = 1.0f;
	if (nbr[0] == '-')
		sign = -sign;
	*res *= sign;
	return (ft_clear_ds(tab), 0);
}
