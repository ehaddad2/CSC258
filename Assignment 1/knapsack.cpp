#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <mutex>
using namespace std;
using namespace chrono;

mutex threadCntLock;
int** memo;
int* weights;
int* values;
thread* threads;
int p;
int pRemaining;

int knapsack(int n, int i, int remaining)
{
    //base case 1
    if(i == n) return 0;
    
    //base case 2
    if(memo[i][remaining] != -1)
    {
        return memo[i][remaining];
    }

    int leave = knapsack(n, i + 1, remaining);
    int take = -1;
    
    if(remaining >= weights[i])
    {
        //take the item if we can
        take = knapsack(n, i + 1, remaining-weights[i]) + values[i];
    }

    memo[i][remaining] = max(leave,take);
    return memo[i][remaining];
}

int knapsackMult(int n, int i, int remaining)
{
    //base case 1
    if(i == n) return 0;
    
    //base case 2
    if(memo[i][remaining] != -1)
    {
        return memo[i][remaining];
    }

    int leave;
    int canSpawn;

    threadCntLock.lock();
    canSpawn = pRemaining > 0;
    if (canSpawn) {
        pRemaining--;
    }
    threadCntLock.unlock();

    if (canSpawn) {//check if we can spawn new thread
        threads[p-pRemaining] = thread(knapsackMult, n, i+1, remaining);
        threads[p-pRemaining].join();
        leave = memo[i+1][remaining];
    }
    else {
        leave = knapsackMult(n,i+1, remaining);

    }
    int take = -1;
    
    if(remaining >= weights[i])
    {
        //take the item if we can
        threadCntLock.lock();
        canSpawn = pRemaining > 0;
        if (canSpawn) {
            pRemaining--;
        }
        threadCntLock.unlock();

        if (canSpawn) {//check if we can spawn new thread
            threads[p-pRemaining] = thread(knapsackMult, n, i+1, remaining-weights[i]);
            threads[p-pRemaining].join();
            take = memo[i+1][remaining-weights[i]]+values[i];
        }
        else{
            take = knapsackMult(n, i+1, remaining-weights[i])+values[i];
        }
    }

    memo[i][remaining] = max(leave,take);
    return memo[i][remaining];
}
int main(int argc, char* argv[])
{
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
    memo = new int* [n];
    for(int i = 0; i < n; i++)
    {
        memo[i] = new int[c+1];
        //intialize to -1
        for(int j = 0; j < c+1; j++)
        {
            memo[i][j] = -1;
        }
    }
    p = atoi(argv[1]);
    pRemaining = p;
	threads = new thread[p];
	/*
	
	for(int i=0; i<p; i++)
	{
		threads[i].join();
	}
    */
    int highest = knapsackMult(n, 0, c);

    auto end = high_resolution_clock::now();
    duration<double, milli> time = end - start;
    
    cout << "The maximum value is " << memo[0][c] << "." << endl;
    cout << "Duration: " << time.count() << " milliseconds." << endl;

}

void printMemo() {
    
}