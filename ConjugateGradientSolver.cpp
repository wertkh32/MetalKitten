#include "ConjugateGradientSolver.h"
extern void CGSolverGPU(float* A, float* x, float* b, int n);

ConjugateGradientSolver::ConjugateGradientSolver()
{

}

void
ConjugateGradientSolver::initSolver(int _n, float** _A)
{
	n = _n;
	A = _A;
	nb = n/3;

	r = (float*)malloc(sizeof(float) * n); 
	d = (float*)malloc(sizeof(float) * n); 
	q = (float*)malloc(sizeof(float) * n);
	tempo = (float*)malloc(sizeof(float) * n);
}

void ConjugateGradientSolver::removeRows(int r)
{
	for(int i=0;i<n;i++)
		A[r][i] = 0.0;
}


void
ConjugateGradientSolver::solve(float* x, float* b)
{

	float deltaOld, deltaNew, delta0,alpha,beta;
	int it;

	for(int i=0;i<n;i++)
	{
		r[i]=0;
		d[i]=0;
		//x[i]=0;
		q[i]=0;
	}

	for(int i=0; i<n;i++)
	{
		d[i] = r[i] = b[i] - dot(A[i],x,n);
	}

	it=0;
	deltaNew = dot(r,r,n);
	delta0 = deltaNew;
	alpha = beta = 0;

	while(it < MAX_ITER && deltaNew > EPSILON*EPSILON*delta0)
	{
		it++;
		for(int i=0;i<n;i++)
			q[i] = dot(A[i],d,n);

		alpha = deltaNew/dot(d,q,n);

		for(int i=0;i<n;i++)
			x[i] = x[i] + alpha*d[i];

		if(it%50==0)
		{
			//refresh r of its horrible floating point errors
			for(int i=0;i<n;i++)
				r[i] = b[i] - dot(A[i],x,n);
		}
		else
		{
			for(int i=0;i<n;i++)
				r[i] = r[i] - alpha * q[i];
		}

		deltaOld = deltaNew;
		deltaNew = dot(r,r,n);
		beta = deltaNew/deltaOld;

		for(int i=0;i<n;i++)
			d[i] = r[i] + beta * d[i];

	}


}


void ConjugateGradientSolver::solveWithConstraints(float* x, float* b, bool* allowed)
{
	float deltaOld, deltaNew, delta0,alpha,beta;
	int it;

	for(int i=0;i<n;i++)
	{
		r[i]=0;
		d[i]=0;
		//x[i]=0;
		q[i]=0;
	}

	for(int i=0; i<n;i++)
	{
		if(allowed[i])
		{
			float temp=0;
			for(int j=0;j<n;j++)
			{
				if(allowed[j])
					temp += A[i][j] * x[j];
			}
			d[i] = r[i] = b[i] - temp;
		}
	}

	it=0;
	deltaNew = 0;//dot(r,r,n);

	for(int i=0;i<n;i++)
	{
		if(allowed[i])
				deltaNew += r[i] * r[i];
	}

	delta0 = deltaNew;
	alpha = beta = 0;

	while(it < MAX_ITER && deltaNew > EPSILON*EPSILON*delta0)
	{
		it++;
		for(int i=0;i<n;i++)
		{
			if(allowed[i])
			{
				q[i] = 0;
				for(int j=0;j<n;j++)
					if(allowed[j])
						q[i] += A[i][j] * d[j];
			}
		}

		float temp2=0;
		for(int i=0;i<n;i++)
		{
			if(allowed[i])
				temp2 += d[i] * q[i];
		}

		alpha = deltaNew/temp2;//dot(d,q,n);
		
		for(int i=0;i<n;i++)
		{
			if(allowed[i])
				x[i] = x[i] + alpha*d[i];
		}

		if(it%20==0)
		{
			//refresh r of its horrible floating point errors
			for(int i=0;i<n;i++)
			{
				if(allowed[i])
				{
					float temp =0;
					for(int j=0;j<n;j++)
					{
						if(allowed[j])
						{
							temp += A[i][j] * x[j];
						}
					}
					r[i] = b[i] - temp;
				}
			}
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(allowed[i])
					r[i] = r[i] - alpha * q[i];
			}
		}

		deltaOld = deltaNew;
		deltaNew = 0;//dot(r,r,n);

		for(int i=0;i<n;i++)
		{
			if(allowed[i])
				deltaNew += r[i] * r[i];
		}

		beta = deltaNew/deltaOld;

		for(int i=0;i<n;i++)
		{
			if(allowed[i])
				d[i] = r[i] + beta * d[i];
		}

	}

}

ConjugateGradientSolver::~ConjugateGradientSolver(void)
{
	free(r); 
	free(d); 
	free(q); 
	//free(flatA);
}
