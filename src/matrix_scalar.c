#include "matrix.h"

int	matsum(t_matrix *mat1, t_matrix *mat2, t_matrix *result)
{
	int	i;
	int	j;

	i = -1;
	while (++i < mat1->rows)
	{
		j = -1;
		while (++j < mat1->cols)
			result->points[i][j] = mat1->points[i][j] + mat2->points[i][j];
	}
	return (1);
}

int	matdiff(t_matrix *mat1, t_matrix *mat2, t_matrix *result)
{
	int	i;
	int	j;

	i = -1;
	while (++i < mat1->rows)
	{
		j = -1;
		while (++j < mat1->cols)
			result->points[i][j] = mat1->points[i][j] - mat2->points[i][j];
	}
	return (1);
}
