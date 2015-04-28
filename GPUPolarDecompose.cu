#pragma once
#define TOLERANCE 0.01

__device__ 
float oneNorm(const float A[3][3])
{
  float norm = 0.0;
  #pragma unroll 3
  for (int i=0; i<3; i++) 
  {
    float columnAbsSum = fabsf(A[0][i]) + fabsf(A[1][i]) + fabsf(A[2][i]);
    if (columnAbsSum > norm) 
      norm = columnAbsSum;
  }
  return norm;
}

__device__
float infNorm(const float A[3][3])
{
  float norm = 0.0;
  #pragma unroll 3
  for (int i=0; i<3; i++) 
  {
    float rowSum = fabsf(A[i][0]) + fabsf(A[i][1]) + fabsf(A[i][2]);
    if (rowSum > norm) 
      norm = rowSum;
  }
  return norm;
}

// cross product: c = a x b
__device__
void crossProduct(const float* a, const float* b, float* c)
{
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
}

// Input: M (3x3 mtx)
// Output: Q (3x3 rotation mtx), S (3x3 symmetric mtx)
__device__
void gpuComputePolarDecomposition(float Mk[3][3])
{
  //float Mk[3][3];
  float Ek[3][3];
  float det, M_oneNorm, M_infNorm, E_oneNorm;

  // Mk = M^T
  //for(int i=0; i<3; i++)
  //  for(int j=0; j<3; j++)
  //    Mk[i][j] = M[j][i];

  M_oneNorm = oneNorm(Mk); 
  M_infNorm = infNorm(Mk);

  do 
  {
    float MadjTk[3][3];
 
    // row 2 x row 3
    crossProduct(Mk[1], Mk[2], MadjTk[0]); 
    // row 3 x row 1
    crossProduct(Mk[2], Mk[0], MadjTk[1]);
    // row 1 x row 2
    crossProduct(Mk[0], Mk[1], MadjTk[2]);

    det = Mk[0][0] * MadjTk[0][0] + Mk[0][1] * MadjTk[0][1] + Mk[0][2] * MadjTk[0][2];
    if (det == 0.0) 
    {
      //printf("Warning (polarDecomposition) : zero determinant encountered.\n");
      break;
    }

    float MadjT_one = oneNorm(MadjTk); 
    float MadjT_inf = infNorm(MadjTk);

    float gamma = sqrtf(sqrtf((MadjT_one * MadjT_inf) / (M_oneNorm * M_infNorm)) / fabsf(det));
    float g1 = gamma * 0.5;
    float g2 = 0.5 / (gamma * det);

	#pragma unroll 3
    for(int i=0; i<3; i++)
		#pragma unroll 3
		for(int j=0; j<3; j++)
		{
			Ek[i][j] = Mk[i][j];
			Mk[i][j] = g1 * Mk[i][j] + g2 * MadjTk[i][j];
			Ek[i][j] -= Mk[i][j];
		}

    E_oneNorm = oneNorm(Ek);
    M_oneNorm = oneNorm(Mk);  
    M_infNorm = infNorm(Mk);
  }
  while ( E_oneNorm > M_oneNorm * TOLERANCE );

  // Q = Mk^T 
  //for(int i=0; i<3; i++)
  //  for(int j=0; j<3; j++)
  //    Q[i][j] = Mk[j][i];

  //return (det);
}
