#include "minirt.h"

void invert_matrix(t_matrix *matrix, t_matrix *result, t_matrix *identity, int n) 
{
	float       diagValue;
	float       factor;

	//set_diagonal(identity, 1.0f);
	for (int i = 0; i < n; i++) 
	{
		diagValue = matrix->points[i][i];
		for (int j = 0; j < n; j++) 
		{
			if (j != i)
				identity->points[i][j] = 0.0f;
			else
				identity->points[i][j] = 1.0f;
			result->points[i][j] = matrix->points[i][j];
			result->points[i][j] /= diagValue;
			identity->points[i][j] /= diagValue;
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
