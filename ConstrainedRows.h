#pragma once
#include "QuickArray.h"
#define MAX_ROWS_CONSTRAINED 100

enum DOF
{
	X = 0, Y = 1, Z = 2
};

struct ConstrainedRows
{
	int arr[MAX_ROWS_CONSTRAINED];
	QuickArray<int> list;
	ConstrainedRows() :list(arr)
	{
	}
	
	void add(int node, DOF dof)
	{
		list.push(node * 3 + dof);
	}
	
	void add(int node)
	{
		list.push(node * 3);
		list.push(node * 3 + 1);
		list.push(node * 3 + 2);
	}


};