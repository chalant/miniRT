/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:51:09 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/26 11:51:46 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	set_normal(float *normal, char *info)
{
	char	**tab;
	int		i;

	tab = check_ranges(info, NORMAL);
	if (!tab)
		return (1);
	i = -1;
	while (++i < 3)
		if (ft_atof(tab[i], &normal[i]))
			return (ft_clear_ds(tab), 1);
	normal[i] = 1;
	return (ft_clear_ds(tab), 0);
}

int	set_fov(float *fov, char *info)
{
	char	**tab;

	tab = check_ranges(info, FOV);
	if (!tab)
		return (1);
	*fov = (float)ft_atoi(tab[0]);
	return (ft_clear_ds(tab), 0);
}

int	set_unit(float *unit, char *info)
{
	char	**tab;

	tab = check_ranges(info, UNIT);
	if (!tab)
		return (1);
	if (ft_atof(tab[0], &*unit))
		return (ft_clear_ds(tab), 1);
	return (ft_clear_ds(tab), 0);
}
