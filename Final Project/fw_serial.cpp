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

// Input: n - number of vertices
// Output: Transformed a that contains the shortest path lengths
void floydWarshallSerial(int n)
{
	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				mat[i][j] = std::min(mat[i][j], mat[i][k] + mat[k][j]);
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
	floydWarshallSerial(n);

	auto end = high_resolution_clock::now();
	duration<double, milli> fw_time = end - fw_start;
	duration<double, milli> actualtime = end - actualstart;

	cout << "Serial time is: " << fw_time.count() << " milliseconds." << endl;
	cout << "Total time is: " << actualtime.count() << " milliseconds." << endl;
}