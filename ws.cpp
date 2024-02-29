#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <thread>
#include <mutex>

// g++ -std=c++11 -o knap knapsack.cpp
// ./knap < ./inputs/0.txt
// ./knap < ./inputs/0.txt 4

using namespace std;

int* weights;
int* values;
int** opt;
int p;
int p_remaining;
thread* threads;
mutex primelock;

int knapsack(int n, int curr, int c)
{
	/*find the optimal value of the knapsack with
	items curr to n, capacity c, and weights and values given
	*/
	
	//base case, we are on the last item
	if(curr == n - 1)
	{
		if(weights[curr] <= c)
		{
			//take if we can
			return values[curr];
		}
		else
		{
			//leave otherwise
			return 0;
		}
	}
	
	if(opt[curr][c] != -1)
	{
		//value already computed
		return opt[curr][c];
	}
	
	// LEAVE CURR ITEM ----------------------
	int leave;
	bool haveThreads;

	primelock.lock();
	haveThreads =  p_remaining>0;
	if(haveThreads) p_remaining --;  // using a thread
	primelock.unlock();

	// if threads available, call knapsack with thread
	if(haveThreads){
		threads[p-p_remaining] = thread(knapsack, n, curr+1 ,c);
		threads[p-p_remaining].join(); // wait for it to finish
		leave = opt[curr+1][c];  // val is maxVal of i+1 to n items  //CHECK
	} else {
		// execute in current thread
		leave = knapsack(n, curr + 1, c);
	}

	// TAKING ITEM ----------------------
	int take = -1;
	
	//take the item, reducing capacity, but gaining value
	if(c >= weights[curr])
	{	
		primelock.lock();
		haveThreads =  p_remaining>0;
		if(haveThreads) p_remaining --;  // using a thread
		primelock.unlock();

		if(haveThreads){
			threads[p-p_remaining] = thread(knapsack, n, curr+1 , c-weights[curr]);
			threads[p-p_remaining].join(); // wait for it to finish
			// val is maxVal of i+1 to n items (just computed by thread) plus val of ith item  //CHECK
			take = opt[curr+1][c-weights[curr]] + values[curr];;  
		} else {
			take = knapsack(n, curr + 1, c-weights[curr]) + values[curr];
		}

	}
	
	// cout << "curr: " << curr << ", c: " << c << endl;

	if(leave > take)
	{
		opt[curr][c] = leave;
	}
	else
	{
		opt[curr][c] = take;
	}
	return opt[curr][c];
}

int main(int argc, char* argv[])
{
	using chrono::high_resolution_clock;
	using chrono::duration;
	
	auto start = high_resolution_clock::now();
	
	int n; //number of items in the knapsack
	cin >> n;
	int c; //capacity of the sack
	cin >> c;
	weights = new int[n]; //array to store the weights of the items
	values = new int[n]; //array to store the values of the items
	for(int i = 0; i<n; i++)
	{
		cin >> weights[i];
		cin >> values[i];
	}
	
	//initialize the array for dynamic programming
	opt = new int*[n];
	for(int i=0; i<n; i++)
	{
		opt[i] = new int[c+1];
		//intialize to -1
		for(int j=0; j<c+1; j++)
		{
			opt[i][j] = -1;
		}
	}

	p = atoi(argv[1]);
	threads = new thread[p];
	
	int highest = knapsack(n, 0, c);
	
	auto end = high_resolution_clock::now();
	duration<double, milli> time = end - start;
	
	cout << "The maximum value is " << highest << "." << endl;
	cout <<"Duration: "<<time.count() <<" miliseconds."<<endl;
}