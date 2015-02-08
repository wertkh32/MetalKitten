#pragma once

template <class T,int N, int M>
class GenMatrix
{
public:
	T mat[N][M];
	GenMatrix(void)
	{
		for(int i=0;i<N;i++)
			for(int j=0;j<M;j++)
				mat[i][j] = 0;
	}

	GenMatrix(T m[N][M])
	{
		for(int i=0;i<N;i++)
			for(int j=0;j<M;j++)
				mat[i][j] = m[i][j];
	}

	GenMatrix(GenMatrix<T,N,M>& m)
	{
		for(int i=0;i<N;i++)
			for(int j=0;j<M;j++)
				mat[i][j] = m(i,j);
	}

	T& operator()(int x, int y){return mat[x][y];}
	
	template<int K>
	GenMatrix<T,N,K> operator*(GenMatrix<T,M,K>& mat2);
	GenMatrix<T,N,M> operator*(T k);

	GenMatrix<T,N,M> operator+(GenMatrix<T,N,M>& mat2);
	void zeroOut(){for(int i=0;i<N;i++)for(int j=0;j<M;j++)mat[i][j]=0;};

	GenMatrix<T,M,N> transpose();
	GenMatrix<T,N,M> inverse();

	void scalarMul(T k)
	{
		for(int i=0;i<N;i++)
			for(int j=0;j<M;j++)
				mat[i][j] *= k;
	}

};

template<class T,int N, int M>
template<int K>
GenMatrix<T,N,K> GenMatrix<T,N,M>::operator*(GenMatrix<T,M,K>& mat2)
{
	GenMatrix<T,N,K> result;
	for(int i = 0; i<N; i++)
		for(int j = 0; j<K; j++)
			for(int k = 0; k<M; k++)
				result(i,j) += mat[i][k] * mat2(k,j);
	return result;
}


template<class T,int N, int M>
GenMatrix<T,N,M> GenMatrix<T,N,M>::operator*(T k)
{
	GenMatrix<T,N,M> result;
	for(int i = 0; i<N; i++)
	{
			for(int j = 0; j<M; j++)
				result(i,j) = mat[i][j] * k;
	}
	return result;
}

template<class T,int N, int M>
GenMatrix<T,N,M> GenMatrix<T,N,M>::operator+(GenMatrix<T,N,M>& mat2)
{
	GenMatrix<T,N,M> result;
	for(int i = 0; i<N; i++)
	{
			for(int j = 0; j<M; j++)
				result(i,j) = mat[i][j] + mat2(i,j);
	}
	return result;
}
\

template<class T,int N, int M>
GenMatrix<T,M,N> GenMatrix<T,N,M>::transpose()
{
	GenMatrix<T,M,N> result;
	for(int i=0; i<N; i++)
		for(int j=0;j<M;j++)
			result(j,i) = mat[i][j];

	return result;
}

template<class T,int N, int M>
GenMatrix<T,N,M> GenMatrix<T,N,M>::inverse()
{
	if(N!=M) return GenMatrix<T,M,N>();
	else
	{
		T result[N][N * 2];

		//build [ A | I ]
		for(int i=0;i<M;i++)
			for(int j=0;j<N;j++)
				result[i][j] = mat[i][j];

		for(int i=0;i<M;i++)
			for(int j=N;j<N * 2;j++)
				if(i==(j-N))
					result[i][j] = 1;
				else
					result[i][j] = 0;

		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				if(i!=j)
				{
					T ratio = result[j][i]/result[i][i];
					for(int k = 0; k < 2*n; k++)
						result[j][k] -= ratio * result[i][k];
				}
			}
		}

		for(int i = 0; i < n; i++)
		{
			T a = result[i][i];
			for(int j = 0; j < 2*n; j++)
				result[i][j] /= a;
		}

		return GenMatrix<T,M,N>(result);
	}
}

