/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_vector_products.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:37:19 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/19 16:37:25 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float	dot_product(float *v1, float *v2, int n)
{
	float	result;

	result = 0.0f;
	while (--n > -1)
		result += v1[n] * v2[n];
	return (result);
}

float	*cross_product(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return (result);
}
