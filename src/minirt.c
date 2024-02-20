/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/12 15:31:32 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "map.h"

int	main(int argc, char *argv[])
{
	t_minirt	minirt;

	if (argc != 2)
		return (1);
	if (minirt_init(&minirt) < 0)
		close_program(&minirt, 1);
	if (import_map(&minirt, argv))
		close_program(&minirt, 1);
	if (set_camera_transform(&minirt, &minirt.camera) < 0)
		close_program(&minirt, 1);
	if (load_bump_maps(&minirt) < 0)
		close_program(&minirt, 1);
	if (set_materials(&minirt) < 0)
		close_program(&minirt, 1);
	mlx_loop(minirt.mlx);
	return (0);
}
