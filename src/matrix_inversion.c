#include "minirt.h"

int	set_matrix(t_matrix *matrix, float value)
{
	int	i;
	int	j;

	i = -1;
	while (++i < matrix->rows)
	{
		j = -1;
		while (++j < matrix->cols) 
			matrix->points[i][j] = value;
	}
	return (0);
}

// void	permute_rows(t_matrix *matrix, int i, int j)
// {
// 	float	*tmp;
// 	int		k;

// 	tmp = matrix->points[i];
// 	matrix->points[i] = matrix->points[j];
// 	matrix->points[j] = tmp;
// }

void invert_matrix(t_matrix *matrix, t_matrix *result, t_matrix *identity, int n) 
{
	float       diag;
	float       factor;
	int			i;
	int			j;

	set_matrix(identity, 0.0f);
	set_diagonal(identity, 1.0f);
	matrix_copy(matrix, result, n);
	i = -1;
	while (++i < n) 
	{
		j = -1;
		diag = result->points[i][i];
		if (diag == 0.0f)
			return ;
		while (++j < n) 
		{
			result->points[i][j] /= diag;
			identity->points[i][j] /= diag;
		}
		for (int k = 0; k < n; k++)
		{
			if (k != i) 
			{
				factor = result->points[k][i];
				for (int j = 0; j < n; j++) 
				{
					result->points[k][j] -= factor * result->points[i][j];
					identity->points[k][j] -= factor * identity->points[i][j];
				}
			}
		}
	}
	matrix_copy(identity, result, n);
}
