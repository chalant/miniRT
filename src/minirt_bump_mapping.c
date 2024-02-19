/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bump_mapping.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:08:40 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:08:41 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*no_perturbation(t_perturbator *perturbator, t_object *object,
	float uv_coords[2], float bump[3])
{
	(void)perturbator;
	(void)object;
	(void)bump;
	(void)uv_coords;
	bump[0] = 0.0f;
	bump[1] = 0.0f;
	bump[2] = 0.0f;
	return (bump);
}

float	compute_du(t_matrix *bump_map, float uv_coords[2], float epsilon)
{
	int	v;
	int	u_e;
	int	u_ne;

	v = abs((int)(uv_coords[1] * (bump_map->rows - 1)) % bump_map->rows);
	u_e = abs((int)((uv_coords[0] + epsilon)
				* (bump_map->cols - 1)) % bump_map->cols);
	u_ne = abs((int)((uv_coords[0] - epsilon)
				* (bump_map->cols - 1)) % bump_map->cols);
	return ((bump_map->points[v][u_e] - bump_map->points[v][u_ne])
		/ (2.0f * epsilon));
}

float	compute_dv(t_matrix *bump_map, float uv_coords[2], float epsilon)
{
	int	v_e;
	int	u;
	int	v_ne;

	v_e = abs((int)((uv_coords[1] + epsilon)
				* (bump_map->rows - 1)) % bump_map->rows);
	u = abs((int)(uv_coords[0] * (bump_map->cols - 1)) % bump_map->cols);
	v_ne = abs((int)((uv_coords[1] - epsilon)
				* (bump_map->rows - 1)) % bump_map->rows);
	return ((bump_map->points[v_e][u] - bump_map->points[v_ne][u])
		/ (2.0f * epsilon));
}

float	*compute_bump(t_perturbator *perturbator, t_object *object,
	float uv_coords[2], float bump[3])
{
	t_matrix	bump_map;
	float		epsilon;
	float		bump_strength;
	float		du;
	float		dv;

	epsilon = 0.001f;
	bump_strength = 0.00001f;
	bump_map = perturbator->map;
	du = compute_du(&bump_map, uv_coords, epsilon);
	dv = compute_dv(&bump_map, uv_coords, epsilon);
	bump[0] = bump_strength * (object->center[0] - du);
	bump[1] = bump_strength * (object->center[1] - dv);
	bump[2] = bump_strength * object->center[2];
	return (bump);
}
