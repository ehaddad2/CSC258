Authors: Stela Ciko, Elias Haddad
Floyd-Warshall: All Pairs Shortest Paths
CSC 258 - Final Project

References:
https://cse.buffalo.edu/faculty/miller/Courses/CSE633/Asmita-Gautam-Spring-2019.pdf
https://en.wikipedia.org/wiki/Parallel_all-pairs_shortest_path_algorithm#:~:text=Another%20variation%20of%20the%20problem,single-source%20shortest%20path%20algorithm


Compile:
    g++ -std=c++11 -o ser fw_serial.cpp
    g++ -std=c++11 -o par fw_parallel.cpp


Run:
    ./ser < test/geeks.txt b
    ./par < test/mat5.txt b


