#include "SpatialHash.h"


SpatialHash::SpatialHash(void)
{
}

AABB
SpatialHash::getAABB(Geometry* geo)
{
	if (geo->type == _TRIANGLE)
		return _getAABB(*((Triangle*)geo));

	return AABB();
}

AABB
SpatialHash::_getAABB(Triangle& tri)
{
	AABB aabb;
	aabb.min = vector3d(MIN(MIN(tri.p0.x, tri.p1.x), tri.p2.x),
						MIN(MIN(tri.p0.y, tri.p1.y), tri.p2.y),
						MIN(MIN(tri.p0.z, tri.p1.z), tri.p2.z));

	aabb.max = vector3d(MAX(MAX(tri.p0.x, tri.p1.x), tri.p2.x),
						MAX(MAX(tri.p0.y, tri.p1.y), tri.p2.y),
						MAX(MAX(tri.p0.z, tri.p1.z), tri.p2.z));

	return aabb;
}

Cell&
SpatialHash::getCell(vector3d& pt)
{
	int x = (pt.x - origin.x) / CELL_SIZE;
	int y = (pt.y - origin.y) / CELL_SIZE;
	int z = (pt.z - origin.z) / CELL_SIZE;

	//printf("%d %d %d\n", x, y, z);

	if (x > DIM_X || y > DIM_Y || z > DIM_Z || x < 0 || y < 0 || z < 0) printf("Screwed");

	return hashtable[x][y][z];
}

void
SpatialHash::addGeometryToHashTable(Geometry* geo)
{
	AABB aabb = getAABB(geo);

	vector3d& min = aabb.min;
	vector3d& max = aabb.max;
	for (float i = min.x; i < max.x; i += CELL_SIZE)
	{
		for (float j = min.y; j < max.y; j += CELL_SIZE)
		{
			for (float k = min.z; k < max.z; k += CELL_SIZE)
			{
				Cell& cell = getCell(vector3d(i, j, k));


				/*bool isadded = false;
				for (int l = 0; l < cell.size();l++)
				if (cell[l] == geo)
				{
					isadded = true;
					break;
				}

				if (isadded)continue;*/
				
				cell.push(geo);
				
				if (cell.size() > 1)
					occupiedList.push(&cell);
			}
		}
	}
}

void
SpatialHash::resetHashTable()
{
	for (int i = 0; i < DIM_X;i++)
	for (int j = 0; j < DIM_Y;j++)
	for (int k = 0; k < DIM_Z; k++)
		hashtable[i][j][k].reset();
	occupiedList.reset();
}

void SpatialHash::buildHashTable()
{
	resetHashTable();
	for (int i = 0; i < geometry.size(); i++)
		addGeometryToHashTable(geometry[i]);
}

void SpatialHash::update()
{
	buildHashTable();
}


void SpatialHash::render()
{
	/*for (int i = 0; i < DIM_X-1; i++)
	for (int j = 0; j < DIM_Y-1; j++)
	for (int k = 0; k < DIM_Z-1; k++)
	{
		vector3d v000 = vector3d(i*CELL_SIZE, j*CELL_SIZE, k*CELL_SIZE) + origin;
		vector3d v100 = vector3d((i + 1)*CELL_SIZE, j*CELL_SIZE, k*CELL_SIZE) + origin;
		vector3d v101 = vector3d((i + 1)*CELL_SIZE, j*CELL_SIZE, (k + 1)*CELL_SIZE) + origin;
		vector3d v001 = vector3d(i*CELL_SIZE, j*CELL_SIZE, (k + 1)*CELL_SIZE) + origin;
		
		vector3d v010 = vector3d(i*CELL_SIZE, (j + 1)*CELL_SIZE, k*CELL_SIZE) + origin;
		vector3d v110 = vector3d((i + 1)*CELL_SIZE, (j + 1)*CELL_SIZE, k*CELL_SIZE) + origin;
		vector3d v111 = vector3d((i + 1)*CELL_SIZE, (j + 1)*CELL_SIZE, (k + 1)*CELL_SIZE) + origin;
		vector3d v011 = vector3d(i*CELL_SIZE, (j + 1)*CELL_SIZE, (k + 1)*CELL_SIZE) + origin;

	}*/

	//vector3d v000 = origin;
	//vector3d v001 = vector3d(0, 0, DIM_Z * CELL_SIZE) + origin;
	
}

SpatialHash::~SpatialHash(void)
{
}
