/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:30:29 by alexphil          #+#    #+#             */
/*   Updated: 2024/02/01 18:40:42 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "minirt.h"
# include "get_next_line.h"
# include <fcntl.h>

typedef struct s_import
{
	t_minirt	*minirt;
	int			fd;
	int			ambient;
	int			camera;
	int			light;
}	t_import;

typedef enum e_ranges
{
	LIGHT,
	XYZ,
	RGB,
	NORMAL,
	DIMENSION,
	FOV,
}	t_ranges;

// Map

// Read

// Process

// Argument to float

#endif