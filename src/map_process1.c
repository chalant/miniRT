/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:32:43 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/26 11:58:57 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	expected_values(int type)
{
	if (type == LIGHT || type == UNIT || type == FOV || type == INDEX)
		return (1);
	else
		return (3);
}

int	check_integer(char *value, int type)
{
	int	integer;
	int	sus;
	int	i;

	if (value[0] != '-' && !ft_isdigit(value[0]))
		return (1);
	sus = 0;
	i = 0;
	while (value[++i])
		if (!ft_isdigit(value[i]))
			sus++;
	if (sus > 1 || (sus == 1 && value[ft_strlen(value) - 1] != '\n'))
		return (1);
	integer = ft_atoi(value);
	if (type == RGB && integer >= 0 && integer <= 255)
		return (0);
	else if (type == FOV && integer >= 0 && integer <= 180)
		return (0);
	else if (type == INDEX && integer >= 0)
		return (0);
	else
		return (1);
}

int	check_decimal(char *value, int type)
{
	float	decimal;

	if (ft_atof(value, &decimal))
		return (1);
	if (type == LIGHT && decimal >= 0.0 && decimal <= 1.0)
		return (0);
	else if (type == NORMAL)
		return (0);
	else if (type == UNIT && decimal > 0.0)
		return (0);
	else if (type == XYZ)
		return (0);
	return (1);
}

char	**check_ranges(char *values, int type)
{
	char	**tab;
	int		i;

	tab = ft_split(values, ',');
	if (!tab)
		return (NULL);
	if ((int)ft_count_strings(tab) != expected_values(type))
		return (ft_clear_ds(tab), NULL);
	i = 0;
	while (tab[i])
	{
		if (type == RGB || type == FOV || type == INDEX)
		{
			if (check_integer(tab[i], type))
				return (ft_clear_ds(tab), NULL);
		}
		else
			if (check_decimal(tab[i], type))
				return (ft_clear_ds(tab), NULL);
		i++;
	}
	return (tab);
}
