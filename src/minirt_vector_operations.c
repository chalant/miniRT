/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:07:19 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:36:21 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
