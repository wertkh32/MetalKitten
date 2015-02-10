#pragma once
#include "QuickArray.h"
#define MAX_ROWS_CONSTRAINED 100

enum DOF
{
	X = 0, Y = 1, Z = 2
};

struct ConstrainedRows
{
	QuickArray<int, MAX_ROWS_CONSTRAINED> list;
	ConstrainedRows()
	{
	}
	
	void add(int row)
	{
		list.push(row);
	}

};