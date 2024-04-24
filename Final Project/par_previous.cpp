#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>

#ifdef _OPENMP
#include "omp.h" // omp_get_num_threads
#endif

using namespace std;
const int INF = numeric_limits<int>::max();

int **mat;
int n = 0;  // vertices
int b = 0;  // blocks

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
}
 
// compute block values
void fwp(int start_r, int end_r, int start_c, int end_c){


	for (int i = start_r; i < end_r; i++)
		{
			for (int j = start_c; j < end_c; j++)
			{
				if (mat[i][j] > (mat[i][k] + mat[k][j])
                    && (mat[k][j] != INF
                        && mat[i][k] != INF))
                    mat[i][j] = mat[i][k] + mat[k][j];

				// mat[i][j] = std::min(mat[i][j], mat[i][k] + mat[k][j]);
			}
		}
}

// n = num vertices, p = num threads
void floydWarshallParallel()
{
	// for (int k = 1; k < n; k++)
	// {
	// 	// Each process P(i,j) that has a segment of the k-th row of D(k-1), broadcasts it to the P(*,j) processes
	// 	// Each process P(i,j) that has a segment of the k-th column of D(k-1), broadcasts it to the P(i,*) processes
	// 	// Each process waits to receive the needed segments
	// 	// Each process computes its part of the D(k) matrix
	// }


	// k-loop cant be parallelized
	for (int k = 0; k < n; k++)
	{	
		// split matrix into blocks and run in parallel
		
	}
	printMatrix();
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
	floydWarshallParallel(n, p);

	auto end = high_resolution_clock::now();
	duration<double, milli> fw_time = end - fw_start;
	duration<double, milli> actualtime = end - actualstart;

	cout << "Parallel time is: " << fw_time.count() << " milliseconds." << endl;
	cout << "Total time is: " << actualtime.count() << " milliseconds." << endl;
}