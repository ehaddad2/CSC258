#include <iostream>
#include <stdlib.h>
#include <chrono>

using namespace std;


__global__
void matrixMultCuda(int n, int* a, int* b, int* c)
{
	//each thread will calculate a single value in c
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	int i = tid/n;
	if(i < n)
	{
		int j = tid%n;
		
		int total = 0;
		for(int k=0; k<n; k++)
		{
			total += a[i*n + k] * b[k*n + j];
		}
		c[tid] = total;
	}
}

int main(int argc, char* argv[])
{
	using chrono::high_resolution_clock;
	using chrono::duration;
	
	auto actualstart = high_resolution_clock::now();
	
	int n;
	cin >> n;
	int* m1;
	int* m2;
	int* m3;
	m1 = (int*)malloc(n*n*sizeof(int));
	m2 = (int*)malloc(n*n*sizeof(int));
	m3 = (int*)malloc(n*n*sizeof(int));
	
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			cin >> m1[i*n + j];
		}
	}
	
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			cin >> m2[i*n + j];
		}
	}
	
	auto start = high_resolution_clock::now();
	
	int* d_m1;
	int* d_m2;
	int* d_m3;
	
	cudaMalloc(&d_m1, n*n*sizeof(int));
	cudaMalloc(&d_m2, n*n*sizeof(int));
	cudaMalloc(&d_m3, n*n*sizeof(int));
	
	cudaMemcpy(d_m1, m1, n*n*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_m2, m2, n*n*sizeof(int), cudaMemcpyHostToDevice);
	
	int dthreadcount = atoi(argv[1]);
	
	matrixMultCuda<<< (n*n + dthreadcount-1)/dthreadcount, dthreadcount>>>(n, d_m1, d_m2, d_m3);

	cudaMemcpy(m3, d_m3, n*n*sizeof(int), cudaMemcpyDeviceToHost);
	
	auto end = high_resolution_clock::now();
	
	duration<double, milli> partime = end - start;
	duration<double, milli> actualtime = end - actualstart;
	
	cout<<"Parallel time is: "<<partime.count()<<" milliseconds."<<endl;
	cout<<"Total time is: "<<actualtime.count()<<" milliseconds."<<endl;
	
	cudaFree(d_m1);
	cudaFree(d_m2);
	cudaFree(d_m3);
	free(m1);
	free(m2);
	free(m3);
}