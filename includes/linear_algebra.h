/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linear_algebra.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/26 12:06:51 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINEAR_ALGEBRA_H
# define LINEAR_ALGEBRA_H

typedef struct s_matrix
{
	float	**points;
	int		rows;
	int		cols;
}			t_matrix;

float	dot_product(float *v1, float *v2, int n);
float	*add_vectors(float *v1, float *v2, float *result, int n);
float	*subtract_vectors(float *v1, float *v2, float *result, int n);
float	*cross_product(float v1[3], float v2[3], float result[3]);
float	*scale_vector(float *vector, float scale, float *result, int n);
float	*normalize_vector(float *vector, float *result, int n);
float	vector_length(float *vector, int n);
float	*copy_vector(float *src, float *dst, int n);

int		create_matrix(t_matrix *matrix, int rows, int cols);
int		delete_matrix(t_matrix *matrix);
int		init_matrix(t_matrix *matrix, float value);
int		homogeneous_matrix(t_matrix *matrix, int rows, int cols);
int		set_diagonal(t_matrix *matrix, float value);

void	matrix_product(t_matrix *mat1, t_matrix *mat2, t_matrix *result, int n);
void	invert_matrix(t_matrix *matrix, t_matrix *result,
			t_matrix *identity, int n);
void	matrix_copy(t_matrix *source, t_matrix *dest, int n);
int		matvec_product(t_matrix *mat, float *vec, float *result);

int		x_rotation(t_matrix *matrix, float angle);
int		y_rotation(t_matrix *matrix, float angle);
int		z_rotation(t_matrix *matrix, float angle);

#endif
