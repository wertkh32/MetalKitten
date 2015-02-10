#pragma once
template<class T, int N> 
class QuickArray
{

	int sz;
	T varr[N];
public:

	QuickArray()
	{
		//for(int i=0;i<N;i++)
		//	varr[i]=0;
		sz=0;
	}

	inline
		T& operator[](int i){
			return varr[i];
	}

	inline
		void push(T v){
			varr[sz++]=v;
	}

	inline
		T pop()
	{
		if(sz > 0)
			return varr[--sz];
		else
			return 0;
	}

	inline
		void reset(){
			sz=0;
	}

	inline int size(){
		return sz;
	}

	~QuickArray(void)
	{
	}

};

