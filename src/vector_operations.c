/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:07:19 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/09 14:07:20 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*copy_vector(float *src, float *dst, int n)
{
	while (--n > -1)
		dst[n] = src[n];
	return (dst);
}

float   dot_product(float *v1, float *v2, int n)
{
	float	result;

	result = 0.0f;
	while (--n > -1)
		result += v1[n] * v2[n];
	return (result);
}

float   *cross_product(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return (result);
}

float vector_length(float *vector, int n)
{
	return (sqrtf(dot_product(vector, vector, n)));
}

float	*normalize_vector(float *vector, float *result, int n)
{
	float	magnitude;

	magnitude = vector_length(vector, n);
	while (--n > -1)
		result[n] = vector[n] / magnitude;
	return (result);
}

float	*scale_vector(float *vector, float scale, float *result, int n)
{
	while (--n > -1)
		result[n] = vector[n] * scale;
	return (result);
}

float	*add_vectors(float *v1, float *v2, float *result, int n)
{
	while (--n > -1)
		result[n] = v1[n] + v2[n];
	return (result);
}

float	*subtract_vectors(float *v1, float *v2, float *result, int n)
{
	while (--n > -1)
		result[n] = v1[n] - v2[n];
	return (result);
}
