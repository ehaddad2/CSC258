#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>

using namespace std;
const int INF = numeric_limits<int>::max();

int **mat;
int n = 0;

void printMatrix()
{
	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			cout << mat[r][c] << " ";
			;
		}
		cout << "\n";
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

// Input: n - number of vertices
// Output: Transformed a that contains the shortest path lengths
void floydWarshallSerial(int n)
{
	// printMatrix();

	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (mat[i][j] > (mat[i][k] + mat[k][j]) && (mat[k][j] != INF && mat[i][k] != INF))
					mat[i][j] = mat[i][k] + mat[k][j];

				// mat[i][j] = std::min(mat[i][j], mat[i][k] + mat[k][j]);
			}
		}
	}
	printMatrix();
}

int main(int argc, char *argv[])
{
	using chrono::duration;
	using chrono::high_resolution_clock;

	auto actualstart = high_resolution_clock::now();

	int p = atoi(argv[1]);

	cin >> n;
	mat = new int *[n];

	for (int i = 0; i < n; i++)
	{
		mat[i] = new int[n];

		for (int j = 0; j < n; j++)
		{
			cin >> mat[i][j];

			if (mat[i][j] == -1)
			{
				mat[i][j] = INF;
			}
		}
	}

	auto fw_start = high_resolution_clock::now();
	floydWarshallParallel(n, p);

	auto end = high_resolution_clock::now();
	duration<double, milli> fw_time = end - fw_start;
	duration<double, milli> actualtime = end - actualstart;

	cout << "Serial time is: " << fw_time.count() << " milliseconds." << endl;
	cout << "Total time is: " << actualtime.count() << " milliseconds." << endl;
}