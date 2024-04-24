#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>
#include <thread>

using namespace std;
using namespace chrono;
const int INF = numeric_limits<int>::max();
int n = 0;
int **dist;
int **distCpy;

void printMatrix(int **mat)
{
	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			cout << mat[r][c] << " ";
		}
		cout << "\n";
	}
}

void innerLoop(int k, int startRow = 0, int startCol = 0, int endRow = n, int endCol = n)
{
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = startCol; j < endCol; j++)
		{
			if (dist[i][j] > (dist[i][k] + dist[k][j]) && (dist[k][j] != INF) && (dist[i][k] != INF))
				dist[i][j] = dist[i][k] + dist[k][j];
		}
	}
}

// Input: n - number of vertices
// Output: Transformed a that contains the shortest path lengths
void floydWarshallSerial(int n)
{
	for (int k = 0; k < n; k++)
	{
		innerLoop(k);
	}
}

int main(int argc, char *argv[])
{
	cin >> n;
	dist = new int *[n];
	distCpy = new int *[n];

	for (int i = 0; i < n; i++)
	{
		dist[i] = new int[n];
		distCpy[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			cin >> dist[i][j];
			if (dist[i][j] == -1)
				dist[i][j] = INF;
		}
		copy(dist[i], dist[i] + n, distCpy[i]);
	}

	// serial
	auto serial_start = high_resolution_clock::now();
	floydWarshallSerial(n);
	auto serial_end = high_resolution_clock::now();
	duration<double, milli> serial_time = serial_end - serial_start;
	printMatrix(dist);
	cout << "Serial time is: " << serial_time.count() << " milliseconds." << endl;

	// parallel
	dist = distCpy;
	int p = atoi(argv[1]);
	int b = n / sqrt(p); // block dimensions are b x b
	thread **threads = new thread *[p];
	for (int i = 0; i < sqrt(p); i++) // making 2d thread array
		threads[i] = new thread[sqrt(p)];

	auto parallel_start = high_resolution_clock::now();

	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < sqrt(p); i++)
		{
			for (int j = 0; j < sqrt(p); j++)
			{
				threads[i][j] = thread(innerLoop, k, i * b, j * b, (i * b) + b, (j * b) + b);
			}
		}

		for (int i = 0; i < sqrt(p); i++)
		{
			for (int j = 0; j < sqrt(p); j++)
			{
				threads[i][j].join();
			}
		}
	}

	auto parallel_end = high_resolution_clock::now();
	duration<double, milli> parallel_time = parallel_end - parallel_start;
	printMatrix(dist);
	cout << "Parallel time is: " << parallel_time.count() << " milliseconds." << endl;
}