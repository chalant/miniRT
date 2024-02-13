/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_projection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:06:34 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:06:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	perspective_projector(t_matrix *matrix, t_display *display, t_camera *camera)
{
	float		t;

	t = fabsf(1 / tanf(to_rad(camera->fov / 2.0f)));
	if (homogeneous_matrix(matrix, 3, 3))
		return (-1);
	matrix->points[0][0] = t / display->aspect_ratio;
	matrix->points[1][1] = t;
	matrix->points[2][2] = camera->far / (camera->far - camera->near);
	matrix->points[2][3] = (camera->far * camera->near) / (camera->far - camera->near);
	matrix->points[3][2] = -1.0f;
	matrix->points[3][3] = 0.0f;
	return (0);
}
