/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_process_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 13:19:18 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/08 18:11:34 by alexphil         ###   ########.fr       */
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

// Simplify code with 2 funcs instead of the current 6 ones
// set_value(float *value, char *info, int type)
// set_values(float *values, char *inf, int type)