#include "MatrixOps.h"


MatrixOps::MatrixOps()
{
}

void
MatrixOps::CholeskyFac(float** in_A, float** out_LT, int dim)
{
	float** temp_A = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		temp_A[i] = (float*)calloc(dim, sizeof(float));

	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			out_LT[i][j] = 0;

	for (int i = 0; i < dim;i++)
		for (int j = 0; j < dim; j++)
			temp_A[i][j] = in_A[i][j];

	for (int i = 0; i < dim; i++)
	{
		float Lii = sqrt(temp_A[i][i]);

		out_LT[i][i] = Lii;

		for (int j = i + 1; j < dim; j++)
			out_LT[i][j] = temp_A[i][j] / Lii;
		
		for (int j = i + 1; j < dim; j++)
			for (int k = i + 1; k < dim; k++)
				temp_A[j][k] -= out_LT[i][j] * out_LT[i][k];
	}


	for (int i = 0; i < dim; i++)
		free(temp_A[i]);
	free(temp_A);

}


void
MatrixOps::InverseMatrix(float** __restrict in_A, float** __restrict out_Ainv, int dim)
{
	float** result = (float**)calloc(dim, sizeof(float*));
	for (int i = 0; i < dim; i++)
		result[i] = (float*)calloc(dim * 2, sizeof(float));

	//build [ A | I ]
	for (int i = 0; i<dim; i++)
	for (int j = 0; j<dim; j++)
		result[i][j] = in_A[i][j];

	for (int i = 0; i<dim; i++)
		for (int j = dim; j<dim * 2; j++)
			if (i == (j - dim))
				result[i][j] = 1;
			else
				result[i][j] = 0;

	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (i != j)
			{
				float ratio = result[j][i] / result[i][i];
				for (int k = 0; k < 2 * dim; k++)
					result[j][k] -= ratio * result[i][k];
			}
		}
	}

	for (int i = 0; i < dim; i++)
	{
		float a = result[i][i];
		for (int j = 0; j < 2 * dim; j++)
			result[i][j] /= a;
	}

	for (int i = 0; i<dim; i++)
		for (int j = dim; j < dim * 2; j++)
			out_Ainv[i][j - dim] = result[i][j];


	for (int i = 0; i < dim; i++)
		free(result[i]);

	free(result);
}

MatrixOps::~MatrixOps()
{
}
