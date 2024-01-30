/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_import.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:24:32 by alexphil          #+#    #+#             */
/*   Updated: 2024/01/30 14:30:48 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_extension(char *filename, char *extension)
{
	int	i;
	int	file_len;
	int	ext_len;
	
	if (!filename || !extension || filename[0] == '.')
		return (1);
	i = 0;
	file_len = ft_strlen(filename);
	ext_len = ft_strlen(extension);
	while (filename[i] != '.')
		i++;
	if (!ft_strncmp(&filename[i], extension, ext_len) && i + ext_len == file_len)
		return (0);
	return (1);
}

int	import_map(t_minirt *minirt, char **av)
{
	(void)minirt;

	if (check_extension(av[1], ".rt"))
		return (1); // TODO: PLUG INTO THE PROJECT ERROR MANAGEMENT
	return (0);
}

// [X] Check if input is a .rt file and check that it's not JUST ".rt"
// [ ] Read line per line with GNL
// [ ] Split line against blanks and check if it's n-sized as expected PER type
// [ ] If a A, C or L type, check if already existing in the scene (if yes, error)
// [ ] Dispatch each model data into a new object struct