#pragma once
template<class T> class QuickArray
{

int sz;
T* varr;
public:

QuickArray(int n)
{
	varr=new T[n];
	sz=0;
}

QuickArray()
{
	varr=0;
	sz=0;
}

void alloc(int n){
	varr=new T[n];
	sz=0;
}

T& operator[](int i){
return varr[i];
}

void QuickArray<T>::push(T v){
	varr[sz++]=v;
}

inline int size(){
return sz;
}

~QuickArray(void)
{
	delete[] varr;
	delete varr;
}

};

