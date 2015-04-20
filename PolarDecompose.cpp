#include "PolarDecompose.h"


float PolarDecompose::oneNorm(Matrix3d& F)
{
  float norm = 0.0;
  for (int i=0; i<3; i++) 
  {
    float columnAbsSum = fabs(F(0,i)) + fabs(F(1,i)) + fabs(F(2,i));
    if (columnAbsSum > norm) 
      norm = columnAbsSum;
  }
  return norm;
}

float PolarDecompose::infNorm(Matrix3d& F)
{
  float norm = 0.0;
  for (int i=0; i<3; i++) 
  {
    float rowSum = fabs(F(i,0)) + fabs(F(i,1)) + fabs(F(i,2));
    if (rowSum > norm) 
      norm = rowSum;
  }
  return norm;
}

void PolarDecompose::compute(Matrix3d& F, Matrix3d& R, Matrix3d& S)
{
  Matrix3d Mk;
  Matrix3d Ek;
  float det, M_oneNorm, M_infNorm, E_oneNorm;

  Mk = F.transpose();

  M_oneNorm = oneNorm(Mk); 
  M_infNorm = infNorm(Mk);

  do 
  {
   
	vector3d r1 = vector3d(Mk(1,0), Mk(1,1), Mk(1,2)).cross(vector3d(Mk(2,0), Mk(2,1), Mk(2,2))); 
	vector3d r2 = vector3d(Mk(2,0), Mk(2,1), Mk(2,2)).cross(vector3d(Mk(0,0), Mk(0,1), Mk(0,2))); 
	vector3d r3 = vector3d(Mk(0,0), Mk(0,1), Mk(0,2)).cross(vector3d(Mk(1,0), Mk(1,1), Mk(1,2))); 
   Matrix3d MadjTk(r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					r3.x,r3.y,r3.z);


    det = Mk(0,0) * MadjTk(0,0) + Mk(0,1) * MadjTk(0,1) + Mk(0,2) * MadjTk(0,2);
    if (det == 0.0) 
    {
      printf("Warning (polarDecomposition) : zero determinant encountered.\n");
      break;
    }

    float MadjT_one = oneNorm(MadjTk); 
    float MadjT_inf = infNorm(MadjTk);

    float gamma = sqrt(sqrt((MadjT_one * MadjT_inf) / (M_oneNorm * M_infNorm)) / fabs(det));
    float g1 = gamma * 0.5;
    float g2 = 0.5 / (gamma * det);

   for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
    {
      Ek(i,j) = Mk(i,j);
      Mk(i,j) = g1 * Mk(i,j) + g2 * MadjTk(i,j);
      Ek(i,j) -= Mk(i,j);
    }

    E_oneNorm = oneNorm(Ek);
    M_oneNorm = oneNorm(Mk);  
    M_infNorm = infNorm(Mk);
  }
  while ( E_oneNorm > M_oneNorm * TOLERANCE);

  // Q = Mk^T 

  R = Mk.transpose();
  
  //S not needed for simulation.
  /*
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
    {
      S(i,j) = 0.0;
      for(int k=0; k<3; k++)
        S(i,j) += Mk(i,k) * F(k,j);
    }
    
  // S must be symmetric; enforce the symmetry
  for (int i=0; i<3; i++) 
    for (int j=i; j<3; j++)
      S(i,j) = S(j,i) = 0.5 * (S(i,j) + S(j,i));
	*/
  //return (det);
}

void PolarDecompose::computeFull(Matrix3d& F, Matrix3d& R, Matrix3d& S)
{
  Matrix3d Mk;
  Matrix3d Ek;
  float det, M_oneNorm, M_infNorm, E_oneNorm;

  Mk = F.transpose();

  M_oneNorm = oneNorm(Mk); 
  M_infNorm = infNorm(Mk);

  do 
  {
   
	vector3d r1 = vector3d(Mk(1,0), Mk(1,1), Mk(1,2)).cross(vector3d(Mk(2,0), Mk(2,1), Mk(2,2))); 
	vector3d r2 = vector3d(Mk(2,0), Mk(2,1), Mk(2,2)).cross(vector3d(Mk(0,0), Mk(0,1), Mk(0,2))); 
	vector3d r3 = vector3d(Mk(0,0), Mk(0,1), Mk(0,2)).cross(vector3d(Mk(1,0), Mk(1,1), Mk(1,2))); 
   Matrix3d MadjTk(r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					r3.x,r3.y,r3.z);


    det = Mk(0,0) * MadjTk(0,0) + Mk(0,1) * MadjTk(0,1) + Mk(0,2) * MadjTk(0,2);
    if (det == 0.0) 
    {
      printf("Warning (polarDecomposition) : zero determinant encountered.\n");
      break;
    }

    float MadjT_one = oneNorm(MadjTk); 
    float MadjT_inf = infNorm(MadjTk);

    float gamma = sqrt(sqrt((MadjT_one * MadjT_inf) / (M_oneNorm * M_infNorm)) / fabs(det));
    float g1 = gamma * 0.5;
    float g2 = 0.5 / (gamma * det);

   for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
    {
      Ek(i,j) = Mk(i,j);
      Mk(i,j) = g1 * Mk(i,j) + g2 * MadjTk(i,j);
      Ek(i,j) -= Mk(i,j);
    }

    E_oneNorm = oneNorm(Ek);
    M_oneNorm = oneNorm(Mk);  
    M_infNorm = infNorm(Mk);
  }
  while ( E_oneNorm > M_oneNorm * TOLERANCE);

  // Q = Mk^T 

  R = Mk.transpose();
  
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
    {
      S(i,j) = 0.0;
      for(int k=0; k<3; k++)
        S(i,j) += Mk(i,k) * F(k,j);
    }
    
  // S must be symmetric; enforce the symmetry
  for (int i=0; i<3; i++) 
    for (int j=i; j<3; j++)
      S(i,j) = S(j,i) = 0.5 * (S(i,j) + S(j,i));

  //return (det);
}
