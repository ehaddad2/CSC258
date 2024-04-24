#include <iostream>
#include <thread>
#include <stdlib.h>
#include <chrono>

using namespace std;

int** m1;
int** m2;
int** m3; //m1 * m2

void matrixmult(int n, int start, int end)
{
	for(int i=start; i<end; i++)
	{
		for(int j=0; j<n; j++)
		{
			//calculate m3[i][j]
			int sum = 0;
			for(int k=0; k<n; k++)
			{
				sum += m1[i][k] * m2[k][j];
			}
			m3[i][j] = sum;
		}
	}
}

int main(int argc, char* argv[])
{
	using chrono::high_resolution_clock;
	using chrono::duration;
	
	auto actualstart = high_resolution_clock::now();
	
	int n;
	cin >> n;
	
	m1 = new int*[n];
	m2 = new int*[n];
	m3 = new int*[n];
	
	for(int i=0; i<n; i++)
	{
		m1[i] = new int[n];
		m2[i] = new int[n];
		m3[i] = new int[n];
		
		for(int j=0; j<n; j++)
		{
			cin >> m1[i][j];
		}
	}
	
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			cin >> m2[i][j];
		}
	}
	
	int p = atoi(argv[1]);
	
	auto start = high_resolution_clock::now();
	
	thread* threads = new thread[p];
	
	for(int i=0; i<p; i++)
	{
		threads[i] = thread(matrixmult, n, n*i/p, n*(i+1)/p);
	}
	
	for(int i=0; i<p; i++)
	{
		threads[i].join();
	}
	
	auto end = high_resolution_clock::now();
	duration<double, milli> partime = end - start;
	duration<double, milli> actualtime = end - actualstart;
	
	cout<<"Parallel time is: "<<partime.count()<<" milliseconds."<<endl;
	cout<<"Total time is: "<<actualtime.count()<<" milliseconds."<<endl;
}