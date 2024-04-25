#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>
#include <thread>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <fstream>

using namespace std;
using namespace chrono;

const int INF = numeric_limits<int>::max();
int n = 0; // num vertices
int p = 0; // total available threads
int **dist;
atomic<int> tasks_completed_ = 0;
condition_variable thread_barrier_;
mutex th_barrier_mutex_;



// thread pool constructor

class ThreadPool
{
public:
	// // Constructor to creates a thread pool with given
	// number of threads
	ThreadPool(size_t num_threads = thread::hardware_concurrency())
	{

		// Creating worker threads
		for (size_t i = 0; i < num_threads; ++i)
		{
			threads_.emplace_back([this]
								  { 
                while (true) { 
                    function<void()> task; 
                    // The reason for putting the below code 
                    // here is to unlock the queue before 
                    // executing the task so that other 
                    // threads can perform enqueue tasks 
                    { 
                        // Locking the queue so that data 
                        // can be shared safely 
                        unique_lock<mutex> lock(queue_mutex_); 
  
                        // Waiting until there is a task to 
                        // execute or the pool is stopped 
                        cv_.wait(lock, [this] { 
                            return !tasks_.empty() || stop_; 
                        }); 
  
                        // exit the thread in case the pool 
                        // is stopped and there are no tasks 
                        if (stop_ && tasks_.empty()) { 
                            return; 
                        } 
  
                        // Get the next task from the queue 
                        task = std::move(tasks_.front()); 
                        tasks_.pop(); 
                    } 
  
                    task(); 
                } });
		}
	}

	// Destructor to stop the thread pool
	~ThreadPool()
	{
		{
			// Lock the queue to update the stop flag safely
			unique_lock<mutex> lock(queue_mutex_);
			stop_ = true;
		}

		// Notify all threads
		cv_.notify_all();

		// Joining all worker threads to ensure they have
		// completed their tasks
		for (auto &thread : threads_)
		{
			thread.join();
		}
	}

	// Enqueue task for execution by the thread pool
	void enqueue(function<void()> task)
	{
		{
			unique_lock<std::mutex> lock(queue_mutex_);
			tasks_.emplace(std::move(task));
		}
		cv_.notify_one();
	}

private:
	// Vector to store worker threads
	vector<thread> threads_;

	// Queue of tasks
	queue<function<void()>> tasks_;

	// Mutex to synchronize access to shared data
	mutex queue_mutex_;

	// Condition variable to signal changes in the state of
	// the tasks queue
	condition_variable cv_;

	// Flag to indicate whether the thread pool should stop
	// or not
	bool stop_ = false;
};

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


void printMatrixToFile(int **mat, const std::string& filename) {
    // Open the output file for writing
    std::ofstream outputFile("outputs/" + filename);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }

    // Redirect the output to the output file stream
    std::ostream& output = outputFile;

    // Print each number from the matrix to the output file stream
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            output << mat[r][c] << "\n"; // Print each number on a separate line
        }
    }

    // Close the output file stream
    outputFile.close();
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
	tasks_completed_.fetch_add(1);
}

void floydWarshallParallel()
{

	int b = n / sqrt(p); // block dimensions are b x b

	// create pool with p threads and enqueue tasks
	ThreadPool pool(p);

	for (int k = 0; k < n; k++)
	{
		tasks_completed_ = 0; //reset counter
		for (int i = 0; i < sqrt(p); i++)
		{
			for (int j = 0; j < sqrt(p); j++)
			{
				pool.enqueue([k, i, j, b]
							 {
								 cout << "Task " << k << i << j << " is running on thread "
									  << this_thread::get_id() << endl;
								 innerLoop(k, i * b, j * b, (i * b) + b, (j * b) + b); });
			}
		}

		// ensure all tasks of this k iteration are done
		// std::unique_lock<std::mutex> lock(th_barrier_mutex_);
		// while(tasks_completed_.load() != p){
		// 	thread_barrier_.wait(lock);
		// }
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

	// get input matrix
	for (int i = 0; i < n; i++)
	{
		dist[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			cin >> dist[i][j];
			if (dist[i][j] == -1)
				dist[i][j] = INF;
		}
	}

	// parallel
	p = atoi(argv[1]);
	string output_filename = argv[2];

	auto parallel_start = high_resolution_clock::now();
	floydWarshallParallel();
	auto parallel_end = high_resolution_clock::now();
	duration<double, milli> parallel_time = parallel_end - parallel_start;
	printMatrixToFile(dist, output_filename);
	// printMatrix(dist);
	cout << "Parallel time is: " << parallel_time.count() << " milliseconds." << endl;
}