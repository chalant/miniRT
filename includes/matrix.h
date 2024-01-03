/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/03 15:46:09 by ychalant         ###   ########.fr       */
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
int			init_matrix(t_matrix *matrix, float value);
int			homogeneous_matrix(t_matrix **matrix, int rows, int cols);
int			set_diagonal(t_matrix *matrix, float value);

void		matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result);
void		inplace_matmul(t_matrix *mat1, t_matrix *mat2, t_matrix *result);
void		matrix_copy(t_matrix *source, t_matrix *dest);
void		map_row(t_matrix *matrix, void (*op)(float *, float), int index,
				float value);

#endif
