#include "minirt.h"

void invert_matrix(t_matrix *matrix, int n) 
{
    // Create an identity matrix
    t_matrix    *identity;
    float       diagValue;
    float       factor;

    identity = malloc(sizeof(t_matrix));
    create_matrix(identity, n, n);
    init_matrix(identity, 0.0f);
    set_diagonal(identity, 1.0f);

    // Apply row operations to transform the matrix into the identity
    for (int i = 0; i < n; i++) 
    {
        diagValue = matrix->points[i][i];
        for (int j = 0; j < n; j++) 
        {
            matrix->points[i][j] /= diagValue;
            identity->points[i][j] /= diagValue;
        }

        for (int k = 0; k < n; k++) 
        {
            if (k != i) 
            {
                factor = matrix->points[k][i];
                for (int j = 0; j < n; j++) 
                {
                    matrix->points[k][j] -= factor * matrix->points[i][j];
                    identity->points[k][j] -= factor * identity->points[i][j];
                }
            }
        }
    }

    // Copy the inverted matrix (identity) back to the original matrix
    matrix_copy(identity, matrix, n);
    // Free memory
    delete_matrix(identity);
}
