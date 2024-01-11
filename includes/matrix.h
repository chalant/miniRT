/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/05 15:22:19 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

typedef struct s_matrix
{
	float	**points;
	int		rows;
	int		cols;
}			t_matrix;

float		**get_points(t_matrix *matrix);
float		get_point(t_matrix *matrix, int i, int j);
void		set_point(t_matrix *matrix, int i, int j, float value);

int			create_matrix(t_matrix *matrix, int rows, int cols);
int			delete_matrix(t_matrix *matrix);
int			init_matrix(t_matrix *matrix, float value);
int			homogeneous_matrix(t_matrix **matrix, int rows, int cols);
int			set_diagonal(t_matrix *matrix, float value);

void		matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result, int n);
void		invert_matrix(t_matrix *matrix, int n);
void		mattranspose(t_matrix *matrix, t_matrix *result);
void		inplace_matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result, int n);
void		matrix_copy(t_matrix *source, t_matrix *dest, int n);
int			matdiff(t_matrix *mat1, t_matrix *mat2, t_matrix *result);
int			matsum(t_matrix *mat1, t_matrix *mat2, t_matrix *result);
int			smatmul(t_matrix *mat, float scalar, t_matrix *result);
int			vmatmul(t_matrix *mat, float *vec, float *result);

int			x_rotation(t_matrix *matrix, float angle);
int			y_rotation(t_matrix *matrix, float angle);
int			z_rotation(t_matrix *matrix, float angle);

#endif
