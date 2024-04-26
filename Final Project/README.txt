Authors: Stela Ciko, Elias Haddad
Floyd-Warshall: All Pairs Shortest Paths
CSC 258 - Final Project

References:
https://cse.buffalo.edu/faculty/miller/Courses/CSE633/Asmita-Gautam-Spring-2019.pdf
https://en.wikipedia.org/wiki/Parallel_all-pairs_shortest_path_algorithm#:~:text=Another%20variation%20of%20the%20problem,single-source%20shortest%20path%20algorithm


Compile:
    g++ project.cpp -o project -std=c++17
    g++ project_pool.cpp -o pool -std=c++17



Run:
    ./project < test/geeks.txt b output
    ./project 
    ./project < test/318.txt 954 318_36


    
    ./pool < test/geeks.txt [p] [output filename]
    ./pool < test/geeks6.txt 9 g6_9        
    ./pool < test/318.txt 954 318_36


