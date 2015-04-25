#include "SparseMatrix.h"


SparseMatrix::SparseMatrix(float** A, int numnodes)
{
	n = numnodes;
	numentries = 0;
	for (int i = 0; i < numnodes;i++)
		for (int j = 0; j < numnodes; j++)
		{
			if (fabs(A[i][j]) > 1e-9)
			{
				numentries++;
			}
		}

	//printf("[%d]", numentries);
	entries = (float*)calloc(numentries, sizeof(float));
	indices = (int*)calloc(numentries, sizeof(int));

	int counter = 0;
	for (int i = 0; i < numnodes;i++)
		for (int j = 0; j < numnodes; j++)
		{
			if (fabs(A[i][j]) > 1e-9)
			{
				entries[counter] = A[i][j];
				indices[counter] = i * numnodes + j;

				counter++;
			}
		}

		//printf("[%d] [%d]", numentries, counter);
}

void
SparseMatrix::mulXCompressed3x3(float* in, float* out)
{
	for (int i = 0; i < n * 3; i++)
		out[i] = 0;

	for (int i = 0; i < numentries; i++)
	{
		int ind = indices[i];
		int x = ind / n;
		int y = ind % n;
		float entry = entries[i];

		//printf("[%d] [%d] [%d] [%d]\n", indices[i],n, x, y);
		out[x * 3] += entry * in[y * 3];
		out[x * 3 + 1] += entry * in[y * 3 + 1];
		out[x * 3 + 2] += entry * in[y * 3 + 2];
	}

}

SparseMatrix::~SparseMatrix()
{
}
