#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>

#ifdef _OPENMP
#include "omp.h" // omp_get_num_threads
#endif

using namespace std;
const int INF = numeric_limits<int>::max();

int **mat_in;
int **mat_out;
int n = 0;
int b = 0;

void printMatrix()
{
	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			cout << mat_in[r][c] << " ";
			;
		}
		cout << "\n";
	}

	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			cout << mat_out[r][c] << " ";
			;
		}
		cout << "\n";
	}
}
void floyd_warshall_in_place(int *C, const int *A, const int *B, const int b, const int n)
{
	for (int k = 0; k < b; k++)
	{
		int kth = k * n;
		for (int i = 0; i < b; i++)
		{
			for (int j = 0; j < b; j++)
			{
				int sum = A[i * n + k] + B[kth + j];
				if (C[i * n + j] > sum)
				{
					C[i * n + j] = sum;
				}
			}
		}
	}
}

void floyd_warshall_blocked(const int *input, int *output, const int n, const int b)
{
	std::memcpy(output, input, n * n * sizeof(int));

	// for now, assume b divides n
	const int blocks = n / b;

	// note that [i][j] == [i * input_width * block_width + j * block_width]
	for (int k = 0; k < blocks; k++)
	{
		floyd_warshall_in_place(&output[k * b * n + k * b], &output[k * b * n + k * b], &output[k * b * n + k * b], b, n);
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for (int j = 0; j < blocks; j++)
		{
			if (j == k)
				continue;
			floyd_warshall_in_place(&output[k * b * n + j * b], &output[k * b * n + k * b], &output[k * b * n + j * b], b, n);
		}
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for (int i = 0; i < blocks; i++)
		{
			if (i == k)
				continue;
			floyd_warshall_in_place(&output[i * b * n + k * b], &output[i * b * n + k * b], &output[k * b * n + k * b], b, n);
			for (int j = 0; j < blocks; j++)
			{
				if (j == k)
					continue;
				floyd_warshall_in_place(&output[i * b * n + j * b], &output[i * b * n + k * b], &output[k * b * n + j * b], b, n);
			}
		}
	}
}

// n = num vertices, p = num threads
void floydWarshallParallel(int n, int p)
{
	for (int k = 1; k < n; k++)
	{
		// Each process P(i,j) that has a segment of the k-th row of D(k-1), broadcasts it to the P(*,j) processes

		// Each process P(i,j) that has a segment of the k-th column of D(k-1), broadcasts it to the P(i,*) processes

		// Each process waits to receive the needed segments

		// Each process computes its part of the D(k) matrix
	}
}


// ./par < test/geeks.txt 2
int main(int argc, char *argv[])
{
	using chrono::duration;
	using chrono::high_resolution_clock;

	auto actualstart = high_resolution_clock::now();

	int p = atoi(argv[1]);
	int b = atoi(argv[2]);

	cin >> n;
	mat_in = new int *[n];

	for (int i = 0; i < n; i++)
	{
		mat_in[i] = new int[n];

		for (int j = 0; j < n; j++)
		{
			cin >> mat_in[i][j];

			if (mat_in[i][j] == -1)
			{
				mat_in[i][j] = INF;
			}
		}
	}
	
	auto fw_start = high_resolution_clock::now();
	// floydWarshallParallel(n, p);
	floyd_warshall_blocked(mat_in, mat_out, n, b);

	auto end = high_resolution_clock::now();
	duration<double, milli> fw_time = end - fw_start;
	duration<double, milli> actualtime = end - actualstart;

	cout << "Parallel time is: " << fw_time.count() << " milliseconds." << endl;
	cout << "Total time is: " << actualtime.count() << " milliseconds." << endl;
}