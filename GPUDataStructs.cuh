#pragma once

#define NODE_BLOCK_SIZE 256
#define TET_BLOCK_SIZE 64
#define DOT_BLOCK_SIZE 512	

#define VECTOR_BLOCK_SIZE 128

#define MAX_NODE_ENTRIES 64
#define MAX_TETS 32

#define TET_INDEX 0 
#define NODE_INDEX 1

struct NodeData
{
	float nodeEntries[MAX_NODE_ENTRIES][NODE_BLOCK_SIZE];
	int nodeEntryIndex[MAX_NODE_ENTRIES][NODE_BLOCK_SIZE];
	int indices[MAX_TETS][2][NODE_BLOCK_SIZE];
	int ntets[NODE_BLOCK_SIZE];
};

struct TetData
{
	float Dm[3][3][TET_BLOCK_SIZE];
	float Bm[3][3][TET_BLOCK_SIZE];
	float index[4][TET_BLOCK_SIZE];
	float weight[TET_BLOCK_SIZE];
	float P[4][3][TET_BLOCK_SIZE];
};

struct CGVars
{
	float delta0, deltaNew, deltaOld, alpha, beta;
};
