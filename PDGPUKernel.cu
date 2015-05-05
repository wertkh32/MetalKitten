#pragma once
#include <cuda.h>
#include <stdio.h>
#include "defines.h"
#include "PDGPUKernel.cuh"
#include "GPUPolarDecompose.cu"

#define DAMPING 0.977
#define PD_ITERATIONS 5
#define CG_ITERATIONS 5

float* debugbuf;

static void HandleError( cudaError_t err,
                         const char *file,
                         int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
                file, line );
		system("pause");
        exit( EXIT_FAILURE );
    }
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

TetData* gpuptr_TetData;
NodeData*   gpuptr_NodeData;
float*   gpuptr_xt;//dynamic
float*   gpuptr_vt;//dynamic
float*	 gpuptr_fext;//dynamic
float*	 gpuptr_b;//dynamic
float*	 gpuptr_sn;
float*	 gpuptr_x;
float*	 gpuptr_x0;
float*	 gpuptr_mass;
char*	 gpuptr_allowed;

//for CG
float* gpuptr_CG_R;
float* gpuptr_CG_D;
float* gpuptr_CG_Q;
CGVars* gpuptr_CG_Vars;

//for debug
float* gpuptr_debug;


__host__
void checkCudaErrors(const char* comment)
{
	cudaDeviceSynchronize();
	cudaError_t error = cudaGetLastError();
	if(error != cudaSuccess)
	{
		// print the CUDA error message and exit
		printf("\n%s\nCUDA error: %s\n", comment, cudaGetErrorString(error));
		//exit(-1);
		system("pause");
	}
}

__host__
void
gpuInitVars(int numele, int numnodes)
{
	int numblocksperele = (numele / TET_BLOCK_SIZE) + 1;
	int numblockpernode = (numnodes / NODE_BLOCK_SIZE) + 1;

	cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);

	HANDLE_ERROR( cudaMalloc(&gpuptr_TetData, numblocksperele * sizeof(TetData)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_NodeData, numblockpernode * sizeof(NodeData)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_xt, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_vt, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_fext, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_b, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_sn, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_x, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_x0, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_mass, numnodes * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_allowed, numnodes * sizeof(char)) );

	HANDLE_ERROR( cudaMalloc(&gpuptr_CG_R, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_CG_D, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_CG_Q, numnodes * 3 * sizeof(float)) );
	HANDLE_ERROR( cudaMalloc(&gpuptr_CG_Vars, sizeof(CGVars)) );

	debugbuf = (float*)malloc(numnodes * 3 * sizeof(float));
	HANDLE_ERROR( cudaMalloc(&gpuptr_debug, numnodes * 3 * sizeof(float)) );

	checkCudaErrors("Allocation");
}

__host__
void
gpuUploadVars(TetData* gpuElements, NodeData* gpuNodes,float* x, float* x0, 
			  float* vt, float* extforces, float* mass, char* allowed, int numnodes, int numelements)
{
	int numblocksperele = (numelements / TET_BLOCK_SIZE) + 1;
	int numblockpernode = (numnodes / NODE_BLOCK_SIZE) + 1;

	HANDLE_ERROR( cudaMemcpy(gpuptr_TetData, gpuElements, numblocksperele * sizeof(TetData), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_NodeData, gpuNodes, numblockpernode * sizeof(NodeData), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_xt, x, numnodes * 3 * sizeof(float), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_x, x, numnodes * 3 * sizeof(float), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_x0, x0, numnodes * 3 * sizeof(float), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_vt, vt, numnodes * 3 * sizeof(float), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_fext, extforces, numnodes * 3 * sizeof(float), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_allowed, allowed, numnodes * sizeof(char), cudaMemcpyHostToDevice) );
	HANDLE_ERROR( cudaMemcpy(gpuptr_mass, mass, numnodes * sizeof(float), cudaMemcpyHostToDevice) );

	checkCudaErrors("Upload vars");
}

__host__
void
gpuDownloadVars(float* x, int numnodes)
{
	cudaMemcpy(x, gpuptr_x, numnodes * 3 * sizeof(float), cudaMemcpyDeviceToHost);
}

__host__
void inspectGPUBuffer(float* gpubuf,int numnodes)
{
	cudaMemcpy(debugbuf, gpubuf, numnodes * 3 * sizeof(float), cudaMemcpyDeviceToHost);

	for(int i=0;i<numnodes * 3;i++)
		printf("%f\n",debugbuf[i]);
	printf("\n");
	//system("pause");
}

__host__
void
gpuUploadExtForces(float* extforces, int numnodes)
{
	cudaMemcpy(gpuptr_fext, extforces, numnodes * 3 * sizeof(float), cudaMemcpyHostToDevice);

	checkCudaErrors("Upload Ext force");
}


__host__
void
gpuDestroyVars()
{
	cudaFree(gpuptr_TetData);
	cudaFree(gpuptr_NodeData);
	cudaFree(gpuptr_xt);
	cudaFree(gpuptr_vt);
	cudaFree(gpuptr_fext);
	cudaFree(gpuptr_sn);
	cudaFree(gpuptr_x);
	cudaFree(gpuptr_x0);
	cudaFree(gpuptr_mass);
	cudaFree(gpuptr_b);
	cudaFree(gpuptr_CG_R);
	cudaFree(gpuptr_CG_D);
	cudaFree(gpuptr_CG_Q);
	cudaFree(gpuptr_CG_Vars);
	cudaFree(gpuptr_debug);
}


//ref function
__device__
void readWarpCoalescedMemory(WarpData* data, int node, float out[16])
{
	int warpthreadid = threadIdx.x % HWARP_SIZE;
	int warpdata_tid = (node % (TET_NUM_HWARPS * TET_PER_HWARP));
	int warpdata_bid = (node / (TET_NUM_HWARPS * TET_PER_HWARP));
	int warpdata_wid = warpdata_tid % TET_NUM_HWARPS;
	int warpdata_lid = warpdata_tid / TET_NUM_HWARPS;

	out[warpthreadid] = data[warpdata_bid].P[warpdata_lid][warpdata_wid][warpthreadid];
}

//ref function
__device__
void writeWarpCoalescedMemory(WarpData* data, int node, float out[16])
{
	int warpthreadid = threadIdx.x % HWARP_SIZE;
	int warpdata_tid = (node % (TET_NUM_HWARPS * TET_PER_HWARP));
	int warpdata_bid = (node / (TET_NUM_HWARPS * TET_PER_HWARP));
	int warpdata_wid = warpdata_tid % TET_NUM_HWARPS;
	int warpdata_lid = warpdata_tid / TET_NUM_HWARPS;

	data[warpdata_bid].P[warpdata_lid][warpdata_wid][warpthreadid] = out[warpthreadid];
}


//ref function
// (numnodes / NODE_BLOCK_SIZE) + 1 blocks
__global__
void PDCompressed3x3MatrixMultiply(NodeData* nodedata, float* in, float* out, int max_entry, int numnodes)
{
	
	int nodeno = blockIdx.x * NODE_BLOCK_SIZE + threadIdx.x;
	
	float x[3], o[3] = {0};

	if(nodeno < numnodes)
	{
		NodeData* node = &(nodedata[blockIdx.x]);

		x[0] = in[nodeno];
		x[1] = in[nodeno + numnodes];
		x[2] = in[nodeno + numnodes * 2];

		for(int i=0;i<max_entry;i++)
		{
			float entry = node->nodeEntries[i][threadIdx.x];
			o[0] += entry * x[0];
			o[1] += entry * x[1];
			o[2] += entry * x[2];
		}

		out[nodeno] = o[0];
		out[nodeno + numnodes] = o[1];
		out[nodeno + numnodes * 2] = o[2];

	}
}

//1 block, DOT_BLOCK_SIZE
__device__
void dot(const float* a, const float* b, float*  out, int n) 
{
	__shared__ float temp[DOT_BLOCK_SIZE];
	int index = threadIdx.x;
	int element = index;

	float tmp = 0;

	while(element < n)
	{
		tmp += a[element] * b[element];
		element += DOT_BLOCK_SIZE;
	}

	temp[index] = tmp;

	__syncthreads();


	int i = DOT_BLOCK_SIZE >> 1;
	while(i>32)
	{
		if(index < i)
			temp[index] += temp[index + i];
		__syncthreads();
		i>>=1;
	}

	while(i>0)
	{
		if(index < i)
			temp[index] += temp[index + i];
		i>>=1;
	}


	if(index == 0)
		*out = temp[0];
}

__global__
void
MakeSnAndV(float* sn, float* x, float* xt, float* vt, float* fext, float* nodemass, int numnodes)
{
	int nodeno = threadIdx.x + blockIdx.x * VECTOR_BLOCK_SIZE;
	if(nodeno < numnodes)
	{
	
		float _xt[3];
		float _vt[3];
		float _x[3];

		int mass = nodemass[nodeno];

		_x[0] = x[nodeno];
		_x[1] = x[nodeno + numnodes];
		_x[2] = x[nodeno + numnodes * 2];
		
		_xt[0] = xt[nodeno];
		_xt[1] = xt[nodeno + numnodes];
		_xt[2] = xt[nodeno + numnodes * 2];
		
		_vt[0] = (_x[0] - _xt[0])/DT;
		_vt[1] = (_x[1] - _xt[1])/DT;
		_vt[2] = (_x[2] - _xt[2])/DT; 

		sn[nodeno] =				 (_x[0] + DT * _vt[0] * DAMPING + DT * DT * fext[nodeno] / mass) * (mass/(DT * DT));
		sn[nodeno + numnodes] =		 (_x[1] + DT * _vt[1] * DAMPING + DT * DT * fext[nodeno + numnodes] / mass) * (mass/(DT * DT));
		sn[nodeno + numnodes * 2] =  (_x[2] + DT * _vt[2] * DAMPING + DT * DT * fext[nodeno + numnodes * 2] / mass) * (mass/(DT * DT));

		xt[nodeno] =				_x[0];
		xt[nodeno + numnodes] =		_x[1];
		xt[nodeno + numnodes * 2] = _x[2];

		vt[nodeno] =				_vt[0];
		vt[nodeno + numnodes] =		_vt[1];
		vt[nodeno + numnodes * 2] = _vt[2];

	}
}



// (numtets / TET_BLOCK_SIZE) + 1 blocks
__global__
void ProjectTransforms(TetData* tets, float* x, int numtets, int numnodes)
{
	int tid = threadIdx.x + blockIdx.x *TET_BLOCK_SIZE;
	int bid = blockIdx.x;
	int ltid = threadIdx.x;

	if(tid < numtets)
	{
		TetData* t_ele = &(tets[bid]);
		
		float nodes[12], R[3][3]={0}, D[3][3]={0};

		#pragma unroll 4
		for(int i=0;i<4;i++)
		{
			int index = t_ele->index[i][ltid];
			nodes[i * 3] = x[index];
			nodes[i * 3 + 1] = x[index + numnodes];
			nodes[i * 3 + 2] = x[index + numnodes * 2];
		}
		
		#pragma unroll 3
		for(int i=0;i<3;i++)
			#pragma unroll 3
			for(int j=0;j<3;j++)
				D[i][j] = t_ele->Bm[i][j][ltid];

		#pragma unroll 3
		for(int i=0;i<3;i++)
			#pragma unroll 3
			for(int j=0;j<3;j++)
					R[i][j] = (nodes[i] - nodes[9 + i]) * D[0][j] + 
							  (nodes[3 + i] - nodes[9 + i]) * D[1][j] + 
							  (nodes[6 + i] - nodes[9 + i]) * D[2][j];

		gpuComputePolarDecomposition(R);


		float det = R[0][0] * (R[1][1] * R[2][2] - R[1][2] * R[2][1]); 
		det -= R[0][1] * (R[2][2] * R[1][0] - R[1][2] * R[2][0]); 
		det += R[0][2] * (R[1][0] * R[2][1] - R[1][1] * R[2][0]);

		int neg = ((det < 0) * -1) | 1;

		#pragma unroll 3
		for(int i=0;i<3;i++)
			#pragma unroll 3
			for(int j=0;j<3;j++)
					R[i][j] = neg * R[i][j];

		#pragma unroll 3
		for(int i=0;i<3;i++)
			#pragma unroll 3
			for(int j=0;j<3;j++)
				D[i][j] = t_ele->Dm[i][j][ltid];

		float w = t_ele->weight[ltid];

		#pragma unroll 3
		for(int i=0;i<3;i++)
		{
			#pragma unroll 3
			for(int j=0;j<3;j++)
			{
				float temp = 0;

				#pragma unroll 3	
				for(int k=0;k<3;k++)
				{
					temp += R[i][k] * D[k][j];
				}

				nodes[j * 3 + i] = temp * w;
			}
		}

		nodes[9] =  -nodes[0]-nodes[3]-nodes[6];
		nodes[10] = -nodes[1]-nodes[4]-nodes[7];
		nodes[11] = -nodes[2]-nodes[5]-nodes[8];

		t_ele->P[0][0][ltid] = nodes[0];
		t_ele->P[0][1][ltid] = nodes[1];
		t_ele->P[0][2][ltid] = nodes[2];
		t_ele->P[1][0][ltid] = nodes[3];
		t_ele->P[1][1][ltid] = nodes[4];
		t_ele->P[1][2][ltid] = nodes[5];
		t_ele->P[2][0][ltid] = nodes[6];
		t_ele->P[2][1][ltid] = nodes[7];
		t_ele->P[2][2][ltid] = nodes[8];
		t_ele->P[3][0][ltid] = nodes[9];
		t_ele->P[3][1][ltid] = nodes[10];
		t_ele->P[3][2][ltid] = nodes[11];
	}
}

__global__
void makeBandRandD(TetData* tetdata, NodeData* nodedata, float* sn, float* b, int numnodes)
{
	int nodeno = blockIdx.x * NODE_BLOCK_SIZE + threadIdx.x;
	int ltid = threadIdx.x;
	int bid = blockIdx.x;

	if(nodeno < numnodes)
	{
		NodeData* nd = &(nodedata[bid]);
		
		float o[3] = {0};

		int n = nd->ntets[ltid];

		o[0] = sn[nodeno];
		o[1] = sn[nodeno + numnodes];
		o[2] = sn[nodeno + numnodes * 2];

		for(int i=0;i<n;i++)
		{
			int tetindex = nd->indices[i][TET_INDEX][ltid];
			int tetblockindex = tetindex / TET_BLOCK_SIZE;
			int tetthreadindex = tetindex % TET_BLOCK_SIZE;
			int nodeindex = nd->indices[i][NODE_INDEX][ltid];
			
			o[0] +=  tetdata[tetblockindex].P[nodeindex][0][tetthreadindex];
			o[1] +=  tetdata[tetblockindex].P[nodeindex][1][tetthreadindex];
			o[2] +=  tetdata[tetblockindex].P[nodeindex][2][tetthreadindex];

		}

		// now: o = b
		b[nodeno] = o[0];
		b[nodeno + numnodes] = o[1];
		b[nodeno + numnodes * 2] = o[2];
		
	}
}

__global__
void makeRandD(NodeData* nodedata, float* b, float* x, float* r, float* d, int max_entry, int numnodes)
{
	int nodeno = blockIdx.x * NODE_BLOCK_SIZE + threadIdx.x;
	int ltid = threadIdx.x;
	int bid = blockIdx.x;

	

	if(nodeno < numnodes)
	{
		NodeData* nd = &(nodedata[bid]);
		float o[3];

		o[0] = b[nodeno];
		o[1] = b[nodeno + numnodes];
		o[2] = b[nodeno + numnodes * 2];

		//o = r = d = b - Ax
		for(int i=0;i<max_entry;i++)
		{
			float entry = nd->nodeEntries[i][threadIdx.x];
			int index = nd->nodeEntryIndex[i][threadIdx.x];

			o[0] -= entry * x[index];
			o[1] -= entry * x[index + numnodes];
			o[2] -= entry *  x[index + numnodes * 2];
		}

		r[nodeno] = o[0];
		r[nodeno + numnodes] = o[1];
		r[nodeno + numnodes * 2] = o[2];

		d[nodeno] = o[0];
		d[nodeno + numnodes] = o[1];
		d[nodeno + numnodes * 2] = o[2];
	}
}

//init
//1 block, DOT_BLOCK_SIZE threads
__global__
void
initDeltaVars(CGVars* vars, float* r, int numnodes)
{
	__shared__ float rr;
	dot(r, r, &rr, numnodes * 3);
	
	if(threadIdx.x == 0)
	{
		vars->deltaNew = rr;
		vars->delta0 = vars->deltaNew;
	}
}

//LOOP
//make q
__global__
void MakeQ(NodeData* nodedata, float* d, float* q, int max_entry, int numnodes)
{
	
	int nodeno = blockIdx.x * NODE_BLOCK_SIZE + threadIdx.x;
	
	float _d[3], o[3] = {0};

	if(nodeno < numnodes)
	{
		NodeData* node = &(nodedata[blockIdx.x]);

		for(int i=0;i<max_entry;i++)
		{
			float entry = node->nodeEntries[i][threadIdx.x];
			int index = node->nodeEntryIndex[i][threadIdx.x];

			_d[0] = d[index];
			_d[1] = d[index + numnodes];
			_d[2] = d[index + numnodes * 2];

			o[0] += entry * _d[0];
			o[1] += entry * _d[1];
			o[2] += entry * _d[2];
		}

		q[nodeno] = o[0];
		q[nodeno + numnodes] = o[1];
		q[nodeno + numnodes * 2] = o[2];

	}
}

__global__
void
makeVars(CGVars* vars, float* d, float* q, float*  r, int numnodes)
{
	float dq, rq, qq;
	dot(d,q,&dq,numnodes * 3);
	dot(r,q,&rq,numnodes * 3);
	dot(q,q,&qq,numnodes * 3);

	__syncthreads();

	if(threadIdx.x == 0)
	{
		vars->alpha = vars->deltaNew / dq;
		vars->deltaOld = vars->deltaNew;

		//r.r = r'.r' - 2*alpha*(r'.q) + alpha * alpha * (q.q)
		vars->deltaNew = vars->deltaNew - (2 * vars->alpha) * rq + (vars->alpha * vars->alpha) * qq;
		vars->beta = vars->deltaNew / vars->deltaOld;
	}
}

__global__
void
makeXRandD(CGVars* vars, float * x, float*  r, float*  d, float* q, int numnodes)
{
	int tid = threadIdx.x + blockIdx.x * VECTOR_BLOCK_SIZE;
	if(tid < numnodes)
	{
		float alpha = vars->alpha;
		float beta = vars->beta;
		float d1,d2,d3;
		float r1,r2,r3;

		d1 = d[tid];
		d2 =  d[tid + numnodes];
		d3 = d[tid + numnodes * 2];

		x[tid] = x[tid] + alpha * d1;
		x[tid + numnodes] = x[tid + numnodes] + alpha * d2;
		x[tid + numnodes * 2] = x[tid + numnodes * 2] + alpha * d3;

		r1 = r[tid] - alpha * q[tid];
		r2 = r[tid + numnodes] - alpha * q[tid + numnodes];
		r3 = r[tid + numnodes * 2] - alpha * q[tid + numnodes * 2];

		d[tid] = r1 + beta * d1;
		d[tid + numnodes] = r2 + beta * d2;
		d[tid + numnodes * 2] = r3 + beta * d3;

		r[tid] = r1;
		r[tid + numnodes] = r2;
		r[tid + numnodes * 2] = r3;
	}
} 

__global__
void constrainNodes(float* x, float* x0, char* constrained, int numnodes)
{
	int tid = threadIdx.x + blockIdx.x * VECTOR_BLOCK_SIZE;
	if(tid < numnodes)
	{
		if(constrained[tid])
		{
			x[tid] = x0[tid];
			x[tid + numnodes] = x0[tid + numnodes];
			x[tid + numnodes * 2] = x0[tid + numnodes * 2];
		}
	}
}


__host__
void GPUTimestep(int numtets, int numnodes, int max_entry)
{
	const int num_blocks_tet = (numtets/TET_BLOCK_SIZE) + 1;
	const int num_blocks_node = (numnodes/NODE_BLOCK_SIZE) + 1;
	const int num_blocks_vec = (numnodes/VECTOR_BLOCK_SIZE) + 1;

	//printf("Started\n");
	
	MakeSnAndV<<<num_blocks_vec, VECTOR_BLOCK_SIZE>>>(gpuptr_sn, gpuptr_x, gpuptr_xt, gpuptr_vt, gpuptr_fext, gpuptr_mass, numnodes);

	//checkCudaErrors("Make Sn, V");

	for(int i=0;i<PD_ITERATIONS;i++)
	{
		ProjectTransforms<<<num_blocks_tet, TET_BLOCK_SIZE>>>(gpuptr_TetData, gpuptr_x, numtets, numnodes);

		//checkCudaErrors("Project Transforms");

		makeBandRandD<<<num_blocks_node, NODE_BLOCK_SIZE>>>(gpuptr_TetData, gpuptr_NodeData, gpuptr_sn, gpuptr_b, numnodes);

		//checkCudaErrors("B, R, D");

		makeRandD<<<num_blocks_node, NODE_BLOCK_SIZE>>>(gpuptr_NodeData, gpuptr_b, gpuptr_x, gpuptr_CG_R, gpuptr_CG_D,max_entry, numnodes);

		//checkCudaErrors("R, D");

		initDeltaVars<<<1, DOT_BLOCK_SIZE>>>(gpuptr_CG_Vars, gpuptr_CG_R, numnodes);

		//checkCudaErrors("init CG Vars");

		for(int j=0;j<CG_ITERATIONS;j++)
		{
			MakeQ<<<num_blocks_node, NODE_BLOCK_SIZE>>>(gpuptr_NodeData, gpuptr_CG_D, gpuptr_CG_Q, max_entry, numnodes);

			//checkCudaErrors("q = Ad");

			makeVars<<<1, DOT_BLOCK_SIZE>>>(gpuptr_CG_Vars, gpuptr_CG_D, gpuptr_CG_Q, gpuptr_CG_R, numnodes);

			//checkCudaErrors("make CG vars");

			makeXRandD<<<num_blocks_vec, VECTOR_BLOCK_SIZE>>>(gpuptr_CG_Vars, gpuptr_x, gpuptr_CG_R, gpuptr_CG_D, gpuptr_CG_Q, numnodes);

			//checkCudaErrors("X, R, D");
		}

		constrainNodes<<<num_blocks_vec, VECTOR_BLOCK_SIZE>>>(gpuptr_x, gpuptr_x0, gpuptr_allowed, numnodes);
	}

}