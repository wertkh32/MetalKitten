#include "GPUDataStructs.cuh"

void gpuUploadVars(TetData* gpuElements, NodeData* gpuNodes,float* x, float* x0,
			  float* vt, float* extforces, float* mass, char* allowed, int numnodes, int numelements);

void gpuUploadExtForces(float* extforces, int numnodes);

void gpuDownloadVars(float* x, int numnodes);

void gpuInitVars(int numele, int numnodes);

void GPUTimestep(int numtets, int numnodes, int max_entry);

void gpuDestroyVars();