/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_vector_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:06:48 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:39:45 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	*copy_vector(float *src, float *dst, int n)
{
	while (--n > -1)
		dst[n] = src[n];
	return (dst);
}

float	vector_length(float *vector, int n)
{
	return (sqrtf(dot_product(vector, vector, n)));
}
