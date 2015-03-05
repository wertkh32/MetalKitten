#pragma once
#include "includes.h"
#include "Geometry.h"

#define CELL_SIZE 3
#define MAX_GEO_PER_CELL 30
#define MAX_GEO 300
#define MAX_OCCUPIED 1000
#define DIM_X 50
#define DIM_Y 50
#define DIM_Z 50
const vector3d origin = vector3d(-DIM_X * CELL_SIZE * 0.5, -DIM_Y * CELL_SIZE * 0.5, -DIM_Z * CELL_SIZE * 0.5);

typedef QuickArray<Geometry*, MAX_GEO_PER_CELL> Cell;

class SpatialHash
{
	QuickArray<Cell*, MAX_OCCUPIED> occupiedList;
	QuickArray<Geometry*, MAX_GEO> geometry;
	Cell hashtable[DIM_X][DIM_Y][DIM_Z];
	AABB _getAABB(Triangle& tri);

	AABB getAABB(Geometry* geo);


	void addGeometryToHashTable(Geometry* geo);
	void resetHashTable();
	void buildHashTable();

public:

	void addGeometry(Geometry* geo){ geometry.push(geo); }
	void update();
	void render();

	QuickArray<Cell*, MAX_OCCUPIED>& getOccupiedCells() { return occupiedList; }
	Cell& getCell(vector3d& pt);


	SpatialHash(void);
	~SpatialHash(void);
};

