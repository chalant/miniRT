/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 13:19:18 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/26 11:51:32 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	set_light(float *light, char *info)
{
	char	**tab;

	tab = check_ranges(info, LIGHT);
	if (!tab)
		return (1);
	if (ft_atof(tab[0], &*light))
		return (ft_clear_ds(tab), 1);
	return (ft_clear_ds(tab), 0);
}

int	set_rgb(float *rgb, char *info)
{
	char	**tab;
	int		i;
	int		_rgb[3];

	tab = check_ranges(info, RGB);
	if (!tab)
		return (1);
	i = -1;
	while (++i < 3)
		_rgb[i] = ft_atoi(tab[i]);
	normalize_rgb(_rgb, rgb);
	return (ft_clear_ds(tab), 0);
}

int	set_xyz(float *xyz, char *info)
{
	char	**tab;
	int		i;

	tab = check_ranges(info, XYZ);
	if (!tab)
		return (1);
	i = -1;
	while (++i < 3)
		if (ft_atof(tab[i], &xyz[i]))
			return (ft_clear_ds(tab), 1);
	xyz[i] = 1;
	return (ft_clear_ds(tab), 0);
}
